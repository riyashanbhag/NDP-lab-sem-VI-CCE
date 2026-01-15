#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090
#define BUF 1024

int main() {
    int sock;
    struct sockaddr_in serv, cli;
    socklen_t len = sizeof(cli);
    char buffer[BUF], filename[100];

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(PORT);

    bind(sock, (struct sockaddr *)&serv, sizeof(serv));

    printf("UDP Server waiting...\n");

    // Receive filename
    recvfrom(sock, filename, sizeof(filename), 0,
             (struct sockaddr *)&cli, &len);

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        sendto(sock, "File not present", 16, 0,
               (struct sockaddr *)&cli, len);
        close(sock);
        return 0;
    }
    fclose(fp);
    sendto(sock, "File OK", 7, 0,
           (struct sockaddr *)&cli, len);

    while (1) {
        int option;
        recvfrom(sock, &option, sizeof(option), 0,
                 (struct sockaddr *)&cli, &len);

        if (option == 4) {
            break;
        }

        // -------- SEARCH --------
        else if (option == 1) {
            char str[50], line[BUF];
            int count = 0;
            recvfrom(sock, str, sizeof(str), 0,
                     (struct sockaddr *)&cli, &len);

            fp = fopen(filename, "r");
            while (fgets(line, BUF, fp)) {
                char *p = line;
                while ((p = strstr(p, str)) != NULL) {
                    count++;
                    p += strlen(str);
                }
            }
            fclose(fp);

            if (count > 0)
                sendto(sock, &count, sizeof(count), 0,
                       (struct sockaddr *)&cli, len);
            else
                sendto(sock, "String not found", 16, 0,
                       (struct sockaddr *)&cli, len);
        }

        // -------- REPLACE --------
        else if (option == 2) {
            char s1[50], s2[50];
            char line[BUF], result[10000] = "";
            int found = 0;

            recvfrom(sock, s1, sizeof(s1), 0,
                     (struct sockaddr *)&cli, &len);
            recvfrom(sock, s2, sizeof(s2), 0,
                     (struct sockaddr *)&cli, &len);

            fp = fopen(filename, "r");
            while (fgets(line, BUF, fp)) {
                char *pos;
                while ((pos = strstr(line, s1))) {
                    found = 1;
                    strncat(result, line, pos - line);
                    strcat(result, s2);
                    strcpy(line, pos + strlen(s1));
                }
                strcat(result, line);
            }
            fclose(fp);

            if (found) {
                fp = fopen(filename, "w");
                fputs(result, fp);
                fclose(fp);
                sendto(sock, "String replaced", 15, 0,
                       (struct sockaddr *)&cli, len);
            } else {
                sendto(sock, "String not found", 16, 0,
                       (struct sockaddr *)&cli, len);
            }
        }

        // -------- REORDER --------
        else if (option == 3) {
            char data[10000];
            int n = 0;

            fp = fopen(filename, "r");
            n = fread(data, 1, sizeof(data), fp);
            fclose(fp);

            for (int i = 0; i < n - 1; i++) {
                for (int j = i + 1; j < n; j++) {
                    if (data[i] > data[j]) {
                        char t = data[i];
                        data[i] = data[j];
                        data[j] = t;
                    }
                }
            }

            fp = fopen(filename, "w");
            fwrite(data, 1, n, fp);
            fclose(fp);

            sendto(sock, "File reordered", 14, 0,
                   (struct sockaddr *)&cli, len);
        }
    }

    close(sock);
    return 0;
}
