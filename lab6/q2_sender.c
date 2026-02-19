/*
------------------------------------------------------------
Write a C program to transmit a set of characters from sender to receiver
using socket programming.

At sender:
- Generate CRC using given polynomial (CRC-12 / CRC-16 / CRC-CCITT)
- Append CRC to data
- Send to receiver

At receiver:
- Perform CRC division
- If remainder is zero → Data not corrupted
- Else → Data corrupted
------------------------------------------------------------
CRC SENDER (Client)
------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

// Function to perform CRC division
void crc(char data[], char generator[]) {

    int data_len = strlen(data);
    int gen_len = strlen(generator);

    for(int i = 0; i <= data_len - gen_len; i++) {

        if(data[i] == '1') {
            for(int j = 0; j < gen_len; j++) {
                data[i+j] =
                (data[i+j] == generator[j]) ? '0' : '1';
            }
        }
    }
}

int main() {

    int sock;
    struct sockaddr_in addr;

    char message[100];
    char data[MAX], generator[20];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr*)&addr,
            sizeof(addr));

    printf("Enter message: ");
    scanf("%s", message);

    printf("Select CRC Type:\n");
    printf("1. CRC-12\n2. CRC-16\n3. CRC-CCITT\n");
    int choice;
    scanf("%d", &choice);

    if(choice == 1)
        strcpy(generator,"1100000001111");
    else if(choice == 2)
        strcpy(generator,"11000000000000101");
    else
        strcpy(generator,"10001000000100001");

    // Convert message to binary (ASCII)
    data[0] = '\0';
    for(int i=0;i<strlen(message);i++){
        char bin[10];
        for(int j=7;j>=0;j--)
            bin[7-j] =
            ((message[i]>>j)&1)+'0';
        bin[8]='\0';
        strcat(data,bin);
    }

    int gen_len = strlen(generator);

    // Append zeros
    for(int i=0;i<gen_len-1;i++)
        strcat(data,"0");

    char temp[MAX];
    strcpy(temp,data);

    crc(temp,generator);

    // Append remainder
    int data_len=strlen(data);
    for(int i=0;i<gen_len-1;i++)
        data[data_len-(gen_len-1)+i]=
        temp[data_len-(gen_len-1)+i];

    printf("Data sent: %s\n", data);

    send(sock,data,strlen(data)+1,0);

    close(sock);
    return 0;
}
