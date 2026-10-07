#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //gives the read(), write(), close() function
#include <arpa/inet.h> //gives the sockaddr_in structure and inet functions : inet_addr(), htons()
#include <sys/socket.h> //gives the socket functions : socket(), bind(), listen(), accept()  

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // 1. Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1) {
        perror("socket");
        return 1;
    }

    // 2. Configure server address
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP address
    server_addr.sin_port = htons(PORT); // Convert port number to network byte order

    // 3. Bind socket to IP address and port
    if (bind(server_fd, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_fd);
        return 1;
    }

    // 4. Listen for incoming connections
    if (listen(server_fd, 1) == -1) {
        perror("listen");
        close(server_fd);
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    // 5. Accept one client connection
    client_fd = accept(server_fd, NULL, NULL);

    if (client_fd == -1) {
        perror("accept");
        close(server_fd);
        return 1;
    }

    printf("Client connected.\n");

    // 6. Read data from client
    ssize_t bytes_read;

    while ((bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[bytes_read] = '\0';
        printf("Received: %s", buffer);
    }

    if (bytes_read == -1) {
        perror("read");
    }

    // 7. Close connections
    close(client_fd);
    close(server_fd);

    printf("\nConnection closed.\n");

    return 0;
}