/*
------------------------------------------------------------
PROGRAM LOGIC
------------------------------------------------------------

1. Client creates TCP socket.
2. Client connects to server.
3. Client sends string to server.
4. Client receives processed result.
5. Client displays output.

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

    // 2️⃣ Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 3️⃣ Connect
    connect(sock,
            (struct sockaddr*)&server_addr,
            sizeof(server_addr));

    // 4️⃣ Input string
    printf("Enter a string: ");
    fgets(buffer, sizeof(buffer), stdin);

    buffer[strcspn(buffer, "\n")] = 0;  // Remove newline

    // 5️⃣ Send to server
    send(sock, buffer, strlen(buffer)+1, 0);

    // 6️⃣ Receive result
    recv(sock, response, sizeof(response), 0);

    printf("\n--- Server Response ---\n%s\n", response);

    close(sock);

    return 0;
}
