#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1) {
        perror("socket");
        return 1;
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_fd);
        return 1;
    }

    // Listen for client
    if (listen(server_fd, 1) == -1) {
        perror("listen");
        close(server_fd);
        return 1;
    }

    printf("Chat server listening on port %d...\n", PORT);

    // Accept client
    client_fd = accept(server_fd, NULL, NULL);

    if (client_fd == -1) {
        perror("accept");
        close(server_fd);
        return 1;
    }

    printf("Client connected.\n");

    while (1) {
        // Receive message from client
        ssize_t bytes_read = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);

        if (bytes_read <= 0) {
            printf("Client disconnected.\n");
            break;
        }

        buffer[bytes_read] = '\0';

        printf("Client: %s", buffer);

        // Exit if client says bye
        if (strncmp(buffer, "bye", 3) == 0) {
            break;
        }

        // Get server response
        printf("Server: ");
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            break;
        }

        // Send response
        if (send(client_fd, buffer, strlen(buffer), 0) == -1) {
            perror("send");
            break;
        }

        if (strncmp(buffer, "bye", 3) == 0) {
            break;
        }
    }

    close(client_fd);
    close(server_fd);

    return 0;
}