#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define MAXSIZE 90
int main(){

int sockfd,clientfd,retval,i;
	socklen_t actuallen;
	int recedbytes,sentbytes;
	struct sockaddr_in serveraddr,clientaddr;
	char buffer[MAXSIZE];
	
	int a=0;
	sockfd=socket(AF_INET,SOCK_STREAM,0);

	if(sockfd==-1)
	{
	printf("\nSocket creation error");
	}

	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(8080);
	serveraddr.sin_addr.s_addr=htons(INADDR_ANY);
	retval=bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if(retval==1)
	{
		printf("Binding error");
		close(sockfd);
	}

	retval=listen(sockfd,1);
    printf("\nlistening...\n");

	if(retval==-1)
	{
	close(sockfd);
	}
	actuallen=sizeof(clientaddr);
	clientfd=accept(sockfd,(struct sockaddr*)&clientaddr,&actuallen);
	if(clientfd==-1)
	{
		close(sockfd);
	}
    if(fork()==0){
        while (1)
        {
            int n = recv(clientfd, buffer, sizeof(buffer)-1, 0);
            buffer[n] = '\0';
            printf("\nClient: %s\n", buffer);
        }
    }else
    {
        while (1)
        {
            fgets(buffer,sizeof(buffer),stdin);
            send(clientfd,buffer,strlen(buffer),0);
        }
    }
close(clientfd);
    close(sockfd);
}