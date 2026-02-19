/*
------------------------------------------------------------
UDP FILE SERVER
Write two separate C programs (Server and Client) using
Socket APIs for TCP and UDP to implement a File Server.

Client sends filename to server.

If file exists:
    Server sends:
        - File contents
        - File size
        - Number of alphabets
        - Number of digits
        - Number of spaces
        - Number of lines
        - Number of other characters

If file does not exist:
    Server sends proper message.

Client continues until user enters "stop".
------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>

#define PORT 9090
#define MAX 8192

int main() {

    int sock;
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    char filename[100], response[MAX];

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(sock, (struct sockaddr*)&server,
         sizeof(server));

    printf("UDP File Server Running...\n");

    while (1) {

        recvfrom(sock, filename, sizeof(filename), 0,
                 (struct sockaddr*)&client, &len);

        if (strcmp(filename, "stop") == 0)
            break;

        FILE *fp = fopen(filename, "r");

        if (!fp) {
            strcpy(response, "File Not Found!");
            sendto(sock, response,
                   strlen(response)+1, 0,
                   (struct sockaddr*)&client, len);
            continue;
        }

        int alphabets=0, digits=0, spaces=0,
            lines=0, others=0, size=0;

        char ch;
        strcpy(response, "File Contents:\n\n");

        while ((ch = fgetc(fp)) != EOF) {

            size++;
            strncat(response, &ch, 1);

            if (isalpha(ch)) alphabets++;
            else if (isdigit(ch)) digits++;
            else if (ch==' ') spaces++;
            else if (ch=='\n') lines++;
            else others++;
        }

        fclose(fp);

        char stats[500];
        sprintf(stats,
        "\n\nFile Size: %d\nAlphabets: %d\nDigits: %d\nSpaces: %d\nLines: %d\nOthers: %d",
        size, alphabets, digits, spaces, lines, others);

        strcat(response, stats);

        sendto(sock, response,
               strlen(response)+1, 0,
               (struct sockaddr*)&client, len);
    }

    close(sock);
    return 0;
}
