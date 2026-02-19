/*
------------------------------------------------------------
TCP CLIENT
------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {

    int sock;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr =
            inet_addr("127.0.0.1");

    connect(sock,
            (struct sockaddr*)&server_addr,
            sizeof(server_addr));

    printf("Connected to TCP server\n");

    close(sock);

    return 0;
}
