/*
------------------------------------------------------------
TCP FILE CLIENT
------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 8192

int main() {

    int sock;
    struct sockaddr_in addr;
    char filename[100], buffer[MAX];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr*)&addr,
            sizeof(addr));

    while (1) {

        printf("Enter filename (or stop): ");
        scanf("%s", filename);

        send(sock, filename,
             strlen(filename)+1, 0);

        if (strcmp(filename, "stop") == 0)
            break;

        recv(sock, buffer, sizeof(buffer), 0);

        printf("\n--- Server Response ---\n%s\n", buffer);
    }

    close(sock);
    return 0;
}
