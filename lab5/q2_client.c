#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock;
    struct sockaddr_in server;
    char str1[100], str2[100];
    char buffer[200], result[100];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    connect(sock, (struct sockaddr *)&server, sizeof(server));

    printf("Enter first string: ");
    scanf("%s", str1);

    printf("Enter second string: ");
    scanf("%s", str2);

    sprintf(buffer, "%s|%s", str1, str2);
    send(sock, buffer, strlen(buffer) + 1, 0);

    recv(sock, result, sizeof(result), 0);
    printf("\nServer Response: %s\n", result);

    close(sock);
    return 0;
}
