/*
------------------------------------------------------------
BOOK DATABASE SERVER
Supports:
1. Insert
2. Delete
3. Display
4. Search (Title / Author)
5. Exit
------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 2048

void insert_book(char *data, char *response) {

    FILE *fp = fopen("books.txt", "a");

    fprintf(fp, "%s\n", data);

    fclose(fp);

    strcpy(response, "Book inserted successfully!");
}

void delete_book(char *acc_no, char *response) {

    FILE *fp = fopen("books.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    char line[MAX];
    char title[100], author[100], acc[50], pages[50], pub[100];
    int found = 0;

    while (fgets(line, sizeof(line), fp)) {

        sscanf(line, "%s %s %s %s %s",
               title, author, acc, pages, pub);

        if (strcmp(acc, acc_no) == 0) {
            found = 1;
            continue;
        }

        fprintf(temp, "%s", line);
    }

    fclose(fp);
    fclose(temp);

    remove("books.txt");
    rename("temp.txt", "books.txt");

    if (found)
        strcpy(response, "Book deleted successfully!");
    else
        strcpy(response, "Book not found!");
}

void display_books(char *response) {

    FILE *fp = fopen("books.txt", "r");

    char line[MAX];
    strcpy(response, "BOOK LIST:\n\n");

    while (fgets(line, sizeof(line), fp)) {
        strcat(response, line);
    }

    fclose(fp);
}

void search_book(char *key, char *response) {

    FILE *fp = fopen("books.txt", "r");

    char line[MAX];
    char title[100], author[100], acc[50], pages[50], pub[100];
    int found = 0;

    strcpy(response, "Search Results:\n\n");

    while (fgets(line, sizeof(line), fp)) {

        sscanf(line, "%s %s %s %s %s",
               title, author, acc, pages, pub);

        if (strcmp(title, key) == 0 ||
            strcmp(author, key) == 0) {

            strcat(response, line);
            found = 1;
        }
    }

    fclose(fp);

    if (!found)
        strcpy(response, "Book not found!");
}

int main() {

    int server_fd, connfd;
    struct sockaddr_in addr;
    char buffer[MAX], response[MAX];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&addr,
         sizeof(addr));

    listen(server_fd, 5);

    printf("Book Database Server Running...\n");

    connfd = accept(server_fd, NULL, NULL);

    while (1) {

        recv(connfd, buffer, sizeof(buffer), 0);

        int choice;
        sscanf(buffer, "%d", &choice);

        if (choice == 1) {
            char data[MAX];
            sscanf(buffer, "1 %[^\n]", data);
            insert_book(data, response);
        }
        else if (choice == 2) {
            char acc[50];
            sscanf(buffer, "2 %s", acc);
            delete_book(acc, response);
        }
        else if (choice == 3) {
            display_books(response);
        }
        else if (choice == 4) {
            char key[100];
            sscanf(buffer, "4 %s", key);
            search_book(key, response);
        }
        else if (choice == 5) {
            strcpy(response, "Exiting...");
            send(connfd, response, strlen(response)+1, 0);
            break;
        }

        send(connfd, response, strlen(response)+1, 0);
    }

    close(connfd);
    close(server_fd);
    return 0;
}
