/*
------------------------------------------------------------
PROGRAM LOGIC – CONCURRENT TCP SERVER USING fork()
------------------------------------------------------------

1. Client sends a string to the server.
2. Server accepts connection from client.
3. Server creates a child process using fork().

5. Child Process:
   - Receives string from client.
   - Converts string to uppercase.
   - Gets its Process ID using getpid().
   - Sends processed string along with PID to client.
   - Closes connection and terminates.
-----------------------------------------------------------
*/

// server.c
// Concurrent TCP Server using fork()

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main() {

    int listenfd, connfd;
    pid_t pid;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len;
    char buffer[MAX];

    // 1️⃣ Create TCP socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    if (listenfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // 2️⃣ Prepare server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // 3️⃣ Bind socket to IP and Port
    bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // 4️⃣ Listen for incoming connections
    listen(listenfd, 5);

    printf("Server running on port %d...\n", PORT);

    while (1) {

        len = sizeof(client_addr);

        // 5️⃣ Accept client connection
        connfd = accept(listenfd,
                        (struct sockaddr*)&client_addr,
                        &len);

        printf("Client connected...\n");

        // 6️⃣ Create child process
        pid = fork();

        if (pid == 0) {
            // 🔹 CHILD PROCESS

            close(listenfd);  // Child does not accept new clients

            recv(connfd, buffer, sizeof(buffer), 0);

            printf("Received from client: %s\n", buffer);

            // Convert string to uppercase
            for (int i = 0; buffer[i]; i++) {
                if (buffer[i] >= 'a' && buffer[i] <= 'z')
                    buffer[i] = buffer[i] - 32;
            }

            // Prepare response with PID
            char response[MAX];
            snprintf(response, sizeof(response),
         "Child PID: %d\nProcessed String: %s",
         getpid(), buffer);


            send(connfd, response, strlen(response)+1, 0);

            close(connfd);
            exit(0);  // Child terminates
        }

        // 🔹 PARENT PROCESS
        close(connfd);  // Parent closes connected socket
    }

    close(listenfd);
    return 0;
}
