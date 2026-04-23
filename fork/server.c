#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#define MAXSIZE 90
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

	retval=listen(sockfd,1);
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
    while (1)
    {
        int choice;
        recv(clientfd,&choice,sizeof(choice),0);
        if(choice==1){
            int a,b;
             recv(clientfd,&a,sizeof(a),0);
             recv(clientfd,&b,sizeof(b),0);
             int sum=a+b;
             int dif=a-b;
             char buffer[100];
             sprintf(buffer,"sum:%d dif:%d",sum,dif);
             send(clientfd,buffer,strlen(buffer),0);

        }else if (choice==2)
        {
            float a,b,x;
            recv(clientfd,&a,sizeof(a),0);
             recv(clientfd,&b,sizeof(b),0);
            if(a!=0){
                x=-b/a;
            }else
            {
                x=0;
            }
            char buffer[100];
             sprintf(buffer,"x:%f ",x);
             send(clientfd,buffer,strlen(buffer),0);

        }
        else if (choice==3)
        {
            int r1,r2,c1,c2;
             recv(clientfd,&r1,sizeof(r1),0);
       recv(clientfd,&c1,sizeof(c1),0);
         recv(clientfd,&r2,sizeof(r2),0);
       recv(clientfd,&c2,sizeof(c2),0);
       int A[10][10];
       int B[10][10];
       int C[10][10];

       recv(clientfd,A,sizeof(A),0);
         recv(clientfd,B,sizeof(B),0);
         for(int i=0;i<r1;i++){
            for(int j=0;j<c2;j++){
                C[i][j]=0;
                for(int k=0;k<c1;k++){
                    C[i][j]+=A[i][k]*B[k][j];
                }
            }
         }
         send(clientfd,C,sizeof(C),0);
        }
        else if (choice==4)
        {
            break;
        }
    }
close(sockfd);
close(clientfd);
}