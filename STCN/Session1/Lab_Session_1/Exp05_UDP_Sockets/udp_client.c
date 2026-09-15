/**
 * Experiment 5: UDP Client–Server Communication (Client)
 * Course: Selected Topics in Computer Networks (CSMT 502)
 *
 * Theory:
 * UDP Client Steps:
 * 1. socket():   Create datagram socket endpoint (SOCK_DGRAM).
 * 2. sendto():   Send datagram packet directly to target server address/port.
 * 3. recvfrom(): Await datagram response (with SO_RCVTIMEO timeout protection).
 * 4. close():    Close socket.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>

#define DEFAULT_IP   "127.0.0.1"
#define DEFAULT_PORT 9999
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
    socklen_t server_len = sizeof(server_addr);
    char buffer[BUFFER_SIZE];

    // 1. Create UDP socket
    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set receive timeout to 3 seconds so client does not hang indefinitely
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid server address");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    if (initial_msg != NULL) {
        printf("[UDP Client] Sending datagram to %s:%d: \"%s\"\n", server_ip, port, initial_msg);
        sendto(sock_fd, initial_msg, strlen(initial_msg), 0,
               (struct sockaddr *)&server_addr, server_len);

        memset(buffer, 0, BUFFER_SIZE);
        ssize_t n = recvfrom(sock_fd, buffer, BUFFER_SIZE - 1, 0,
                             (struct sockaddr *)&server_addr, &server_len);
        if (n > 0) {
            buffer[n] = '\0';
            printf("[UDP Client] Response from server: \"%s\"\n", buffer);
        } else {
            printf("[UDP Client] Timeout or no response from server.\n");
        }

        // Send exit message to shut down server
        printf("[UDP Client] Sending 'exit' command to server.\n");
        sendto(sock_fd, "exit", 4, 0, (struct sockaddr *)&server_addr, server_len);
        memset(buffer, 0, BUFFER_SIZE);
        n = recvfrom(sock_fd, buffer, BUFFER_SIZE - 1, 0,
                     (struct sockaddr *)&server_addr, &server_len);
        if (n > 0) {
            buffer[n] = '\0';
            printf("[UDP Client] Server reply: \"%s\"\n", buffer);
        }
    } else {
        printf("Interactive UDP Client connected to %s:%d (type 'exit' to quit):\n", server_ip, port);
        while (1) {
            printf("> ");
            if (!fgets(buffer, BUFFER_SIZE, stdin)) break;
            buffer[strcspn(buffer, "\r\n")] = '\0';
            if (strlen(buffer) == 0) continue;

            sendto(sock_fd, buffer, strlen(buffer), 0,
                   (struct sockaddr *)&server_addr, server_len);

            char rx[BUFFER_SIZE];
            memset(rx, 0, BUFFER_SIZE);
            ssize_t n = recvfrom(sock_fd, rx, BUFFER_SIZE - 1, 0,
                                 (struct sockaddr *)&server_addr, &server_len);
            if (n > 0) {
                rx[n] = '\0';
                printf("Server response: %s\n", rx);
            } else {
                printf("[UDP Client] Timed out waiting for response.\n");
            }

            if (strcasecmp(buffer, "exit") == 0 || strcasecmp(buffer, "quit") == 0) {
                break;
            }
        }
    }

    close(sock_fd);
    printf("[UDP Client] Closed.\n");
    return 0;
}
