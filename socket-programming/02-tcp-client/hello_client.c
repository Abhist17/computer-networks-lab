#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    const char *message = "Hello, world!";

    // 1. Create TCP socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (client_fd == -1) {
        perror("socket");
        return 1;
    }

    // 2. Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(client_fd);
        return 1;
    }

    // 3. Connect to server
    if (connect(client_fd, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) == -1) {
        perror("connect");
        close(client_fd);
        return 1;
    }

    printf("Connected to server.\n");

    // 4. Send message
    if (send(client_fd, message, strlen(message), 0) == -1) {
        perror("send");
        close(client_fd);
        return 1;
    }

    printf("Message sent: %s\n", message);

    // 5. Close connection
    close(client_fd);

    return 0;
}