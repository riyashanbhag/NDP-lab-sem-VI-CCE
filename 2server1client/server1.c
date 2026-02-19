/*
------------------------------------------------------------
QUESTION:
Write a TCP server that calculates the square of a number
sent by a client and sends the result back.
------------------------------------------------------------

PROGRAM LOGIC:
1. Create TCP socket.
2. Bind to PORT 8080.
3. Listen for client connection.
4. Accept client.
5. Receive number.
6. Calculate square.
7. Send result back.
8. Close connection.
------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main() {

    int server_fd, connfd;
    struct sockaddr_in server_addr;
    char buffer[MAX];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind
    bind(server_fd, (struct sockaddr*)&server_addr,
         sizeof(server_addr));

    // Listen
    listen(server_fd, 5);

    printf("Square Server running on port %d...\n", PORT);

    // Accept client
    connfd = accept(server_fd, NULL, NULL);

    // Receive number
    recv(connfd, buffer, sizeof(buffer), 0);

    int num = atoi(buffer);
    int square = num * num;

    sprintf(buffer, "Square: %d", square);

    // Send result
    send(connfd, buffer, strlen(buffer)+1, 0);

    close(connfd);
    close(server_fd);

    return 0;
}
