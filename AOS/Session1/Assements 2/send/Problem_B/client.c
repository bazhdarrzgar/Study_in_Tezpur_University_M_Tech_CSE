#define _DEFAULT_SOURCE
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

#define DEFAULT_PROXY_HOST "127.0.0.1"
#define DEFAULT_PROXY_PORT 8000
#define DEFAULT_FILE       "file1.txt"
#define BUFFER_SIZE        4096

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
    int id;
    int total_clients;
    const char *host;
    int port;
    const char *filename;
    pthread_barrier_t *connect_barrier;
    pthread_barrier_t *send_barrier;
} client_task_t;

static void *client_worker(void *arg) {
    client_task_t *task = (client_task_t *)arg;
    char label[32];
    if (task->total_clients > 1 || task->id > 0) {
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
            safe_printf("[%s] Cannot resolve host %s\n", label, task->host);
            close(sd);
            return NULL;
        }
        memcpy(&serv_addr.sin_addr, he->h_addr_list[0], he->h_length);
    }

    if (connect(sd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        safe_printf("[%s] Failed to connect to proxy server\n", label);
        close(sd);
        return NULL;
    }

    safe_printf("[%s] Connected to proxy server successfully.\n", label);

    /* If running in a pool, synchronize so all clients connect first */
    if (task->total_clients > 1 && task->connect_barrier != NULL) {
        pthread_barrier_wait(task->connect_barrier);

        if (task->id == 1) {
            safe_printf("\n--- All %d clients connected. Sending file requests concurrently... ---\n\n",
                        task->total_clients);
        }

        pthread_barrier_wait(task->send_barrier);
    }

    char request[512];
    snprintf(request, sizeof(request), "GET %s\n", task->filename);
    ssize_t sent = send(sd, request, strlen(request), 0);
    if (sent < 0) {
        safe_printf("[%s] Failed to send request\n", label);
        close(sd);
        return NULL;
    }
    safe_printf("[%s] Sent request: \"GET %s\"\n", label, task->filename);

    char buffer[BUFFER_SIZE];
    ssize_t n;
    size_t total_received = 0;
    char first_line[256] = {0};

    if (task->total_clients == 1) {
        safe_printf("\n--- [SERVER RESPONSE START] ---\n");
        while ((n = recv(sd, buffer, sizeof(buffer) - 1, 0)) > 0) {
            buffer[n] = '\0';
            printf("%s", buffer);
            total_received += n;
        }
        printf("\n--- [SERVER RESPONSE END] (%zu bytes received) ---\n", total_received);
    } else {
        int first_chunk = 1;
        while ((n = recv(sd, buffer, sizeof(buffer) - 1, 0)) > 0) {
            buffer[n] = '\0';
            if (first_chunk) {
                char *newline = strchr(buffer, '\n');
                size_t len = (newline != NULL) ? (size_t)(newline - buffer) : strlen(buffer);
                if (len >= sizeof(first_line)) len = sizeof(first_line) - 1;
                memcpy(first_line, buffer, len);
                first_line[len] = '\0';
                first_chunk = 0;
            }
            total_received += n;
        }

        if (strlen(first_line) > 0) {
            safe_printf("[%s] Server response: %s (%zu bytes received)\n", label, first_line, total_received);
        } else {
            safe_printf("[%s] Server response received (%zu bytes)\n", label, total_received);
        }
    }

    if (n < 0) {
        safe_printf("[%s] recv error\n", label);
    }

    close(sd);
    safe_printf("[%s] Connection closed cleanly.\n", label);
    return NULL;
}

int main(int argc, char *argv[]) {
    const char *host = DEFAULT_PROXY_HOST;
    int port = DEFAULT_PROXY_PORT;
    const char *files[64];
    int num_files = 0;
    int client_id = 0;

    int pos = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--id") == 0) {
            if (i + 1 < argc) client_id = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printf("Usage: %s [host] [port] [file1] [file2] ... [fileN]\n", argv[0]);
            printf("       %s [host] [port] [file] --id <ID>\n", argv[0]);
            return 0;
        } else {
            if (pos == 0) {
                host = argv[i];
            } else if (pos == 1) {
                port = atoi(argv[i]);
            } else {
                if (num_files < 64) {
                    files[num_files++] = argv[i];
                }
            }
            pos++;
        }
    }

    if (num_files == 0) {
        files[0] = DEFAULT_FILE;
        num_files = 1;
    }

    if (num_files == 1) {
        printf("====================================================\n");
        if (client_id > 0) {
            printf("  Client %d Connecting to Proxy Server %s:%d\n", client_id, host, port);
        } else {
            printf("  Client Connecting to Proxy Server %s:%d\n", host, port);
        }
        printf("  Requesting file: \"%s\"\n", files[0]);
        printf("====================================================\n");

        client_task_t task;
        task.id = client_id;
        task.total_clients = 1;
        task.host = host;
        task.port = port;
        task.filename = files[0];
        task.connect_barrier = NULL;
        task.send_barrier = NULL;

        client_worker(&task);
    } else {
        printf("====================================================\n");
        printf("  TCP Client Pool: %d Concurrent Clients\n", num_files);
        printf("  Target Proxy Server: %s:%d\n", host, port);
        printf("====================================================\n");

        pthread_barrier_t connect_barrier;
        pthread_barrier_t send_barrier;
        pthread_barrier_init(&connect_barrier, NULL, num_files);
        pthread_barrier_init(&send_barrier, NULL, num_files);

        pthread_t threads[num_files];
        client_task_t tasks[num_files];

        for (int i = 0; i < num_files; i++) {
            tasks[i].id = i + 1;
            tasks[i].total_clients = num_files;
            tasks[i].host = host;
            tasks[i].port = port;
            tasks[i].filename = files[i];
            tasks[i].connect_barrier = &connect_barrier;
            tasks[i].send_barrier = &send_barrier;

            if (pthread_create(&threads[i], NULL, client_worker, &tasks[i]) != 0) {
                perror("pthread_create failed");
                exit(EXIT_FAILURE);
            }
        }

        for (int i = 0; i < num_files; i++) {
            pthread_join(threads[i], NULL);
        }

        pthread_barrier_destroy(&connect_barrier);
        pthread_barrier_destroy(&send_barrier);
    }

    return 0;
}

