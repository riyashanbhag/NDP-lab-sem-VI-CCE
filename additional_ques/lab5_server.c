/*
------------------------------------------------------------
TRAVEL TICKET RESERVATION SERVER
Concurrent using fork()

Maintains:
Route | Available Seats | Booked Seats

Client sends:
RouteName SeatsToBook

Server:
- Checks availability
- Updates database
- Sends confirmation or failure
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

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(listenfd, (struct sockaddr*)&server_addr,
         sizeof(server_addr));

    listen(listenfd, 5);

    printf("Travel Reservation Server Running...\n");

    while (1) {

        connfd = accept(listenfd, NULL, NULL);

        if (fork() == 0) {

            close(listenfd);

            recv(connfd, buffer, sizeof(buffer), 0);

            char route[100];
            int seats_requested;

            sscanf(buffer, "%s %d", route, &seats_requested);

            FILE *fp = fopen("seats.txt", "r");
            FILE *temp = fopen("temp.txt", "w");

            char file_route[100];
            int available, booked;
            int found = 0;

            char response[MAX];

            while (fscanf(fp, "%s %d %d",
                          file_route, &available, &booked) != EOF) {

                if (strcmp(route, file_route) == 0) {

                    found = 1;

                    if (available >= seats_requested) {

                        available -= seats_requested;
                        booked += seats_requested;

                        fprintf(temp, "%s %d %d\n",
                                file_route, available, booked);

                        sprintf(response,
                                "Booking Successful!\nRemaining Seats: %d\nChild PID: %d",
                                available, getpid());
                    }
                    else {

                        fprintf(temp, "%s %d %d\n",
                                file_route, available, booked);

                        sprintf(response,
                                "Booking Failed!\nNot Enough Seats Available.");
                    }
                }
                else {
                    fprintf(temp, "%s %d %d\n",
                            file_route, available, booked);
                }
            }

            fclose(fp);
            fclose(temp);

            remove("seats.txt");
            rename("temp.txt", "seats.txt");

            if (!found) {
                strcpy(response, "Invalid Route!");
            }

            send(connfd, response, strlen(response)+1, 0);

            close(connfd);
            exit(0);
        }

        close(connfd);

        while(waitpid(-1, NULL, WNOHANG) > 0);
    }

    close(listenfd);
    return 0;
}
