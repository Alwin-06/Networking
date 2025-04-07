#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

void receivefile(int sockfd)
{
        char buffer[4098];
        int bytesread=0,i,j;
        char temp;
        FILE *fp;
        fp = fopen("output.txt","w");
        if(fp == NULL)
        {
                printf("Error in Opening File\n");
        }
        else
        {
                bytesread = recv(sockfd,buffer,sizeof(buffer),0);
                buffer[strlen(buffer)-1]='\0';
                printf("File Received Successfully...\n");
                i=0;j=strlen(buffer)-1;
                while(i<j)
                {
                        temp = buffer[i];
                        buffer[i]=buffer[j];
                        buffer[j]=temp;
                        i++;j--;
                }
                printf("Reversed File Contents\n");
                printf("%s\n",buffer);
                fclose(fp);
        }
}

int main()
{
        struct sockaddr_in serv,cli;
        memset(&serv,0,sizeof(serv));
        serv.sin_family=AF_INET;
        serv.sin_addr.s_addr=INADDR_ANY;
        serv.sin_port=htons(5555);
        socklen_t cli_len=sizeof(cli);

        int sockfd = socket(AF_INET,SOCK_STREAM,0);
        bind(sockfd,(struct sockaddr*)&serv,sizeof(serv));
        listen(sockfd,5);
        int newsockfd = accept(sockfd,(struct sockaddr*)&cli,&cli_len);

        receivefile(newsockfd);
        close(newsockfd);
        close(sockfd);
        return 0;
}
