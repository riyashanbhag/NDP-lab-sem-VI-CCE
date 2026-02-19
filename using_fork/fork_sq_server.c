/*
------------------------------------------------------------
PROGRAM LOGIC
------------------------------------------------------------

3. When client connects:
      - Parent writes received number to file.
      - Server creates child using fork().
4. Child:
      - Calculates square of number.
      - Gets its PID.
      - Sends square + PID to client.
      - Terminates.
5. Parent:
      - Closes connected socket.
      - Continues listening for new clients.

------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define PORT 8080
#define MAX 1024

int main() {

    int listenfd, connfd;
    struct sockaddr_in server_addr;
    char buffer[MAX];

    // 1️⃣ Create socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // 2️⃣ Bind
    bind(listenfd, (struct sockaddr*)&server_addr,
         sizeof(server_addr));

    // 3️⃣ Listen
    listen(listenfd, 5);

    printf("Concurrent Server running...\n");

    while (1) {

        // 4️⃣ Accept client
        connfd = accept(listenfd, NULL, NULL);

        // Receive number from client
        recv(connfd, buffer, sizeof(buffer), 0);

        int number = atoi(buffer);

        // 5️⃣ Parent writes number to file
        FILE *fp = fopen("numbers.txt", "a");
        fprintf(fp, "Received number: %d\n", number);
        fclose(fp);

        pid_t pid = fork();

        if (pid == 0) {
            // 🔹 CHILD PROCESS

            close(listenfd);

            int square = number * number;
            int child_pid = getpid();

            char response[MAX];

            sprintf(response,
                    "Child PID: %d\nSquare: %d",
                    child_pid, square);

            send(connfd, response, strlen(response)+1, 0);

            close(connfd);
            exit(0);
        }

        // 🔹 PARENT PROCESS
        close(connfd);

        // Prevent zombie processes
        while(waitpid(-1, NULL, WNOHANG) > 0);
    }

    close(listenfd);
    return 0;
}
