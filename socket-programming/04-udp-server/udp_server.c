#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // 1. Create UDP socket
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (server_fd == -1) {
        perror("socket");
        return 1;
    }

    // 2. Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 3. Bind socket to IP and port
    if (bind(server_fd, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_fd);
        return 1;
    }

    printf("UDP server listening on port %d...\n", PORT);

    // 4. Receive UDP datagram
    ssize_t bytes_received = recvfrom(
        server_fd,
        buffer,
        BUFFER_SIZE - 1,
        0,
        (struct sockaddr *)&client_addr,
        &client_len
    );

    if (bytes_received == -1) {
        perror("recvfrom");
        close(server_fd);
        return 1;
    }

    buffer[bytes_received] = '\0';

    // 5. Print received message
    printf("Received: %s\n", buffer);

    // 6. Close socket
    close(server_fd);

    return 0;
}