/**
 * Experiment 4: Client–Server Communication using TCP Sockets (Server)
 * Course: Selected Topics in Computer Networks (CSMT 502)
 *
 * Theory:
 * TCP (Transmission Control Protocol) is a connection-oriented, reliable,
 * byte-stream transport layer protocol.
 * Communication Steps (Server):
 * 1. socket():  Create an endpoint for communication (SOCK_STREAM).
 * 2. bind():    Assign local IP address and port number to the socket.
 * 3. listen():  Put the socket into passive listening state for connection requests.
 * 4. accept():  Accept incoming connection from client (three-way handshake).
 * 5. recv()/send(): Exchange data reliably over the established TCP connection.
 * 6. close():   Close connection and release socket descriptor.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>

#define DEFAULT_PORT 8888
#define BUFFER_SIZE  1024

void str_toupper(char *dest, const char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = toupper((unsigned char)src[i]);
        i++;
    }
    dest[i] = '\0';
}

int main(int argc, char *argv[]) {
    int port = DEFAULT_PORT;
    if (argc > 1) {
        port = atoi(argv[1]);
    }

    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    char reply[BUFFER_SIZE + 64];

    // 1. Create TCP socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set SO_REUSEADDR to avoid "Address already in use" errors
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Configure server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all available interfaces
    server_addr.sin_port = htons(port);

    // 2. Bind socket to port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 3. Listen for connections (backlog = 5)
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("[TCP Server] Listening on 0.0.0.0:%d ...\n", port);
    fflush(stdout);

    // 4. Accept incoming connection
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    printf("[TCP Server] Connection established with client %s:%d\n", client_ip, ntohs(client_addr.sin_port));
    fflush(stdout);

    // 5. Echo / Service Loop
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_read = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_read <= 0) {
            printf("[TCP Server] Client disconnected.\n");
            break;
        }

        // Strip trailing newline/carriage return
        buffer[strcspn(buffer, "\r\n")] = '\0';
        printf("[TCP Server] Received: \"%s\"\n", buffer);

        if (strcasecmp(buffer, "exit") == 0 || strcasecmp(buffer, "quit") == 0) {
            snprintf(reply, sizeof(reply), "GOODBYE");
            send(client_fd, reply, strlen(reply), 0);
            printf("[TCP Server] Termination signal received. Closing session.\n");
            break;
        }

        // Echo back transformed message (Uppercase Echo + ACK)
        char upper[BUFFER_SIZE];
        str_toupper(upper, buffer);
        snprintf(reply, sizeof(reply), "ACK: %s", upper);
        send(client_fd, reply, strlen(reply), 0);
        printf("[TCP Server] Sent response: \"%s\"\n", reply);
        fflush(stdout);
    }

    // 6. Cleanup
    close(client_fd);
    close(server_fd);
    printf("[TCP Server] Socket closed. Server stopped.\n");
    return 0;
}
