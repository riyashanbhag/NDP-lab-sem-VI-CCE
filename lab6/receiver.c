/*
------------------------------------------------------------
Write a C program to transmit binary data from sender to receiver
using socket programming.

At the sender side:
- Generate parity bit (odd or even parity).
- Append parity bit to binary data.

At the receiver side:
- Check whether the received data is corrupt
  using parity checking technique.
- Display appropriate message.
------------------------------------------------------------
RECEIVER PROGRAM (Server)
Checks data using Parity Technique
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

    int server_fd, connfd;
    struct sockaddr_in addr;
    char data[MAX];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&addr,
         sizeof(addr));

    listen(server_fd, 5);

    printf("Receiver Waiting...\n");

    connfd = accept(server_fd, NULL, NULL);

    recv(connfd, data, sizeof(data), 0);

    printf("Received Data: %s\n", data);

    int count = 0;

    for (int i = 0; data[i] != '\0'; i++) {
        if (data[i] == '1')
            count++;
    }

    // EVEN PARITY CHECK
    if (count % 2 == 0)
        printf("Data is NOT corrupt (Even Parity Matched)\n");
    else
        printf("Data is CORRUPT\n");

    close(connfd);
    close(server_fd);

    return 0;
}
