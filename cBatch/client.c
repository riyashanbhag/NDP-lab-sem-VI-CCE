// client.c
// Transaction Client

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

    char input[MAX];
    char response[MAX];

    // 1️⃣ Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 2️⃣ Connect to server
    connect(sock,
            (struct sockaddr*)&server_addr,
            sizeof(server_addr));

    // 3️⃣ Take input
    printf("Enter Transaction String (Ex: TXN4500CR300DR): ");
    scanf("%s", input);

    // 4️⃣ Send to server
    send(sock, input, strlen(input)+1, 0);

    // 5️⃣ Receive response
    recv(sock, response, sizeof(response), 0);

    printf("\n--- Transaction Details ---\n%s\n", response);

    close(sock);

    return 0;
}
