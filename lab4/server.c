// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    char buffer[1024], response[1024];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 5);

    printf("Server running...\n");

    while (1) {
        int new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (new_socket < 0) {
            perror("accept");
            continue;
        }

        if (fork() == 0) {  // Child process
            close(server_fd);  // Child doesn’t need listening socket

            recv(new_socket, buffer, sizeof(buffer), 0);
            int option;
            char input[100];
            sscanf(buffer, "%d %[^\n]", &option, input);

            // trim newline just in case
            input[strcspn(input, "\n")] = 0;

            FILE *fp = fopen("students.txt", "r");
            int reg, sem, marks;
            char name[50], dept[20], section[5], courses[100];
            char address_stu[50], subject[20];
            int found = 0;

            if (fp) {
                while (fscanf(fp, "%d %s %s %d %s %s %s %s %d",
                              &reg, name, dept, &sem, section,
                              courses, address_stu, subject, &marks) != EOF) {

                    if (option == 1 && atoi(input) == reg) {
                        sprintf(response, "Child PID: %d\nName: %s\nAddress: %s",
                                getpid(), name, address_stu);
                        found = 1;
                        break;
                    }
                    if (option == 2 && strcmp(input, name) == 0) {
                        sprintf(response,
                                "Child PID: %d\nDept: %s\nSemester: %d\nSection: %s\nCourses: %s",
                                getpid(), dept, sem, section, courses);
                        found = 1;
                        break;
                    }
                    if (option == 3 && strcmp(input, subject) == 0) {
                        sprintf(response, "Child PID: %d\nMarks in %s: %d",
                                getpid(), subject, marks);
                        found = 1;
                        break;
                    }
                }
                fclose(fp);
            }

            if (!found) {
                sprintf(response, "Child PID: %d\nRecord not found", getpid());
            }

            send(new_socket, response, strlen(response), 0);
            close(new_socket);
            exit(0);
        }

        close(new_socket);  // Parent closes connected socket
    }

    close(server_fd);
    return 0;
}
