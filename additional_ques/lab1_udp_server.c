/*
------------------------------------------------------------
Write two separate C programs (Server and Client)
using UNIX socket APIs for both:

1. Connection-Oriented Service (TCP)
2. Connectionless Service (UDP)

The server should display the client's:
- Socket address
- IP address
- Port number
------------------------------------------------------------
UDP SERVER
Displays Client IP and Port Number
(Connectionless Service)
------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 9090
#define MAX 100

int main() {

    int sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len = sizeof(client_addr);
    char buffer[MAX];

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sock,
         (struct sockaddr*)&server_addr,
         sizeof(server_addr));

    printf("UDP Server Running...\n");

    recvfrom(sock, buffer, sizeof(buffer), 0,
             (struct sockaddr*)&client_addr,
             &len);

    printf("\n--- Client Details (UDP) ---\n");

    printf("Client IP Address: %s\n",
           inet_ntoa(client_addr.sin_addr));

    printf("Client Port Number: %d\n",
           ntohs(client_addr.sin_port));

    printf("Client Socket Address: %u\n",
           client_addr.sin_addr.s_addr);

    close(sock);

    return 0;
}
