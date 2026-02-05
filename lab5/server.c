// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 2

int client_count = 0;
int client_sockets[10];

char msg1[50], msg2[50];
int received = 0;

pthread_mutex_t lock;

void *handle_client(void *arg) {
    int sock = *(int *)arg;
    char buffer[1024];
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);

    getpeername(sock, (struct sockaddr *)&addr, &len);
    recv(sock, buffer, sizeof(buffer), 0);

    pthread_mutex_lock(&lock);

    received++;
    if (received == 1)
        strcpy(msg1, buffer);
    else if (received == 2)
        strcpy(msg2, buffer);

    printf("Received \"%s\" from %s:%d\n",
           buffer, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

    // After both clients send data
    if (received == 2) {
        FILE *fp = fopen("data.txt", "r");
        char base[50];
        fscanf(fp, "%s", base);
        fclose(fp);

        printf("\n%s %s %s\n", base, msg1, msg2);
    }

    pthread_mutex_unlock(&lock);

    close(sock);
    pthread_exit(NULL);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    pthread_t tid;

    pthread_mutex_init(&lock, NULL);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 5);

    printf("Server running...\n");

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);

        pthread_mutex_lock(&lock);
        client_count++;
        client_sockets[client_count - 1] = new_socket;

        if (client_count > MAX_CLIENTS) {
            for (int i = 0; i < client_count - 1; i++)
                send(client_sockets[i], "terminate session", 18, 0);

            printf("More than 2 clients connected. Terminating...\n");
            exit(0);
        }
        pthread_mutex_unlock(&lock);

        pthread_create(&tid, NULL, handle_client, &new_socket);
    }

    return 0;
}
