#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090
#define BUF 1024

int main() {
    int sock;
    struct sockaddr_in serv;
    socklen_t len = sizeof(serv);
    char buffer[BUF], filename[100];

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT);
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter filename: ");
    scanf("%s", filename);
    sendto(sock, filename, sizeof(filename), 0,
           (struct sockaddr *)&serv, len);

    recvfrom(sock, buffer, BUF, 0, NULL, NULL);
    if (strcmp(buffer, "File OK") != 0) {
        printf("%s\n", buffer);
        close(sock);
        return 0;
    }

    while (1) {
        int opt;
        printf("\n1.Search 2.Replace 3.Reorder 4.Exit\n");
        scanf("%d", &opt);
        sendto(sock, &opt, sizeof(opt), 0,
               (struct sockaddr *)&serv, len);

        if (opt == 4)
            break;

        // -------- SEARCH --------
        if (opt == 1) {
            char str[50];
            int count;

            printf("Enter string: ");
            scanf("%s", str);
            sendto(sock, str, sizeof(str), 0,
                   (struct sockaddr *)&serv, len);

            int n = recvfrom(sock, &count, sizeof(count), 0, NULL, NULL);
            if (n == sizeof(int))
                printf("String found %d times\n", count);
            else {
                recvfrom(sock, buffer, BUF, 0, NULL, NULL);
                printf("%s\n", buffer);
            }
        }

        // -------- REPLACE --------
        else if (opt == 2) {
            char s1[50], s2[50];

            printf("Enter string to replace: ");
            scanf("%s", s1);
            printf("Enter new string: ");
            scanf("%s", s2);

            sendto(sock, s1, sizeof(s1), 0, (struct sockaddr *)&serv, len);
            sendto(sock, s2, sizeof(s2), 0, (struct sockaddr *)&serv, len);

            recvfrom(sock, buffer, BUF, 0, NULL, NULL);
            printf("%s\n", buffer);
        }

        // -------- REORDER --------
        else if (opt == 3) {
            recvfrom(sock, buffer, BUF, 0, NULL, NULL);
            printf("%s\n", buffer);
        }
    }

    close(sock);
    return 0;
}
