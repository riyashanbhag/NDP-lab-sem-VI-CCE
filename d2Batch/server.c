// server.c
// TCP Server: Encrypt + Sort

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

// Function to sort integer array
void sort(int arr[], int n) {
    int temp;
    for(int i = 0; i < n-1; i++) {
        for(int j = i+1; j < n; j++) {
            if(arr[i] > arr[j]) {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

int main() {

    int server_fd, connfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len;

    char buffer[MAX];

    // 1️⃣ Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 2️⃣ Setup address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // 3️⃣ Bind
    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // 4️⃣ Listen
    listen(server_fd, 5);

    printf("Server Running...\n");

    while(1) {

        len = sizeof(client_addr);

        // 5️⃣ Accept client
        connfd = accept(server_fd,
                        (struct sockaddr*)&client_addr,
                        &len);

        printf("Client Connected\n");

        // 6️⃣ Receive input string
        recv(connfd, buffer, sizeof(buffer), 0);

        printf("Received: %s\n", buffer);

        int n = strlen(buffer);
        int encrypted[MAX];

        // 7️⃣ Encrypt each character
        for(int i = 0; i < n; i++) {
            int ascii = (int)buffer[i];
            encrypted[i] = ascii * ascii + 1;
        }

        // 8️⃣ Sort encrypted values
        sort(encrypted, n);

        // 9️⃣ Convert result into string to send back
        char result[MAX] = "";
        char temp[50];

        for(int i = 0; i < n; i++) {
            sprintf(temp, "%d ", encrypted[i]);
            strcat(result, temp);
        }

        // 🔟 Send result
        send(connfd, result, strlen(result)+1, 0);

        close(connfd);
    }

    close(server_fd);
    return 0;
}
