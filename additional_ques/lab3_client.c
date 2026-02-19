/*
------------------------------------------------------------
MENU DRIVEN CALCULATOR CLIENT
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

        printf("\n--- MENU ---\n");
        printf("1. Add/Subtract\n");
        printf("2. Solve ax + b = 0\n");
        printf("3. Multiply Matrices\n");
        printf("4. Exit\n");
        printf("Enter choice: ");

        int choice;
        scanf("%d", &choice);

        if (choice == 1) {

            int a, b;
            printf("Enter two integers: ");
            scanf("%d %d", &a, &b);

            sprintf(buffer, "1 %d %d", a, b);
        }

        else if (choice == 2) {

            float a, b;
            printf("Equation ax + b = 0\n");
            printf("Enter a and b: ");
            scanf("%f %f", &a, &b);

            sprintf(buffer, "2 %f %f", a, b);
        }

        else if (choice == 3) {

            int r1,c1,r2,c2;
            printf("Enter rows & cols of Matrix A: ");
            scanf("%d %d",&r1,&c1);

            printf("Enter rows & cols of Matrix B: ");
            scanf("%d %d",&r2,&c2);

            sprintf(buffer,"3 %d %d %d %d ",r1,c1,r2,c2);

            printf("Enter Matrix A:\n");
            for(int i=0;i<r1;i++)
                for(int j=0;j<c1;j++){
                    int val;
                    scanf("%d",&val);
                    char temp[50];
                    sprintf(temp,"%d ",val);
                    strcat(buffer,temp);
                }

            printf("Enter Matrix B:\n");
            for(int i=0;i<r2;i++)
                for(int j=0;j<c2;j++){
                    int val;
                    scanf("%d",&val);
                    char temp[50];
                    sprintf(temp,"%d ",val);
                    strcat(buffer,temp);
                }
        }

        else if (choice == 4) {
            sprintf(buffer,"4");
            send(sock,buffer,strlen(buffer)+1,0);
            break;
        }

        send(sock, buffer,
             strlen(buffer)+1, 0);

        recv(sock, buffer,
             sizeof(buffer), 0);

        printf("\n--- Result ---\n%s\n", buffer);
    }

    close(sock);
    return 0;
}
