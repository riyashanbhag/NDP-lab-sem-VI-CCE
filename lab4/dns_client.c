// dns_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 9090
#define MAX 1024

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[MAX];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation error");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    char domain[50];
    printf("Enter domain to resolve: ");
    fgets(domain, sizeof(domain), stdin);
    domain[strcspn(domain, "\n")] = 0;  // remove newline

    send(sock, domain, strlen(domain), 0);

    memset(buffer, 0, sizeof(buffer));
    read(sock, buffer, sizeof(buffer));

    printf("\n--- Server Response ---\n%s\n", buffer);

    close(sock);
    return 0;
}
