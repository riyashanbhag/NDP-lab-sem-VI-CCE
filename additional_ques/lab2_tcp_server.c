/*
------------------------------------------------------------
TCP FILE SERVER
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
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 8192

int main() {

    int server_fd, connfd;
    struct sockaddr_in addr;
    char filename[100], buffer[MAX], response[MAX];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&addr,
         sizeof(addr));

    listen(server_fd, 5);

    printf("TCP File Server Running...\n");

    connfd = accept(server_fd, NULL, NULL);

    while (1) {

        recv(connfd, filename, sizeof(filename), 0);

        if (strcmp(filename, "stop") == 0)
            break;

        FILE *fp = fopen(filename, "r");

        if (fp == NULL) {
            strcpy(response, "File Not Found!");
            send(connfd, response,
                 strlen(response)+1, 0);
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
            else if (ch == ' ') spaces++;
            else if (ch == '\n') lines++;
            else others++;
        }

        fclose(fp);

        char stats[500];
        sprintf(stats,
        "\n\nFile Size: %d bytes\nAlphabets: %d\nDigits: %d\nSpaces: %d\nLines: %d\nOther Characters: %d",
        size, alphabets, digits, spaces, lines, others);

        strcat(response, stats);

        send(connfd, response,
             strlen(response)+1, 0);
    }

    close(connfd);
    close(server_fd);

    return 0;
}
