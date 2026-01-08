#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

void sortArray(int arr[], int n, int ascending) {
    int temp;
    for (int i = 0; i < n-1; i++) {
        for (int j = i+1; j < n; j++) {
            if ((ascending && arr[i] > arr[j]) || (!ascending && arr[i] < arr[j])) {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[MAX];
    
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Client connected!\n");

    while(1) {
        int n;
        int arr[100], choice, num;

        // Receive data from client
        read(new_socket, &n, sizeof(int));
        if (n == 0) break; // client wants to exit
        read(new_socket, arr, n * sizeof(int));
        read(new_socket, &choice, sizeof(int));

        char response[MAX];
        response[0] = '\0';

        if (choice == 1) { // Search
            read(new_socket, &num, sizeof(int));
            int found = 0;
            for (int i = 0; i < n; i++) {
                if (arr[i] == num) {
                    found = 1;
                    break;
                }
            }
            if (found)
                sprintf(response, "Number %d found in array.", num);
            else
                sprintf(response, "Number %d not found in array.", num);
        } 
        else if (choice == 2) { // Sort ascending
            sortArray(arr, n, 1);
            sprintf(response, "Sorted Ascending: ");
            for (int i = 0; i < n; i++)
                sprintf(response + strlen(response), "%d ", arr[i]);
        } 
        else if (choice == 3) { // Sort descending
            sortArray(arr, n, 0);
            sprintf(response, "Sorted Descending: ");
            for (int i = 0; i < n; i++)
                sprintf(response + strlen(response), "%d ", arr[i]);
        } 
        else if (choice == 4) { // Split odd/even
            sprintf(response, "Even: ");
            for (int i = 0; i < n; i++)
                if (arr[i] % 2 == 0)
                    sprintf(response + strlen(response), "%d ", arr[i]);
            strcat(response, " | Odd: ");
            for (int i = 0; i < n; i++)
                if (arr[i] % 2 != 0)
                    sprintf(response + strlen(response), "%d ", arr[i]);
        } 
        else if (choice == 5) { // Exit
            printf("Client exited.\n");
            break;
        } 
        else {
            sprintf(response, "Invalid choice!");
        }

        write(new_socket, response, strlen(response)+1);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
