// client.c
// TCP Client

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

    // 2️⃣ Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 3️⃣ Connect
    connect(sock,
            (struct sockaddr*)&server_addr,
            sizeof(server_addr));

    // 4️⃣ Take alphanumeric input
    printf("Enter Alphanumeric String: ");
    scanf("%s", input);

    // 5️⃣ Send to server
    send(sock, input, strlen(input)+1, 0);

    // 6️⃣ Receive encrypted + sorted result
    recv(sock, response, sizeof(response), 0);

    printf("\nEncrypted & Sorted Output:\n%s\n", response);

    close(sock);

    return 0;
}
