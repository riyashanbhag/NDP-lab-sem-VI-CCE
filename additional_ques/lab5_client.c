/*
------------------------------------------------------------
TRAVEL TICKET RESERVATION CLIENT

Client sends:
RouteName SeatsToBook

Displays server response and terminates.
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

    char route[100];
    int seats;

    printf("Available Routes:\n");
    printf("1. Mangalore-Mumbai\n");
    printf("2. Delhi-Chennai\n");

    printf("Enter Route (exact name): ");
    scanf("%s", route);

    printf("Enter number of seats: ");
    scanf("%d", &seats);

    sprintf(buffer, "%s %d", route, seats);

    send(sock, buffer, strlen(buffer)+1, 0);

    recv(sock, buffer, sizeof(buffer), 0);

    printf("\n--- Server Response ---\n%s\n", buffer);

    close(sock);

    return 0;
}
