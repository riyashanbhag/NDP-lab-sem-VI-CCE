/*
------------------------------------------------------------
MENU DRIVEN CALCULATOR SERVER
------------------------------------------------------------
Supports:
1. Add & Subtract
2. Solve ax + b = 0
3. Matrix Multiplication
4. Exit
------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 4096

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

    printf("Calculator Server Running...\n");

    connfd = accept(server_fd, NULL, NULL);

    while (1) {

        recv(connfd, buffer, sizeof(buffer), 0);

        int choice;
        sscanf(buffer, "%d", &choice);

        if (choice == 1) {

            int a, b;
            sscanf(buffer, "1 %d %d", &a, &b);

            sprintf(response,
                    "Addition: %d\nSubtraction: %d",
                    a + b, a - b);
        }

        else if (choice == 2) {

            float a, b, x;
            sscanf(buffer, "2 %f %f", &a, &b);

            if (a == 0)
                strcpy(response, "Invalid equation (a cannot be 0)");
            else {
                x = -b / a;
                sprintf(response,
                        "Value of x: %.2f", x);
            }
        }

        else if (choice == 3) {

            int r1, c1, r2, c2;
            int index = 1;

            sscanf(buffer, "3 %d %d %d %d",
                   &r1, &c1, &r2, &c2);

            if (c1 != r2) {
                strcpy(response,
                       "Matrix multiplication not possible");
            }
            else {

                int A[10][10], B[10][10], C[10][10];

                char *ptr = strchr(buffer, ' ');
                ptr = strchr(ptr+1, ' ');
                ptr = strchr(ptr+1, ' ');
                ptr = strchr(ptr+1, ' ');
                ptr = strchr(ptr+1, ' ');
                ptr++;

                for (int i=0;i<r1;i++)
                    for (int j=0;j<c1;j++)
                        sscanf(ptr, "%d", &A[i][j]),
                        ptr = strchr(ptr, ' ')+1;

                for (int i=0;i<r2;i++)
                    for (int j=0;j<c2;j++)
                        sscanf(ptr, "%d", &B[i][j]),
                        ptr = strchr(ptr, ' ')+1;

                for (int i=0;i<r1;i++)
                    for (int j=0;j<c2;j++) {
                        C[i][j] = 0;
                        for (int k=0;k<c1;k++)
                            C[i][j] += A[i][k]*B[k][j];
                    }

                strcpy(response, "Result Matrix:\n");

                char temp[100];
                for (int i=0;i<r1;i++) {
                    for (int j=0;j<c2;j++) {
                        sprintf(temp, "%d ", C[i][j]);
                        strcat(response, temp);
                    }
                    strcat(response, "\n");
                }
            }
        }

        else if (choice == 4) {
            strcpy(response, "Exiting...");
            send(connfd, response,
                 strlen(response)+1, 0);
            break;
        }

        send(connfd, response,
             strlen(response)+1, 0);
    }

    close(connfd);
    close(server_fd);

    return 0;
}
