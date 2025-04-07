#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#define MSG_LEN 5

int main()
{
        int ack,seq;
        struct sockaddr_in serv;
        serv.sin_family=AF_INET;
        serv.sin_addr.s_addr=inet_addr("127.0.0.1");
        serv.sin_port=htons(5100);

        int sockfd = socket(AF_INET,SOCK_STREAM,0);
        connect(sockfd,(struct sockaddr*)&serv,sizeof(serv));

        seq=0;
        while(seq < MSG_LEN)
        {
                write(sockfd,&seq,sizeof(seq));
                printf("Sending Packet with Sequence Number %d\n",seq);
                if(read(sockfd,&ack,sizeof(ack))>0)
                {
                        printf("Received Acknowledgement: %d\n",ack);
                        seq = ack;
                }
                else
                {
                        printf("Timeout!!! Error in Receiving Packet\n");
                }
        }
        close(sockfd);
        return 0;
}
