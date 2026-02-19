/*
------------------------------------------------------------
PROGRAM LOGIC
------------------------------------------------------------
6. Server calculates:
      - Length of string
      - Number of vowels
      - Number of consonants
7.

------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main() {

    int server_fd, connfd;
    struct sockaddr_in server_addr;
    char buffer[MAX];
    char result[MAX];

    // 1️⃣ Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 2️⃣ Setup address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // 3️⃣ Bind
    bind(server_fd, (struct sockaddr*)&server_addr,
         sizeof(server_addr));

    // 4️⃣ Listen
    listen(server_fd, 5);

    printf("Server running...\n");

    // 5️⃣ Accept connection
    connfd = accept(server_fd, NULL, NULL);

    // 6️⃣ Receive string
    recv(connfd, buffer, sizeof(buffer), 0);

    int length = strlen(buffer);
    int vowels = 0, consonants = 0;

    // 7️⃣ Process string
    for(int i = 0; buffer[i]; i++) {

        char ch = tolower(buffer[i]);

        if(isalpha(ch)) {
            if(ch=='a'||ch=='e'||ch=='i'||ch=='o'||ch=='u')
                vowels++;
            else
                consonants++;
        }
    }

    // 8️⃣ Prepare result
    sprintf(result,
            "Length: %d\nVowels: %d\nConsonants: %d",
            length, vowels, consonants);

    // 9️⃣ Send back
    send(connfd, result, strlen(result)+1, 0);

    close(connfd);
    close(server_fd);

    return 0;
}
