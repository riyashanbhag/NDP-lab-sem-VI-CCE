#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9091
#define BUF 1024

// Function to check palindrome
int is_palindrome(char *str) {
    int i = 0, j = strlen(str) - 1;
    while (i < j) {
        if (tolower(str[i]) != tolower(str[j]))
            return 0;
        i++;
        j--;
    }
    return 1;
}

// Function to count vowels
void count_vowels(char *str, int *a, int *e, int *i_v, int *o, int *u) {
    *a = *e = *i_v = *o = *u = 0;
    for (int k = 0; str[k]; k++) {
        char c = tolower(str[k]);
        if (c == 'a') (*a)++;
        else if (c == 'e') (*e)++;
        else if (c == 'i') (*i_v)++;
        else if (c == 'o') (*o)++;
        else if (c == 'u') (*u)++;
    }
}

int main() {
    int sock;
    struct sockaddr_in serv, cli;
    socklen_t len = sizeof(cli);
    char buffer[BUF];

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(PORT);

    bind(sock, (struct sockaddr *)&serv, sizeof(serv));
    printf("UDP Palindrome Server is running on port %d...\n", PORT);

    while (1) {
        memset(buffer, 0, BUF);
        recvfrom(sock, buffer, BUF, 0, (struct sockaddr *)&cli, &len);

        if (strcmp(buffer, "Halt") == 0) {
            printf("Halt received. Server exiting...\n");
            break;
        }

        int palindrome = is_palindrome(buffer);
        int a, e, i_v, o, u;
        count_vowels(buffer, &a, &e, &i_v, &o, &u);
        int length = strlen(buffer);

        // Prepare response
        char response[BUF];
        sprintf(response,
            "String: %s\nPalindrome: %s\nLength: %d\nVowels - A:%d E:%d I:%d O:%d U:%d",
            buffer,
            palindrome ? "Yes" : "No",
            length,
            a, e, i_v, o, u
        );

        sendto(sock, response, strlen(response)+1, 0, (struct sockaddr *)&cli, len);
    }

    close(sock);
    return 0;
}
