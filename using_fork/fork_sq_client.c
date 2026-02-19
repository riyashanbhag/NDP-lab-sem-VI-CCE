/*
------------------------------------------------------------
PROGRAM LOGIC
------------------------------------------------------------

1. Client creates TCP socket.
2. Client connects to server.
3. Client sends a number.
4. Client receives square and child PID.
5. Client displays result.

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
    char response[MAX];

    // 1️⃣ Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 2️⃣ Connect
    connect(sock,
            (struct sockaddr*)&server_addr,
            sizeof(server_addr));

    // 3️⃣ Input number
    printf("Enter a number: ");
    scanf("%s", buffer);

    // 4️⃣ Send to server
    send(sock, buffer, strlen(buffer)+1, 0);

    // 5️⃣ Receive result
    recv(sock, response, sizeof(response), 0);

    printf("\n--- Server Response ---\n%s\n", response);

    close(sock);

    return 0;
}
