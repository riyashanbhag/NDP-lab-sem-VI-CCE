#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
struct Product
{
    int id;
    char name[50];
    int q;
    int price;
};
void insert(struct Product p){
    FILE *fp= fopen("products.txt","ab");
    fwrite(&p,sizeof(p),1,fp);
    fclose(fp);
}
void delete(int clientfd,int id ){
    FILE *fp= fopen("products.txt","rb");
     FILE *temp= fopen("temp.txt","wb");
     struct Product p;
     int found=0;
     while(fread(&p,sizeof(p),1,fp)){
        if(p.id!=id){
            fwrite(&p,sizeof(p),1,temp);
        }else{
            found=1;
        }
     }
     fclose(fp);
     fclose(temp);
     remove("products.txt");
     rename("temp.txt","products.txt");
     if(found){
        send(clientfd,"deleted",7,0);
     }
     else
     {
       send(clientfd,"no",2,0); 
     }

}
void display(int clientfd){
    FILE *fp= fopen("products.txt","rb");
    struct Product p;
   if(!fp){
   send(clientfd,"no",2,0);
   return;
   }
   char buffer[2000]="";
   while(fread(&p,sizeof(p),1,fp)){
    char temp[200];
    sprintf(temp,"ID:%d\n name:%s\n q:%d\n price:%d\n",p.id,p.name,p.q,p.price);
    strcat(buffer,temp);
   }
   if(strlen(buffer)==0){
     strcpy(buffer, "No records\n");
   }
   send(clientfd,buffer,strlen(buffer),0);
   fclose(fp);
}
void update(int clientfd,int id,int q){
     FILE *fp= fopen("products.txt","rb");
     FILE *temp= fopen("temp.txt","wb");
     struct Product p;
     int found=0;
     while(fread(&p,sizeof(p),1,fp)){
        if(p.id==id){
            found=1;
            p.q=q;
        }
        fwrite(&p,sizeof(p),1,temp);
     }
     fclose(fp);
     fclose(temp);
     remove("products.txt");
     rename("temp.txt","products.txt");
     if(found){
        send(clientfd,"updated",7,0);
     }
     else
     {
       send(clientfd,"no",2,0); 
     }

}
void search(int clientfd,int min, int max){
     FILE *fp = fopen("products.txt", "rb");
    struct Product p;
    int found = 0;

    char buffer[2000] = "";

    while (fread(&p, sizeof(p), 1, fp)) {
        if (p.price >= min && p.price <= max) {
            char temp[200];
            sprintf(temp, "ID:%d Name:%sQty:%d Price:%.d\n",
                    p.id, p.name, p.q, p.price);
            strcat(buffer, temp);
            found = 1;
        }
    }

    if (!found)
        strcpy(buffer, "No products in range\n");

    send(clientfd, buffer, strlen(buffer), 0);
    fclose(fp);
}
int main(){

    int sockfd,clientfd,retval,i;
	socklen_t actuallen;
	int recedbytes,sentbytes;
	struct sockaddr_in serveraddr,clientaddr;
	char buffer[1024];
	
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
    //insert 2del, 3 disp, 4 updtae, 5 search , 6 exit
    while(1){
        int choice;
        recv(clientfd,&choice, sizeof(choice),0);
        if(choice==1){
            struct Product p;
            recv(clientfd,&p, sizeof(p),0);
            insert(p);
            send(clientfd,"inserted\n",9,0);

        }else if (choice==2)
        {
            int id;
            recv(clientfd,&id, sizeof(id),0);
            delete(clientfd,id);
            
           
        }
        else if (choice==3)
        {
           
           
            display(clientfd);
            
           
        }
        else if (choice==4)
        {
            int id,q;
            recv(clientfd,&id, sizeof(id),0);
            recv(clientfd,&q, sizeof(q),0);
            update(clientfd,id,q);
            
           
        }
        else if (choice==5)
        {
            int min,max;
            recv(clientfd,&min, sizeof(min),0);
            recv(clientfd,&max, sizeof(max),0);
            search(clientfd,min,max);
            
           
        }
        else if (choice==6)
        {
            break;
        }
    }
    close(clientfd);
    close(sockfd);
}