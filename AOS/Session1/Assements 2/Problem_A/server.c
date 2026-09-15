#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DEFAULT_PORT 1500
#define NUM_WORKERS 5
#define QUEUE_SIZE 32
#define BUFFER_SIZE 1024

typedef struct {
    int client_sockets[QUEUE_SIZE];
    int head;
    int tail;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} connection_queue_t;

typedef struct {
    int thread_id;
    pthread_t thread;
} worker_t;

static connection_queue_t queue;
static worker_t workers[NUM_WORKERS];
static volatile sig_atomic_t server_running = 1;
static int listen_sd = -1;

static void queue_init(connection_queue_t *q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->not_empty, NULL);
    pthread_cond_init(&q->not_full, NULL);
}

static void queue_push(connection_queue_t *q, int client_sd) {
    pthread_mutex_lock(&q->mutex);
    while (q->count == QUEUE_SIZE && server_running) {
        pthread_cond_wait(&q->not_full, &q->mutex);
    }
    if (!server_running) {
        pthread_mutex_unlock(&q->mutex);
        return;
    }
    q->client_sockets[q->tail] = client_sd;
    q->tail = (q->tail + 1) % QUEUE_SIZE;
    q->count++;
    pthread_cond_signal(&q->not_empty);
    pthread_mutex_unlock(&q->mutex);
}

static int queue_pop(connection_queue_t *q) {
    pthread_mutex_lock(&q->mutex);
    while (q->count == 0 && server_running) {
        pthread_cond_wait(&q->not_empty, &q->mutex);
    }
    if (!server_running && q->count == 0) {
        pthread_mutex_unlock(&q->mutex);
        return -1;
    }
    int client_sd = q->client_sockets[q->head];
    q->head = (q->head + 1) % QUEUE_SIZE;
    q->count--;
    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->mutex);
    return client_sd;
}

static void handle_signal(int sig) {
    (void)sig;
    server_running = 0;
    if (listen_sd >= 0) {
        close(listen_sd);
        listen_sd = -1;
    }
    pthread_mutex_lock(&queue.mutex);
    pthread_cond_broadcast(&queue.not_empty);
    pthread_cond_broadcast(&queue.not_full);
    pthread_mutex_unlock(&queue.mutex);
}

static void *worker_routine(void *arg) {
    worker_t *self = (worker_t *)arg;
    int worker_id = self->thread_id;
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    printf("[Server] Worker Thread %d initialized and ready.\n", worker_id);

    while (server_running) {
        int client_sd = queue_pop(&queue);
        if (client_sd < 0) {
            break;
        }

        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        getpeername(client_sd, (struct sockaddr *)&client_addr, &addr_len);
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
        int client_port = ntohs(client_addr.sin_port);

        printf("[Thread %d] Accepted connection from %s:%d (sd: %d)\n",
               worker_id, client_ip, client_port, client_sd);

        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_received = recv(client_sd, buffer, sizeof(buffer) - 1, 0);

        if (bytes_received > 0) {
            buffer[strcspn(buffer, "\r\n")] = '\0';
            printf("[Thread %d] Received request: \"%s\" from %s:%d\n",
                   worker_id, buffer, client_ip, client_port);

            snprintf(response, sizeof(response), "Hi, responding from Thread %d\n", worker_id);

            ssize_t bytes_sent = send(client_sd, response, strlen(response), 0);
            if (bytes_sent < 0) {
                perror("[Server] send error");
            } else {
                printf("[Thread %d] Sent response to %s:%d -> \"Hi, responding from Thread %d\"\n",
                       worker_id, client_ip, client_port, worker_id);
            }
        } else if (bytes_received == 0) {
            printf("[Thread %d] Client %s:%d disconnected before sending data.\n",
                   worker_id, client_ip, client_port);
        } else {
            perror("[Thread] recv error");
        }

        close(client_sd);
        printf("[Thread %d] Finished servicing client %s:%d. Connection closed.\n",
               worker_id, client_ip, client_port);
    }

    printf("[Server] Worker Thread %d shutting down.\n", worker_id);
    return NULL;
}

int main(int argc, char *argv[]) {
    setvbuf(stdout, NULL, _IONBF, 0);

    int port = DEFAULT_PORT;
    if (argc >= 2) {
        port = atoi(argv[1]);
        if (port <= 0 || port > 65535) {
            fprintf(stderr, "Usage: %s [port_number]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_signal;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    signal(SIGPIPE, SIG_IGN);

    queue_init(&queue);

    printf("==================================================\n");
    printf("  Concurrent TCP Server with Worker Thread Pool   \n");
    printf("  Port: %d | Worker Threads: %d                  \n", port, NUM_WORKERS);
    printf("==================================================\n");

    for (int i = 0; i < NUM_WORKERS; i++) {
        workers[i].thread_id = i + 1;
        if (pthread_create(&workers[i].thread, NULL, worker_routine, &workers[i]) != 0) {
            perror("pthread_create failed");
            exit(EXIT_FAILURE);
        }
    }

    listen_sd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt SO_REUSEADDR failed");
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    if (bind(listen_sd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind failed");
        close(listen_sd);
        exit(EXIT_FAILURE);
    }

    if (listen(listen_sd, 10) < 0) {
        perror("listen failed");
        close(listen_sd);
        exit(EXIT_FAILURE);
    }

    printf("[Server] Listening for incoming connections on port %d...\n", port);

    while (server_running) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_sd = accept(listen_sd, (struct sockaddr *)&client_addr, &addr_len);

        if (client_sd < 0) {
            if (!server_running) {
                break;
            }
            perror("accept error");
            continue;
        }

        queue_push(&queue, client_sd);
    }

    printf("\n[Server] Shutting down... waiting for worker threads to finish.\n");

    pthread_mutex_lock(&queue.mutex);
    pthread_cond_broadcast(&queue.not_empty);
    pthread_mutex_unlock(&queue.mutex);

    for (int i = 0; i < NUM_WORKERS; i++) {
        pthread_join(workers[i].thread, NULL);
    }

    pthread_mutex_destroy(&queue.mutex);
    pthread_cond_destroy(&queue.not_empty);
    pthread_cond_destroy(&queue.not_full);

    printf("[Server] Server shutdown complete.\n");
    return 0;
}
