/*
------------------------------------------------------------
UDP CLIENT
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
    struct sockaddr_in server_addr;
    char message[] = "Hello";

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr =
            inet_addr("127.0.0.1");

    sendto(sock, message,
           strlen(message)+1, 0,
           (struct sockaddr*)&server_addr,
           sizeof(server_addr));

    printf("Message sent to UDP server\n");

    close(sock);

    return 0;
}
