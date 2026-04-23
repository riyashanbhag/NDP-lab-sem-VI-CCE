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
#define PORT 8080
struct Product
{
    int id;
    char name[50];
    int q;
    int price;

};
int main(){
    
	char buffer[1024];
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
        printf("1.Insert\n2.delete\n3.display\n4.update quantity\n5.search by price\n6.exit\n enter choice: ");
        scanf("%d",&choice);
        send(sockfd,&choice,sizeof(choice),0);
        if(choice==1){
            struct Product p;
            //id name q price
            printf("enter id: ");
            scanf("%d",&p.id);
            getchar();
            printf("enter name: ");
            fgets(p.name,sizeof(p.name),stdin);
            printf("enter q: ");
            scanf("%d",&p.q);
            getchar();
            printf("enter price: ");
            scanf("%d",&p.price);
            getchar();

            send(sockfd,&p,sizeof(p),0);
            int n=recv(sockfd,buffer,sizeof(buffer)-1,0);
            buffer[n]='\0';
            printf("%s",buffer);
        }
        //insert, del,disp, update, search
        else if (choice==2)
        {
           int id;
           printf("enter id:");
           scanf("%d",&id);
           send(sockfd,&id,sizeof(id),0);
           int n=recv(sockfd,buffer,sizeof(buffer)-1,0);
           buffer[n]='\0';
           printf("%s",buffer); 
        }
        else if (choice==3)
        {
             int n=recv(sockfd,buffer,sizeof(buffer)-1,0);
           buffer[n]='\0';
           printf("%s",buffer); 
        }
        else if(choice==4){
            int id,q;
           printf("enter id:");
           scanf("%d",&id);
            printf("enter qua:");
           scanf("%d",&q);
           send(sockfd,&id,sizeof(id),0);
            send(sockfd,&q,sizeof(q),0);
           int n=recv(sockfd,buffer,sizeof(buffer)-1,0);
           buffer[n]='\0';
           printf("%s",buffer); 
        }
        else if (choice==5)
        {
            int min,max;
             printf("enter min price: ");
           scanf("%d",&min);
            printf("enter maxprice: ");
           scanf("%d",&max);
           send(sockfd,&min,sizeof(min),0);
            send(sockfd,&max,sizeof(max),0);
           int n=recv(sockfd,buffer,sizeof(buffer)-1,0);
           buffer[n]='\0';
           printf("%s",buffer); 
        }
        else if (choice==6)
        {
            break;
        }
    }
    close(sockfd);
}