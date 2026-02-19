/*
------------------------------------------------------------
Write two separate C programs (Server and Client)
using UNIX socket APIs for both:

1. Connection-Oriented Service (TCP)

The server should display the client's:
- Socket address
- IP address
- Port number
---------------------------------------------------------
TCP SERVER
Displays Client IP Address and Port Number
(Connection-Oriented Service)
------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {

    int server_fd, connfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len = sizeof(client_addr);

    // Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd,
         (struct sockaddr*)&server_addr,
         sizeof(server_addr));

    listen(server_fd, 5);

    printf("TCP Server Running...\n");

    // Accept connection
    connfd = accept(server_fd,
                    (struct sockaddr*)&client_addr,
                    &len);

    printf("\n--- Client Details (TCP) ---\n");

    printf("Client IP Address: %s\n",
           inet_ntoa(client_addr.sin_addr));

    printf("Client Port Number: %d\n",
           ntohs(client_addr.sin_port));

    printf("Client Socket Address: %u\n",
           client_addr.sin_addr.s_addr);

    close(connfd);
    close(server_fd);

    return 0;
}
