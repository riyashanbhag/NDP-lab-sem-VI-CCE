#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090
#define MAX 1024

/* Function to swap characters */
void swap(char *x, char *y) {
    char temp = *x;
    *x = *y;
    *y = temp;
}

/* Recursive function to generate permutations */
void permute(char *str, int l, int r, char *result) {
    int i;
    if (l == r) {
        strcat(result, str);
        strcat(result, "\n");
        return;
    }
    for (i = l; i <= r; i++) {
        swap(&str[l], &str[i]);
        permute(str, l + 1, r, result);
        swap(&str[l], &str[i]); // backtrack
    }
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    char buffer[MAX];
    char result[MAX * 10];   // to store permutations

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    printf("UDP Server waiting for string...\n");

    len = sizeof(cliaddr);
    recvfrom(sockfd, buffer, MAX, 0,
             (struct sockaddr *)&cliaddr, &len);

    buffer[strcspn(buffer, "\n")] = '\0'; // remove newline
    printf("Received string: %s\n", buffer);

    result[0] = '\0';

    permute(buffer, 0, strlen(buffer) - 1, result);

    printf("\nPermutations:\n%s", result);

    /* Half duplex: server sends response only after receiving */
    sendto(sockfd, result, strlen(result), 0,
           (struct sockaddr *)&cliaddr, len);

    close(sockfd);
    return 0;
}
