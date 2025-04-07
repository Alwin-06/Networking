#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

void sendfile(int sockfd)
{
        char buffer[4098],filename[128];
        FILE *fp;
        printf("Enter the filename: ");
        scanf("%s", filename);
        fp=fopen(filename,"r");
        int bytesread=0;
        if(fp == NULL)
        {
                printf("Error in Opening File\n");
        }
        else
        {
                while((bytesread = fread(buffer,sizeof(char),sizeof(buffer),fp))>0)
                {
                        send(sockfd,buffer,strlen(buffer),0);
                }
                printf(" %s Sent Successfully...\n",filename);
                fclose(fp);
        }
}

int main()
{
        struct sockaddr_in serv;
        memset(&serv,0,sizeof(serv));
        serv.sin_family=AF_INET;
        serv.sin_addr.s_addr=inet_addr("127.0.0.1");
        serv.sin_port=htons(5555);

        int sockfd = socket(AF_INET,SOCK_STREAM,0);
        connect(sockfd,(struct sockaddr*)&serv,sizeof(serv));

        sendfile(sockfd);
        close(sockfd);
        return 0;
}
