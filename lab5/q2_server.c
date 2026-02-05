#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080

int areAnagrams(char s1[], char s2[]) {
    int count[256] = {0};

    if (strlen(s1) != strlen(s2))
        return 0;

    for (int i = 0; s1[i]; i++) {
        count[(unsigned char)s1[i]]++;
        count[(unsigned char)s2[i]]--;
    }

    for (int i = 0; i < 256; i++)
        if (count[i] != 0)
            return 0;

    return 1;
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len = sizeof(client_addr);

    char buffer[200];
    char str1[100], str2[100];
    char result[100];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    printf("Iterative Server running...\n");

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &len);

        // Receive combined string
        recv(client_fd, buffer, sizeof(buffer), 0);

        // Split strings using delimiter
        sscanf(buffer, "%[^|]|%s", str1, str2);

        if (areAnagrams(str1, str2))
            strcpy(result, "Strings are Anagrams");
        else
            strcpy(result, "Strings are NOT Anagrams");

        send(client_fd, result, strlen(result) + 1, 0);

        // Display client info + date & time
        time_t now;
        time(&now);

        printf("\nClient connected: %s:%d",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));
        printf("\nDate & Time: %s", ctime(&now));

        close(client_fd);
    }

    return 0;
}
