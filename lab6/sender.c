/*
------------------------------------------------------------
SENDER PROGRAM (Client)
Generates parity bit and sends data
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
    struct sockaddr_in addr;
    char data[MAX];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr*)&addr,
            sizeof(addr));

    printf("Enter binary data: ");
    scanf("%s", data);

    int count = 0;

    for (int i = 0; data[i] != '\0'; i++) {
        if (data[i] == '1')
            count++;
    }

    char parity;

    // EVEN PARITY GENERATION
    if (count % 2 == 0)
        parity = '0';
    else
        parity = '1';

    int len = strlen(data);
    data[len] = parity;
    data[len+1] = '\0';

    printf("Data Sent with Parity Bit: %s\n", data);

    send(sock, data, strlen(data)+1, 0);

    close(sock);

    return 0;
}
