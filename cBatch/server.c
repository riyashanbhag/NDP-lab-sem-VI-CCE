/*
-----------------------------------------------
SERVER LOGIC – TRANSACTION PROCESSING PROGRAM
-----------------------------------------------

Step 1:
Server accepts transaction string from client.
Example:
TXN4500CR300DR

------------------------------------------------
PARENT PROCESS RESPONSIBILITIES:
------------------------------------------------

1. Accept connection from client.

2. Receive transaction string from client.

3. Write the original transaction string
   into the file "transactions.txt".

4. Create a child process using fork().

------------------------------------------------
CHILD PROCESS RESPONSIBILITIES:
------------------------------------------------

1. Extract Credit amount from string.
   Example:
   4500CR → Credit = 4500

2. Extract Debit amount from string.
   Example:
   300DR → Debit = 300

3. Calculate Total amount.
   Total = Credit + Debit

4. Get Child Process ID using getpid().

5. Store the following details into
   "transactions.txt":

   - Child PID
   - Credit amount
   - Debit amount
   - Total amount

6. Send the calculated details back
   to the client.

------------------------------------------------
CLIENT SIDE:
------------------------------------------------

1. Accept input from user.
2. Send transaction string to server.
3. Receive processed result.
4. Display result.*/

// server.c
// Fork + File Handling + Transaction Processing

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main() {

    int server_fd, connfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len;

    char buffer[MAX];

    // 1️⃣ Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    printf("Transaction Server Running...\n");

    while (1) {

        len = sizeof(client_addr);

        // 2️⃣ Accept client connection
        connfd = accept(server_fd,
                        (struct sockaddr*)&client_addr,
                        &len);

        recv(connfd, buffer, sizeof(buffer), 0);

        printf("Received: %s\n", buffer);

        // 3️⃣ Parent writes original transaction to file
        FILE *fp = fopen("transactions.txt", "a");
        fprintf(fp, "Transaction: %s\n", buffer);
        fclose(fp);

        // 4️⃣ Create child process
        if (fork() == 0) {

            // CHILD PROCESS

            close(server_fd);

            int credit = 0, debit = 0;
            int i = 0;

            // 5️⃣ Parse Credit Amount
            while (buffer[i] && !(buffer[i] >= '0' && buffer[i] <= '9'))
                i++;

            while (buffer[i] >= '0' && buffer[i] <= '9') {
                credit = credit * 10 + (buffer[i] - '0');
                i++;
            }

            if (buffer[i] == 'C' && buffer[i+1] == 'R')
                i += 2;

            // 6️⃣ Parse Debit Amount
            while (buffer[i] >= '0' && buffer[i] <= '9') {
                debit = debit * 10 + (buffer[i] - '0');
                i++;
            }

            int total = credit + debit;
            int pid = getpid();

            // 7️⃣ Store details in file
            FILE *fp2 = fopen("transactions.txt", "a");

            fprintf(fp2, "Child PID: %d\n", pid);
            fprintf(fp2, "Credit: %d\n", credit);
            fprintf(fp2, "Debit: %d\n", debit);
            fprintf(fp2, "Total: %d\n", total);
            fprintf(fp2, "--------------------------------\n");

            fclose(fp2);

            // 8️⃣ Send result back to client
            char response[MAX];

            sprintf(response,
                    "Child PID: %d\nCredit: %d\nDebit: %d\nTotal: %d",
                    pid, credit, debit, total);

            send(connfd, response, strlen(response)+1, 0);

            close(connfd);
            exit(0);
        }

        // Parent closes connection
        close(connfd);
    }

    close(server_fd);
    return 0;
}
