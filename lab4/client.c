// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char message[1024];
    char buffer[1024] = {0};

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    int option;
    char input[100];

    printf("1. Registration Number\n");
    printf("2. Name of Student\n");
    printf("3. Subject Code\n");
    printf("Enter your choice: ");
    scanf("%d", &option);
    getchar();  // consume leftover newline from scanf

    printf("Enter data: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;  // remove trailing newline

    sprintf(message, "%d %s", option, input);
    send(sock, message, strlen(message), 0);

    read(sock, buffer, sizeof(buffer));
    printf("\n--- Server Response ---\n%s\n", buffer);

    close(sock);
    return 0;
}
