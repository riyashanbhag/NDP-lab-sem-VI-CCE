/*
------------------------------------------------------------
EMPLOYEE DATABASE SERVER (Concurrent using fork)

Client sends Employee ID.
Server returns:
- Name
- Salary
- Department
- Total Salary of that Department

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

    // Create socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(listenfd, (struct sockaddr*)&server_addr,
         sizeof(server_addr));

    listen(listenfd, 5);

    printf("Employee Server Running...\n");

    while (1) {

        connfd = accept(listenfd, NULL, NULL);

        if (fork() == 0) {

            close(listenfd);

            recv(connfd, buffer, sizeof(buffer), 0);

            int search_id = atoi(buffer);

            FILE *fp = fopen("employees.txt", "r");

            int emp_id, salary;
            char name[50], dept[50];

            int found = 0;
            int dept_total = 0;
            char found_dept[50];

            // First pass: find employee
            while (fscanf(fp, "%d %s %s %d",
                          &emp_id, name, dept, &salary) != EOF) {

                if (emp_id == search_id) {
                    found = 1;
                    strcpy(found_dept, dept);
                    break;
                }
            }

            if (!found) {
                send(connfd, "Employee Not Found",
                     18, 0);
                fclose(fp);
                close(connfd);
                exit(0);
            }

            rewind(fp); // Go back to start

            // Second pass: calculate total salary of department
            int id2, sal2;
            char n2[50], d2[50];

            while (fscanf(fp, "%d %s %s %d",
                          &id2, n2, d2, &sal2) != EOF) {

                if (strcmp(d2, found_dept) == 0)
                    dept_total += sal2;
            }

            fclose(fp);

            char response[MAX];

            sprintf(response,
                    "Name: %s\nDepartment: %s\nSalary: %d\nTotal Dept Salary: %d\nChild PID: %d",
                    name, found_dept, salary, dept_total, getpid());

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
