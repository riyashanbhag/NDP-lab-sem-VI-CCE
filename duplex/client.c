#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#define MAXSIZE 50
int main(){
    char buffer[MAXSIZE];
	int sockfd,retval,i;
	int recedbytes,sentbytes;
	struct sockaddr_in serveraddr;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd==-1)
	{
		printf("\nSocket Creation Error");
		return 0;
	}

	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(8080);
	serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	retval=connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if(retval==-1)
	{
		printf("Connection error");
		return 0;
	}
if(fork()==0){
        while (1)
        {
            int n = recv(sockfd, buffer, sizeof(buffer)-1, 0);
            buffer[n] = '\0';
            printf("\nServer: %s\n", buffer);
        }
    }else
    {
        while (1)
        {
            fgets(buffer,sizeof(buffer),stdin);
            send(sockfd,buffer,strlen(buffer),0);
        }
    }

    close(sockfd);
}