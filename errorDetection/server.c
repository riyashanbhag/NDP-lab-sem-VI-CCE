//server-> reciever
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#define MAXSIZE 90
void xor(char *data, char *division, char *remainder){
    char temp[100];
    strcpy(temp,data);
    for(int i=0;i<=strlen(data)-n;i++){
        if(temp[i]=='1'){
            for(int j=0;j<n;j++){
                temp[i+j]=(temp[i+j]==divisor[j])'0':'1';
            }
        }
    }
    strcpy(remainder, temp+strlen(data)-n+1);
}
int main(){

    int sockfd,clientfd,retval,i;
	socklen_t actuallen;
	int recedbytes,sentbytes;
	struct sockaddr_in serveraddr,clientaddr;
	char buff[MAXSIZE];
	
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

	retval=listen(sockfd,3);
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
	char data[1000];
	char remainder[1000];
    recv(clientfd,data,sizeof(data),0);
    char divisor[]="11000000000000101";
    xor(data,division ,remainder);
    int error=0;
    for(int i=0;i<strlen(remainder);i++){
        if(remainder[i]=='1') {
            error=1;
            break;
        }
    }
    if(error){
        send(clientfd,"error\n",6,0);
    }else
    {
        send(clientfd,"no error\n",9,0);
    }
    close(clientfd);
    close(sockfd);

}