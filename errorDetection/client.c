//cleint-> sender
#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#define MAXSIZE 50
void xor(char *data, char *divisior, char *codeword){
    
}
int main(){
    char buff[MAXSIZE];
	int sockfd,retval,i;
	int recedbytes,sentbytes;
	struct sockaddr_in serveraddr;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd==-1)
	{
		printf("\nSocket Creation Error");
		return;
	}

	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(8080);
	serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	retval=connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if(retval==-1)
	{
		printf("Connection error");
		return;
	}
    char data[100];
    char codeword[100];
    char buffer[100];
    printf("enter data");
    scanf("%s",data);
    char division[]="11000000000000101";
    xor(data,divisor,codeword);
    send(sockfd,codeword,strlen(codeword),0);
    int n=recv(sockfd,buffer,sizeof(buffer)-1,0);
    buffer[n]='\0';
    printf("%s",buffer);
    close(sockfd);

}