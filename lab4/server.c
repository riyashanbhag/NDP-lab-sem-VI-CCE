// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <strings.h>

#define PORT 8080
#define MAX_STUDENTS 100

typedef struct {
    int reg_no;
    char name[50];
    char dept[20];
    int semester;
    char section[5];
    char courses[100];
    char address[50];
    char subject[20];
    int marks;
} Student;

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024], response[1024];

    Student students[MAX_STUDENTS];
    int student_count = 0;

    /* ---------- Load students from file ---------- */
    FILE *fp = fopen("students.txt", "r");
    if (!fp) {
        perror("students.txt");
        exit(1);
    }

    while (fscanf(fp, "%d %s %s %d %s %s %s %s %d",
                   &students[student_count].reg_no,
                   students[student_count].name,
                   students[student_count].dept,
                   &students[student_count].semester,
                   students[student_count].section,
                   students[student_count].courses,
                   students[student_count].address,
                   students[student_count].subject,
                   &students[student_count].marks) != EOF) {
        student_count++;
    }
    fclose(fp);

    /* ---------- ADMIN MENU ---------- */
    int choice;
    do {
        printf("\n--- ADMIN MENU ---\n");
        printf("1. Add Student\n");
        printf("2. Delete Student\n");
        printf("3. Modify Student\n");
        printf("4. Start Server\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1 && student_count < MAX_STUDENTS) {
            Student s;
            printf("Reg No: "); scanf("%d", &s.reg_no);
            printf("Name: "); scanf("%s", s.name);
            printf("Dept: "); scanf("%s", s.dept);
            printf("Semester: "); scanf("%d", &s.semester);
            printf("Section: "); scanf("%s", s.section);
            printf("Courses: "); scanf("%s", s.courses);
            printf("Address: "); scanf("%s", s.address);
            printf("Subject: "); scanf("%s", s.subject);
            printf("Marks: "); scanf("%d", &s.marks);

            students[student_count++] = s;
            printf("Student added.\n");
        }

        if (choice == 2) {
            int reg, found = 0;
            printf("Enter Reg No to delete: ");
            scanf("%d", &reg);

            for (int i = 0; i < student_count; i++) {
                if (students[i].reg_no == reg) {
                    students[i] = students[student_count - 1];
                    student_count--;
                    found = 1;
                    printf("Student deleted.\n");
                    break;
                }
            }
            if (!found) printf("Student not found.\n");
        }

        if (choice == 3) {
            int reg, found = 0;
            printf("Enter Reg No to modify: ");
            scanf("%d", &reg);

            for (int i = 0; i < student_count; i++) {
                if (students[i].reg_no == reg) {
                    printf("New Address: ");
                    scanf("%s", students[i].address);
                    printf("New Marks: ");
                    scanf("%d", &students[i].marks);
                    found = 1;
                    printf("Student modified.\n");
                    break;
                }
            }
            if (!found) printf("Student not found.\n");
        }

    } while (choice != 4);

    /* ---------- Save updated data ---------- */
    fp = fopen("students.txt", "w");
    for (int i = 0; i < student_count; i++) {
        fprintf(fp, "%d %s %s %d %s %s %s %s %d\n",
                students[i].reg_no,
                students[i].name,
                students[i].dept,
                students[i].semester,
                students[i].section,
                students[i].courses,
                students[i].address,
                students[i].subject,
                students[i].marks);
    }
    fclose(fp);

    /* ---------- Socket setup ---------- */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 5);

    printf("\nServer running on port %d...\n", PORT);

    /* ---------- Client handling ---------- */
    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address,
                            (socklen_t *)&addrlen);

        if (fork() == 0) {
            close(server_fd);

            char input[100];
            int option, found = 0;

            memset(buffer, 0, sizeof(buffer));
            memset(input, 0, sizeof(input));

            recv(new_socket, buffer, sizeof(buffer) - 1, 0);
            sscanf(buffer, "%d %[^\n]", &option, input);

            for (int i = 0; i < student_count; i++) {

                if (option == 1 && atoi(input) == students[i].reg_no) {
                    sprintf(response,
                            "Child PID: %d\nName: %s\nAddress: %s",
                            getpid(), students[i].name, students[i].address);
                    found = 1;
                    break;
                }

                if (option == 2 && strcasecmp(input, students[i].name) == 0) {
                    sprintf(response,
                            "Child PID: %d\nDept: %s\nSemester: %d\nSection: %s\nCourses: %s",
                            getpid(),
                            students[i].dept,
                            students[i].semester,
                            students[i].section,
                            students[i].courses);
                    found = 1;
                    break;
                }

                if (option == 3 && strcasecmp(input, students[i].subject) == 0) {
                    sprintf(response,
                            "Child PID: %d\nMarks in %s: %d",
                            getpid(),
                            students[i].subject,
                            students[i].marks);
                    found = 1;
                    break;
                }
            }

            if (!found)
                sprintf(response, "Child PID: %d\nRecord not found", getpid());

            send(new_socket, response, strlen(response), 0);
            close(new_socket);
            exit(0);
        }

        close(new_socket);
        wait(NULL);   // prevent zombies
    }
}
