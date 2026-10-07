#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    const char *message = "Hello, world!";

    client_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (client_fd == -1) {
        perror("socket");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(client_fd);
        return 1;
    }

    if (sendto(client_fd,
               message,
               strlen(message),
               0,
               (struct sockaddr *)&server_addr,
               sizeof(server_addr)) == -1) {
        perror("sendto");
        close(client_fd);
        return 1;
    }

    printf("Message sent: %s\n", message);

    close(client_fd);
    return 0;
}
