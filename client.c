#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

struct Book {
    int acc_no;
    char title[50];
    char author[50];
    int pages;
    char publisher[50];
};

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[1024];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    while (1) {
        int choice;
        printf("\n1.Insert\n2.Delete\n3.Display\n4.Search\n5.Exit\n");
        scanf("%d", &choice);
        getchar(); // clear buffer

        send(sock, &choice, sizeof(choice), 0);

        if (choice == 1) {
            struct Book b;

            printf("Acc No: ");
            scanf("%d", &b.acc_no);
            getchar();

            printf("Title: ");
            fgets(b.title, sizeof(b.title), stdin);

            printf("Author: ");
            fgets(b.author, sizeof(b.author), stdin);

            printf("Pages: ");
            scanf("%d", &b.pages);
            getchar();

            printf("Publisher: ");
            fgets(b.publisher, sizeof(b.publisher), stdin);

            send(sock, &b, sizeof(b), 0);

            recv(sock, buffer, sizeof(buffer), 0);
            printf("%s", buffer);
        }

        else if (choice == 2) {
            int acc;
            printf("Enter Acc No to delete: ");
            scanf("%d", &acc);

            send(sock, &acc, sizeof(acc), 0);

            recv(sock, buffer, sizeof(buffer), 0);
            printf("%s", buffer);
        }

        else if (choice == 3) {
            recv(sock, buffer, sizeof(buffer), 0);
            printf("%s", buffer);
        }

        else if (choice == 4) {
            char key[50];
            printf("Enter Title/Author: ");
            fgets(key, sizeof(key), stdin);

            send(sock, key, sizeof(key), 0);

            recv(sock, buffer, sizeof(buffer), 0);
            printf("%s", buffer);
        }

        else if (choice == 5) {
            break;
        }
    }

    close(sock);
}