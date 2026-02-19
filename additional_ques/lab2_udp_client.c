/*
------------------------------------------------------------
UDP FILE CLIENT
------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 9090
#define MAX 8192

int main() {

    int sock;
    struct sockaddr_in server;
    socklen_t len = sizeof(server);

    char filename[100], buffer[MAX];

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1) {

        printf("Enter filename (or stop): ");
        scanf("%s", filename);

        sendto(sock, filename,
               strlen(filename)+1, 0,
               (struct sockaddr*)&server, len);

        if (strcmp(filename, "stop") == 0)
            break;

        recvfrom(sock, buffer, sizeof(buffer), 0,
                 NULL, NULL);

        printf("\n--- Server Response ---\n%s\n", buffer);
    }

    close(sock);
    return 0;
}
