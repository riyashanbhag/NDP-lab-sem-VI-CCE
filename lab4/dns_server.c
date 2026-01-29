// dns_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090
#define MAX 1024

struct DNS {
    char domain[50];
    char ip[20];
};

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[MAX];

    // Example database
    struct DNS db[5];
    FILE *fp = fopen("database.txt", "r");
    int count = 0;

    if (!fp) {
        perror("Cannot open database.txt");
        exit(1);
    }

    // Read database from file
    while (fscanf(fp, "%s %s", db[count].domain, db[count].ip) != EOF) {
        count++;
        if (count >= 100) break; // limit
    }
    fclose(fp);

    // Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    listen(server_fd, 5);
    printf("DNS Server running on port %d...\n", PORT);

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }

        if (fork() == 0) {  // Child process handles client
            close(server_fd);

            memset(buffer, 0, sizeof(buffer));
            read(new_socket, buffer, sizeof(buffer));

            // Remove trailing newline if present
            buffer[strcspn(buffer, "\n")] = 0;

            char response[MAX];
            int found = 0;

            for (int i = 0; i < count; i++) {
                if (strcmp(buffer, db[i].domain) == 0) {
                    sprintf(response, "Domain: %s\nIP Address: %s", db[i].domain, db[i].ip);
                    found = 1;
                    break;
                }
            }

            if (!found) {
                sprintf(response, "Domain %s not found in database", buffer);
            }

            send(new_socket, response, strlen(response), 0);
            close(new_socket);
            exit(0);
        }

        close(new_socket); // Parent closes client socket
    }

    close(server_fd);
    return 0;
}
