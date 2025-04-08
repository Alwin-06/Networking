#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define PACKETS 5
#define WINDOW_SIZE 4

int main()
{
        struct sockaddr_in serv;
        memset(&serv,0,sizeof(serv));
        serv.sin_family=AF_INET;
        serv.sin_addr.s_addr=inet_addr("127.0.0.1");
        serv.sin_port=htons(9991);

        int sockfd = socket(AF_INET,SOCK_STREAM,0);
        connect(sockfd,(struct sockaddr*)&serv,sizeof(serv));

        int base=0,seq,ack,i;
        while(base < PACKETS)
        {
                for(i=0; i < WINDOW_SIZE && (base + i) < PACKETS; i++)
                {
                        seq = base + i;
                        send(sockfd, &seq, sizeof(seq), 0);
                        printf("Sent Packet with Sequence Number %d\n", seq);
                }
                if(recv(sockfd, &ack, sizeof(ack), 0) > 0)
                {
                        printf("Received Acknowledgement: %d\n", ack);
                        base = ack;
                }
                else
                {
                        printf("Timeout!!!\n");
                }
        }
        close(sockfd);
        return 0;
}
