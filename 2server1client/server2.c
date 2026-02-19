/*
------------------------------------------------------------
QUESTION:
Write a TCP server that calculates the cube of a number
sent by a client and sends the result back.
------------------------------------------------------------

PROGRAM LOGIC:
1. Create TCP socket.
2. Bind to PORT 8081.
3. Listen for client.
4. Accept connection.
5. Receive number.
6. Calculate cube.
7. Send result.
------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define MAX 1024

int main() {

    int server_fd, connfd;
    struct sockaddr_in server_addr;
    char buffer[MAX];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&server_addr,
         sizeof(server_addr));

    listen(server_fd, 5);

    printf("Cube Server running on port %d...\n", PORT);

    connfd = accept(server_fd, NULL, NULL);

    recv(connfd, buffer, sizeof(buffer), 0);

    int num = atoi(buffer);
    int cube = num * num * num;

    sprintf(buffer, "Cube: %d", cube);

    send(connfd, buffer, strlen(buffer)+1, 0);

    close(connfd);
    close(server_fd);

    return 0;
}
