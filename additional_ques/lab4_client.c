/*
------------------------------------------------------------
BOOK DATABASE CLIENT
Menu-driven client
------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 2048

int main() {

    int sock;
    struct sockaddr_in addr;
    char buffer[MAX];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr*)&addr,
            sizeof(addr));

    while (1) {

        printf("\n--- BOOK DATABASE MENU ---\n");
        printf("1. Insert Book\n");
        printf("2. Delete Book\n");
        printf("3. Display All Books\n");
        printf("4. Search Book\n");
        printf("5. Exit\n");
        printf("Enter choice: ");

        int choice;
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {

            char data[MAX];
            printf("Enter Title Author AccNo Pages Publisher:\n");
            fgets(data, sizeof(data), stdin);
            data[strcspn(data, "\n")] = 0;

            sprintf(buffer, "1 %s", data);
        }
        else if (choice == 2) {

            char acc[50];
            printf("Enter Accession Number to delete: ");
            scanf("%s", acc);

            sprintf(buffer, "2 %s", acc);
        }
        else if (choice == 3) {
            sprintf(buffer, "3");
        }
        else if (choice == 4) {

            char key[100];
            printf("Enter Title or Author: ");
            scanf("%s", key);

            sprintf(buffer, "4 %s", key);
        }
        else if (choice == 5) {
            sprintf(buffer, "5");
            send(sock, buffer, strlen(buffer)+1, 0);
            break;
        }

        send(sock, buffer, strlen(buffer)+1, 0);

        recv(sock, buffer, sizeof(buffer), 0);

        printf("\n--- Server Response ---\n%s\n", buffer);
    }

    close(sock);
    return 0;
}
