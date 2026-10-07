#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create TCP socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (client_fd == -1) {
        perror("socket");
        return 1;
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(client_fd);
        return 1;
    }

    // Connect to server
    if (connect(client_fd, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) == -1) {
        perror("connect");
        close(client_fd);
        return 1;
    }

    printf("Connected to chat server.\n");

    while (1) {
        // Get client message
        printf("Client: ");

        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            break;
        }

        // Send message
        if (send(client_fd, buffer, strlen(buffer), 0) == -1) {
            perror("send");
            break;
        }

        if (strncmp(buffer, "bye", 3) == 0) {
            break;
        }

        // Receive server response
        ssize_t bytes_read = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);

        if (bytes_read <= 0) {
            printf("Server disconnected.\n");
            break;
        }

        buffer[bytes_read] = '\0';

        printf("Server: %s", buffer);

        if (strncmp(buffer, "bye", 3) == 0) {
            break;
        }
    }

    close(client_fd);

    return 0;
}