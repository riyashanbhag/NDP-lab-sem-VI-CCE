/*
------------------------------------------------------------
CRC RECEIVER (Server)
------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

void crc(char data[], char generator[]) {

    int data_len = strlen(data);
    int gen_len = strlen(generator);

    for(int i=0;i<=data_len-gen_len;i++) {

        if(data[i]=='1') {
            for(int j=0;j<gen_len;j++)
                data[i+j]=
                (data[i+j]==generator[j])?'0':'1';
        }
    }
}

int main() {

    int server_fd, connfd;
    struct sockaddr_in addr;
    char data[MAX];
    char generator[20];

    server_fd=socket(AF_INET,SOCK_STREAM,0);

    addr.sin_family=AF_INET;
    addr.sin_port=htons(PORT);
    addr.sin_addr.s_addr=INADDR_ANY;

    bind(server_fd,(struct sockaddr*)&addr,
         sizeof(addr));

    listen(server_fd,5);

    printf("CRC Receiver Waiting...\n");

    connfd=accept(server_fd,NULL,NULL);

    recv(connfd,data,sizeof(data),0);

    printf("Received Data: %s\n",data);

    printf("Select CRC Type used by Sender:\n");
    printf("1. CRC-12\n2. CRC-16\n3. CRC-CCITT\n");

    int choice;
    scanf("%d",&choice);

    if(choice==1)
        strcpy(generator,"1100000001111");
    else if(choice==2)
        strcpy(generator,"11000000000000101");
    else
        strcpy(generator,"10001000000100001");

    char temp[MAX];
    strcpy(temp,data);

    crc(temp,generator);

    int flag=0;
    for(int i=0;i<strlen(generator)-1;i++){
        if(temp[strlen(temp)-(strlen(generator)-1)+i]=='1'){
            flag=1;
            break;
        }
    }

    if(flag==0)
        printf("Data is NOT Corrupted\n");
    else
        printf("Data is Corrupted\n");

    close(connfd);
    close(server_fd);
    return 0;
}
