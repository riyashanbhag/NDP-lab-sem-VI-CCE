/*
------------------------------------------------------------
QUESTION:
Write a TCP client program that communicates with
two different servers.

The client sends a number to:
1. Square Server (PORT 8080)
2. Cube Server (PORT 8081)

The client displays both results.
------------------------------------------------------------

PROGRAM LOGIC:
1. Create socket for Server 1.
2. Connect to PORT 8080.
3. Send number.
4. Receive square.
5. Close socket.
6. Create socket for Server 2.
7. Connect to PORT 8081.
8. Send number.
9. Receive cube.
10. Display both results.
------------------------------------------------------------
*/

/*
------------------------------------------------------------
QUESTION:
Single client communicates with two servers.
Server 1 returns square.
Server 2 returns cube.
------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT1 8080
#define PORT2 8081
#define MAX 1024

int main() {

    int sock1, sock2;
    struct sockaddr_in addr1, addr2;

    char number[MAX];     // stores original input
    char response[MAX];   // stores server replies

    printf("Enter a number: ");
    scanf("%s", number);

    // ------------------ CONNECT TO SQUARE SERVER ------------------

    sock1 = socket(AF_INET, SOCK_STREAM, 0);

    addr1.sin_family = AF_INET;
    addr1.sin_port = htons(PORT1);
    addr1.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock1, (struct sockaddr*)&addr1, sizeof(addr1)) < 0) {
        perror("Connection to Square Server failed");
        exit(1);
    }

    // Send original number
    send(sock1, number, strlen(number) + 1, 0);

    // Receive square result
    recv(sock1, response, sizeof(response), 0);

    printf("%s\n", response);

    close(sock1);


    // ------------------ CONNECT TO CUBE SERVER ------------------

    sock2 = socket(AF_INET, SOCK_STREAM, 0);

    addr2.sin_family = AF_INET;
    addr2.sin_port = htons(PORT2);
    addr2.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock2, (struct sockaddr*)&addr2, sizeof(addr2)) < 0) {
        perror("Connection to Cube Server failed");
        exit(1);
    }

    // Send SAME original number again
    send(sock2, number, strlen(number) + 1, 0);

    // Receive cube result
    recv(sock2, response, sizeof(response), 0);

    printf("%s\n", response);

    close(sock2);

    return 0;
}
