// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[1024] = {0};
    char message[1024];
    int option;
    char input[100];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket error");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    printf("\n--- CLIENT MENU ---\n");
    printf("1. Registration Number\n");
    printf("2. Name of Student\n");
    printf("3. Subject Code\n");
    printf("Enter option: ");
    scanf("%d", &option);
    getchar();

    printf("Enter value: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;

    sprintf(message, "%d %s", option, input);
    send(sock, message, strlen(message), 0);

    recv(sock, buffer, sizeof(buffer), 0);
    printf("\n--- SERVER RESPONSE ---\n%s\n", buffer);

    close(sock);
    return 0;
}
