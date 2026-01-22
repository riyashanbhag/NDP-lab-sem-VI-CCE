#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090 
#define MAX 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr;
    char buffer[MAX];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 1);

    printf("Server waiting for connection...\n");
    client_fd = accept(server_fd, NULL, NULL);
    printf("Client connected!\n");

    pid_t pid = fork();

    /* Child process: Receive messages */
    if (pid == 0) {
        while (1) {
            memset(buffer, 0, MAX);
            if (read(client_fd, buffer, MAX) <= 0)
                break;
            printf("\nFriend: %s", buffer);
        }
    }
    /* Parent process: Send messages */
    else {
        while (1) {
            memset(buffer, 0, MAX);
            fgets(buffer, MAX, stdin);
            write(client_fd, buffer, strlen(buffer));
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
