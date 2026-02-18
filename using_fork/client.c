/*
------------------------------------------------------------
PROGRAM LOGIC – TCP CLIENT
------------------------------------------------------------
1. Client creates a TCP socket.
2. Client connects to server using IP and port.
3. User enters a string.
4. Client sends string to server.
5. Client receives processed result (uppercase + PID).
6. Client displays server response.

------------------------------------------------------------
*/

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
    char buffer[MAX];
    char response[MAX];

    // 1️⃣ Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        perror("Socket error");
        exit(1);
    }

    // 2️⃣ Prepare server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 3️⃣ Connect to server
    connect(sock,
            (struct sockaddr*)&server_addr,
            sizeof(server_addr));

    // 4️⃣ Take input from user
    printf("Enter a string: ");
    scanf("%s", buffer);

    // 5️⃣ Send to server
    send(sock, buffer, strlen(buffer)+1, 0);

    // 6️⃣ Receive response
    recv(sock, response, sizeof(response), 0);

    printf("\n--- Server Response ---\n%s\n", response);

    close(sock);

    return 0;
}
