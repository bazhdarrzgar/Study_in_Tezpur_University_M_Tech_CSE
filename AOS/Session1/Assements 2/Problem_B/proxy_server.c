#define _DEFAULT_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DEFAULT_PORT     8000
#define PROXY_DIR        "proxy_files"
#define BUFFER_SIZE      4096
#define MAX_FILENAME_LEN 256

typedef struct {
    int client_sd;
    struct sockaddr_in client_addr;
    int worker_id;
} client_context_t;

static volatile sig_atomic_t server_running = 1;
static int listen_sd = -1;
static int thread_counter = 0;

static void handle_signal(int sig) {
    (void)sig;
    server_running = 0;
    if (listen_sd >= 0) {
        close(listen_sd);
        listen_sd = -1;
    }
}

static void sanitize_filename(char *dest, const char *src, size_t max_len) {
    const char *last_slash = strrchr(src, '/');
    const char *clean_name = (last_slash != NULL) ? last_slash + 1 : src;
    strncpy(dest, clean_name, max_len - 1);
    dest[max_len - 1] = '\0';
    dest[strcspn(dest, "\r\n ")] = '\0';
}

static void *client_handler(void *arg) {
    client_context_t *ctx = (client_context_t *)arg;
    int client_sd = ctx->client_sd;
    int worker_id = ctx->worker_id;

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &ctx->client_addr.sin_addr, client_ip, sizeof(client_ip));
    int client_port = ntohs(ctx->client_addr.sin_port);

    printf("[Proxy Thread %d] Handling connection from client %s:%d\n",
           worker_id, client_ip, client_port);

    char recv_buffer[BUFFER_SIZE];
    memset(recv_buffer, 0, sizeof(recv_buffer));
    ssize_t bytes_received = recv(client_sd, recv_buffer, sizeof(recv_buffer) - 1, 0);

    if (bytes_received <= 0) {
        close(client_sd);
        free(ctx);
        return NULL;
    }

    recv_buffer[bytes_received] = '\0';

    char req_file[MAX_FILENAME_LEN];
    char raw_name[MAX_FILENAME_LEN];
    if (sscanf(recv_buffer, "GET %255s", raw_name) == 1) {
        sanitize_filename(req_file, raw_name, sizeof(req_file));
    } else {
        sanitize_filename(req_file, recv_buffer, sizeof(req_file));
    }

    printf("[Proxy Thread %d] Client requested: \"%s\"\n", worker_id, req_file);

    char file_path[512];
    snprintf(file_path, sizeof(file_path), "%s/%s", PROXY_DIR, req_file);

    FILE *fp = fopen(file_path, "rb");
    if (fp != NULL) {
        printf("[Proxy Thread %d] Found designated proxy copy for \"%s\". Transmitting...\n",
               worker_id, req_file);

        char header[BUFFER_SIZE];
        snprintf(header, sizeof(header),
                 "STATUS: 200 OK (Served from Proxy Copy)\n"
                 "FILE: %s\n"
                 "----------------------------------------\n",
                 req_file);
        send(client_sd, header, strlen(header), 0);

        char file_buf[BUFFER_SIZE];
        size_t n;
        while ((n = fread(file_buf, 1, sizeof(file_buf), fp)) > 0) {
            send(client_sd, file_buf, n, 0);
        }
        fclose(fp);

        printf("[Proxy Thread %d] Successfully sent proxy copy of \"%s\" to %s:%d\n",
               worker_id, req_file, client_ip, client_port);
    } else {
        printf("[Proxy Thread %d] \"%s\" is not in designated proxy copy.\n",
               worker_id, req_file);

        char err_msg[BUFFER_SIZE];
        snprintf(err_msg, sizeof(err_msg),
                 "STATUS: 404 NOT FOUND\n"
                 "ERROR: \"%s\" not found in proxy copy.\n"
                 "Designated available proxy files: file1.txt, file2.txt\n",
                 req_file);
        send(client_sd, err_msg, strlen(err_msg), 0);
    }

    close(client_sd);
    printf("[Proxy Thread %d] Finished request. Connection closed.\n", worker_id);
    free(ctx);
    return NULL;
}

int main(int argc, char *argv[]) {
    setvbuf(stdout, NULL, _IONBF, 0);

    int port = DEFAULT_PORT;
    if (argc >= 2) port = atoi(argv[1]);

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_signal;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    signal(SIGPIPE, SIG_IGN);

    mkdir(PROXY_DIR, 0755);

    listen_sd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#ifdef SO_REUSEPORT
    setsockopt(listen_sd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
#endif


    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    if (bind(listen_sd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Bind failed");
        close(listen_sd);
        exit(EXIT_FAILURE);
    }

    if (listen(listen_sd, 16) < 0) {
        perror("Listen failed");
        close(listen_sd);
        exit(EXIT_FAILURE);
    }

    printf("========================================================\n");
    printf("  Multithreaded Proxy Server Started                    \n");
    printf("  Listening on Port: %d                                 \n", port);
    printf("  Designated Proxy Files: ./%s/ (file1.txt, file2.txt)  \n", PROXY_DIR);
    printf("========================================================\n");

    while (server_running) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_sd = accept(listen_sd, (struct sockaddr *)&client_addr, &addr_len);

        if (client_sd < 0) {
            if (!server_running) break;
            perror("Accept failed");
            continue;
        }

        client_context_t *ctx = malloc(sizeof(client_context_t));
        if (!ctx) {
            close(client_sd);
            continue;
        }
        ctx->client_sd = client_sd;
        ctx->client_addr = client_addr;
        ctx->worker_id = ++thread_counter;

        pthread_t tid;
        if (pthread_create(&tid, NULL, client_handler, ctx) != 0) {
            perror("pthread_create failed");
            close(client_sd);
            free(ctx);
        } else {
            pthread_detach(tid);
        }
    }

    if (listen_sd >= 0) close(listen_sd);
    printf("[Proxy Server] Stopped.\n");
    return 0;
}
