#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define DEFAULT_HOST "127.0.0.1"
#define DEFAULT_PORT 1500
#define DEFAULT_MSG  "hello"
#define BUFFER_SIZE  1024

static pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

static void safe_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    pthread_mutex_lock(&print_mutex);
    vprintf(format, args);
    fflush(stdout);
    pthread_mutex_unlock(&print_mutex);
    va_end(args);
}

typedef struct {
    int id;                        /* 1-based client ID, or 0 if default single client */
    int pool_size;                 /* Total clients running in pool */
    const char *host;
    int port;
    const char *msg;
    pthread_barrier_t *connect_barrier;
    pthread_barrier_t *send_barrier;
} client_task_t;

static void *client_worker(void *arg) {
    client_task_t *task = (client_task_t *)arg;
    char label[32];
    if (task->pool_size > 1 || task->id > 0) {
        snprintf(label, sizeof(label), "Client %d", task->id > 0 ? task->id : 1);
    } else {
        snprintf(label, sizeof(label), "Client");
    }

    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        safe_printf("[%s] Socket creation failed\n", label);
        return NULL;
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(task->port);

    if (inet_pton(AF_INET, task->host, &serv_addr.sin_addr) <= 0) {
        struct hostent *he = gethostbyname(task->host);
        if (he == NULL) {
            safe_printf("[%s] Error resolving hostname: %s\n", label, task->host);
            close(sd);
            return NULL;
        }
        memcpy(&serv_addr.sin_addr, he->h_addr_list[0], he->h_length);
    }

    if (connect(sd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        safe_printf("[%s] Connection to server failed\n", label);
        close(sd);
        return NULL;
    }

    safe_printf("[%s] Connected to server successfully.\n", label);

    /* If running in a pool, synchronize so all clients connect to server first */
    if (task->pool_size > 1 && task->connect_barrier != NULL) {
        pthread_barrier_wait(task->connect_barrier);

        if (task->id == 1) {
            safe_printf("\n--- All %d clients connected. Sending requests from all clients... ---\n\n",
                        task->pool_size);
        }

        pthread_barrier_wait(task->send_barrier);
    }

    /* Send request to server */
    char send_buffer[BUFFER_SIZE];
    snprintf(send_buffer, sizeof(send_buffer), "%s\n", task->msg);
    ssize_t bytes_sent = send(sd, send_buffer, strlen(send_buffer), 0);
    if (bytes_sent < 0) {
        safe_printf("[%s] Failed to send data\n", label);
        close(sd);
        return NULL;
    }
    safe_printf("[%s] Sent request: \"%s\"\n", label, task->msg);

    /* Receive response from server */
    char recv_buffer[BUFFER_SIZE];
    memset(recv_buffer, 0, sizeof(recv_buffer));
    ssize_t bytes_received = recv(sd, recv_buffer, sizeof(recv_buffer) - 1, 0);
    if (bytes_received < 0) {
        safe_printf("[%s] Failed to receive data\n", label);
        close(sd);
        return NULL;
    } else if (bytes_received == 0) {
        safe_printf("[%s] Server closed connection unexpectedly.\n", label);
    } else {
        recv_buffer[strcspn(recv_buffer, "\r\n")] = '\0';
        safe_printf("[%s] Server response received: \"%s\"\n", label, recv_buffer);
    }

    close(sd);
    safe_printf("[%s] Connection closed cleanly.\n", label);
    return NULL;
}

int main(int argc, char *argv[]) {
    const char *host = DEFAULT_HOST;
    int port = DEFAULT_PORT;
    const char *msg = DEFAULT_MSG;
    int pool_size = 1;
    int client_id = 0;

    int pos = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--pool") == 0 || strcmp(argv[i], "-p") == 0) {
            if (i + 1 < argc) {
                pool_size = atoi(argv[++i]);
            }
        } else if (strcmp(argv[i], "--id") == 0) {
            if (i + 1 < argc) {
                client_id = atoi(argv[++i]);
            }
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printf("Usage: %s [host] [port] [message] [pool_size]\n", argv[0]);
            printf("       %s [host] [port] [message] --pool <N>\n", argv[0]);
            printf("       %s [host] [port] [message] --id <ID>\n", argv[0]);
            return 0;
        } else {
            if (pos == 0) host = argv[i];
            else if (pos == 1) port = atoi(argv[i]);
            else if (pos == 2) msg = argv[i];
            else if (pos == 3) pool_size = atoi(argv[i]);
            pos++;
        }
    }

    if (pool_size < 1) pool_size = 1;

    if (pool_size == 1) {
        printf("=========================================\n");
        if (client_id > 0) {
            printf("  TCP Client %d Connecting to %s:%d\n", client_id, host, port);
        } else {
            printf("  TCP Client Connecting to %s:%d\n", host, port);
        }
        printf("  Sending message: \"%s\"\n", msg);
        printf("=========================================\n");

        client_task_t task;
        task.id = client_id;
        task.pool_size = 1;
        task.host = host;
        task.port = port;
        task.msg = msg;
        task.connect_barrier = NULL;
        task.send_barrier = NULL;

        client_worker(&task);
    } else {
        printf("=========================================\n");
        printf("  TCP Client Pool: %d Concurrent Clients\n", pool_size);
        printf("  Connecting to %s:%d\n", host, port);
        printf("  Sending message: \"%s\"\n", msg);
        printf("=========================================\n");

        pthread_barrier_t connect_barrier;
        pthread_barrier_t send_barrier;
        pthread_barrier_init(&connect_barrier, NULL, pool_size);
        pthread_barrier_init(&send_barrier, NULL, pool_size);

        pthread_t threads[pool_size];
        client_task_t tasks[pool_size];

        for (int i = 0; i < pool_size; i++) {
            tasks[i].id = i + 1;
            tasks[i].pool_size = pool_size;
            tasks[i].host = host;
            tasks[i].port = port;
            tasks[i].msg = msg;
            tasks[i].connect_barrier = &connect_barrier;
            tasks[i].send_barrier = &send_barrier;

            if (pthread_create(&threads[i], NULL, client_worker, &tasks[i]) != 0) {
                perror("pthread_create failed");
                exit(EXIT_FAILURE);
            }
        }

        for (int i = 0; i < pool_size; i++) {
            pthread_join(threads[i], NULL);
        }

        pthread_barrier_destroy(&connect_barrier);
        pthread_barrier_destroy(&send_barrier);
    }

    return 0;
}

