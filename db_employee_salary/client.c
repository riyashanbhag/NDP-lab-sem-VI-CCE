/*
------------------------------------------------------------
EMPLOYEE DATABASE CLIENT

Client sends Employee ID.
Displays employee details received from server.
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

    int sock;
    struct sockaddr_in server_addr;
    char buffer[MAX];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock,
            (struct sockaddr*)&server_addr,
            sizeof(server_addr));

    printf("Enter Employee ID: ");
    scanf("%s", buffer);

    send(sock, buffer, strlen(buffer)+1, 0);

    recv(sock, buffer, sizeof(buffer), 0);

    printf("\n--- Employee Details ---\n%s\n", buffer);

    close(sock);

    return 0;
}
