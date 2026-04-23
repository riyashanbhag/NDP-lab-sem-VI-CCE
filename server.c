// server.c
#include <stdio.h>
#include <stdlib.h>
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

void insertBook(struct Book b) {
    FILE *fp = fopen("books.txt", "ab");
    fwrite(&b, sizeof(b), 1, fp);
    fclose(fp);
}

void displayBooks(int client_socket) {
    FILE *fp = fopen("books.txt", "rb");
    struct Book b;

    if (!fp) {
        send(client_socket, "No records\n", 11, 0);
        return;
    }

    while (fread(&b, sizeof(b), 1, fp)) {
        char buffer[200];
        sprintf(buffer, "Acc:%d Title:%s Author:%s Pages:%d Publisher:%s\n",
                b.acc_no, b.title, b.author, b.pages, b.publisher);
        send(client_socket, buffer, strlen(buffer), 0);
    }
    fclose(fp);
}

void searchBook(int client_socket, char *key) {
    FILE *fp = fopen("books.txt", "rb");
    struct Book b;
    int found = 0;

    while (fread(&b, sizeof(b), 1, fp)) {
        if (strcmp(b.title, key) == 0 || strcmp(b.author, key) == 0) {
            char buffer[200];
            sprintf(buffer, "Found: %s by %s\n", b.title, b.author);
            send(client_socket, buffer, strlen(buffer), 0);
            found = 1;
        }
    }

    if (!found)
        send(client_socket, "Not found\n", 10, 0);

    fclose(fp);
}

void deleteBook(int client_socket, int acc_no) {
    FILE *fp = fopen("books.txt", "rb");
    FILE *temp = fopen("temp.txt", "wb");
    struct Book b;
    int found = 0;

    while (fread(&b, sizeof(b), 1, fp)) {
        if (b.acc_no != acc_no)
            fwrite(&b, sizeof(b), 1, temp);
        else
            found = 1;
    }

    fclose(fp);
    fclose(temp);

    remove("books.txt");
    rename("temp.txt", "books.txt");

    if (found)
        send(client_socket, "Deleted\n", 8, 0);
    else
        send(client_socket, "Not found\n", 10, 0);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Server running...\n");

    client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

    while (1) {
        int choice;
        recv(client_socket, &choice, sizeof(choice), 0);

        if (choice == 1) {
            struct Book b;
            recv(client_socket, &b, sizeof(b), 0);
            insertBook(b);
            send(client_socket, "Inserted\n", 9, 0);
        }
        else if (choice == 2) {
            int acc;
            recv(client_socket, &acc, sizeof(acc), 0);
            deleteBook(client_socket, acc);
        }
        else if (choice == 3) {
            displayBooks(client_socket);
        }
        else if (choice == 4) {
            char key[50];
            recv(client_socket, key, sizeof(key), 0);
            searchBook(client_socket, key);
        }
        else if (choice == 5) {
            break;
        }
    }

    close(client_socket);
    close(server_fd);
}