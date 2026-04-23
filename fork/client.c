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
#include<arpa/inet.h>
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
   while(1){
     int choice;
    printf("1.Add/Subtract two integer\n 2. Find the value of ’x’ in a linear equation\n3. Multiply two matrice\n 4.exit\n enter choice: ");
    scanf("%d",&choice);
    send(sockfd,&choice,sizeof(choice),0);

    if(choice==1){
        int a,b;
        printf("enter a: ");
        scanf("%d",&a);
        printf("enter b: ");
        scanf("%d",&b);
       send(sockfd,&a,sizeof(a),0);
       send(sockfd,&b,sizeof(b),0);
       int n=recv(sockfd,buffer,sizeof(buffer)-1,0);
       buffer[n]='\0';
       printf("%s",buffer);
    }
    else if (choice==2)
    {
        float a,b;
        printf("enter a: ");
        scanf("%f",&a);
        printf("enter b: ");
        scanf("%f",&b);
       send(sockfd,&a,sizeof(a),0);
       send(sockfd,&b,sizeof(b),0);
       int n=recv(sockfd,buffer,sizeof(buffer)-1,0);
       buffer[n]='\0';
       printf("%s",buffer);
    }
    else if (choice==3)
    {
        int r1,c1,r2,c2;
        int A[10][10];
        int B[10][10];
        int C[10][10];
         printf("enter r1,c1: ");
        scanf("%d %d",&r1,&c1);
         printf("enter r2,c2: ");
        scanf("%d %d",&r2,&c2);
         send(sockfd,&r1,sizeof(r1),0);
       send(sockfd,&c1,sizeof(c1),0);
         send(sockfd,&r2,sizeof(r2),0);
       send(sockfd,&c2,sizeof(c2),0);
       printf("enter A: ");
       for(int i=0;i<r1;i++)
                for(int j=0;j<c1;j++)
                    scanf("%d",&A[i][j]);

                    printf("Enter matrix B:\n");
            for(int i=0;i<r2;i++)
                for(int j=0;j<c2;j++)
                    scanf("%d",&B[i][j]);


                    send(sockfd, A, sizeof(A), 0);
            send(sockfd, B, sizeof(B), 0);

            recv(sockfd, C, sizeof(C), 0);
            printf("Result:\n");
            for(int i=0;i<r1;i++){
                for(int j=0;j<c2;j++)
                    printf("%d ", C[i][j]);
                printf("\n");
            }
    }
    else if (choice==4)
    {
        break;
    }
    }
    
    close(sockfd);
}