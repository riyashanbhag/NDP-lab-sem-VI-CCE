#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9091
#define BUF 1024

int main() {
    int sock;
    struct sockaddr_in serv;
    socklen_t len = sizeof(serv);
    char buffer[BUF];
    char input[BUF];

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT);
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("UDP Palindrome Client started. Type 'Halt' to exit.\n");

    while (1) {
        printf("\nEnter a string: ");
        fgets(input, BUF, stdin);
        input[strcspn(input, "\n")] = 0;  // Remove newline

        sendto(sock, input, strlen(input)+1, 0, (struct sockaddr *)&serv, len);

        if (strcmp(input, "Halt") == 0) {
            printf("Exiting client...\n");
            break;
        }

        recvfrom(sock, buffer, BUF, 0, NULL, NULL);
        printf("\n--- Server Response ---\n%s\n", buffer);
    }

    close(sock);
    return 0;
}
