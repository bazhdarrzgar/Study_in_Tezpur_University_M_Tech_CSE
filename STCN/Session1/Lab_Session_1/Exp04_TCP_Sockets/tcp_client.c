/**
 * Experiment 4: Client–Server Communication using TCP Sockets (Client)
 * Course: Selected Topics in Computer Networks (CSMT 502)
 *
 * Theory:
 * TCP Client Steps:
 * 1. socket():  Create endpoint for communication (SOCK_STREAM).
 * 2. connect(): Initiate 3-way handshake with server IP and port.
 * 3. send():    Send message bytes to the server.
 * 4. recv():    Receive server's response.
 * 5. close():   Close socket connection.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define DEFAULT_IP   "127.0.0.1"
#define DEFAULT_PORT 8888
#define BUFFER_SIZE  1024

int main(int argc, char *argv[]) {
    const char *server_ip = DEFAULT_IP;
    int port = DEFAULT_PORT;
    const char *initial_msg = NULL;

    if (argc >= 2) {
        port = atoi(argv[1]);
    }
    if (argc >= 3) {
        initial_msg = argv[2];
    }

    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // 1. Create socket
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid server address");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    // 2. Connect to server
    printf("[TCP Client] Connecting to %s:%d ...\n", server_ip, port);
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    printf("[TCP Client] Successfully connected to server!\n");

    // If a message was supplied on CLI, send it, read response, then send exit
    if (initial_msg != NULL) {
        printf("[TCP Client] Sending: \"%s\"\n", initial_msg);
        send(sock_fd, initial_msg, strlen(initial_msg), 0);

        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_recv = recv(sock_fd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_recv > 0) {
            buffer[bytes_recv] = '\0';
            printf("[TCP Client] Server Response: \"%s\"\n", buffer);
        }

        // Send exit to cleanly terminate server
        printf("[TCP Client] Sending: \"exit\"\n");
        send(sock_fd, "exit", 4, 0);
        memset(buffer, 0, BUFFER_SIZE);
        recv(sock_fd, buffer, BUFFER_SIZE - 1, 0);
        printf("[TCP Client] Server final response: \"%s\"\n", buffer);
    } else {
        // Interactive mode
        printf("Type messages to send to server (type 'exit' to quit):\n");
        while (1) {
            printf("> ");
            if (!fgets(buffer, BUFFER_SIZE, stdin)) break;
            buffer[strcspn(buffer, "\r\n")] = '\0';

            if (strlen(buffer) == 0) continue;

            send(sock_fd, buffer, strlen(buffer), 0);

            char rx[BUFFER_SIZE];
            memset(rx, 0, BUFFER_SIZE);
            ssize_t n = recv(sock_fd, rx, BUFFER_SIZE - 1, 0);
            if (n <= 0) {
                printf("[TCP Client] Server closed connection.\n");
                break;
            }
            rx[n] = '\0';
            printf("Server replied: %s\n", rx);

            if (strcasecmp(buffer, "exit") == 0 || strcasecmp(buffer, "quit") == 0) {
                break;
            }
        }
    }

    close(sock_fd);
    printf("[TCP Client] Disconnected.\n");
    return 0;
}
