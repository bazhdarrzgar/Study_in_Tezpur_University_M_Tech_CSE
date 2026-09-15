/**
 * Experiment 5: UDP Client–Server Communication (Server)
 * Course: Selected Topics in Computer Networks (CSMT 502)
 *
 * Theory:
 * UDP (User Datagram Protocol) is a connectionless, unreliable, message-oriented
 * transport layer protocol. No three-way handshake or connection state is maintained.
 *
 * Server Steps:
 * 1. socket():   Create datagram socket endpoint (SOCK_DGRAM).
 * 2. bind():     Bind socket to local IP address and port.
 * 3. recvfrom(): Block and wait for incoming datagram packet from any client.
 * 4. sendto():   Transmit datagram reply directly to client's sockaddr_in.
 * 5. close():    Close socket.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>

#define DEFAULT_PORT 9999
#define BUFFER_SIZE  1024

int main(int argc, char *argv[]) {
    int port = DEFAULT_PORT;
    if (argc > 1) {
        port = atoi(argv[1]);
    }

    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    char reply[BUFFER_SIZE + 64];

    // 1. Create UDP socket
    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("UDP socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // 2. Bind socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("UDP bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("[UDP Server] Listening for datagrams on 0.0.0.0:%d ...\n", port);
    fflush(stdout);

    // 3. Receive & Reply Loop
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_recv = recvfrom(server_fd, buffer, BUFFER_SIZE - 1, 0,
                                      (struct sockaddr *)&client_addr, &client_len);
        if (bytes_recv < 0) {
            perror("recvfrom error");
            break;
        }

        buffer[strcspn(buffer, "\r\n")] = '\0';
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
        printf("[UDP Server] Datagram from %s:%d -> \"%s\" (%zd bytes)\n",
               client_ip, ntohs(client_addr.sin_port), buffer, bytes_recv);

        if (strcasecmp(buffer, "exit") == 0 || strcasecmp(buffer, "quit") == 0) {
            snprintf(reply, sizeof(reply), "UDP Server Stopping. Goodbye!");
            sendto(server_fd, reply, strlen(reply), 0,
                   (struct sockaddr *)&client_addr, client_len);
            printf("[UDP Server] Exit command received. Terminating.\n");
            break;
        }

        // Echo back with UDP acknowledgment
        snprintf(reply, sizeof(reply), "UDP-ACK: [Echo] %s", buffer);
        sendto(server_fd, reply, strlen(reply), 0,
               (struct sockaddr *)&client_addr, client_len);
        printf("[UDP Server] Sent reply to %s:%d\n", client_ip, ntohs(client_addr.sin_port));
        fflush(stdout);
    }

    close(server_fd);
    printf("[UDP Server] Socket closed.\n");
    return 0;
}
