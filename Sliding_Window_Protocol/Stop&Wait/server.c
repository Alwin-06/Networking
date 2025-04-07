#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#define MSG_LEN 5
#define TIME 2

int main()
{
        int ack,seq;
        struct sockaddr_in serv;
        struct sockaddr_storage store;
        socklen_t addrsize = sizeof(store);
        struct timeval timeout={TIME,0};

        serv.sin_family=AF_INET;
        serv.sin_addr.s_addr=INADDR_ANY;
        serv.sin_port=htons(5100);

        int sockfd = socket(AF_INET,SOCK_STREAM,0);
        bind(sockfd,(struct sockaddr*)&serv,sizeof(serv));
        listen(sockfd,5);
        int newsockfd=accept(sockfd,(struct sockaddr*)&store,&addrsize);

        setsockopt(newsockfd,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(timeout));
        ack=0;

        while(ack < MSG_LEN)
        {
                if(read(newsockfd,&seq,sizeof(seq))>0)
                {
                        if(seq == ack)
                        {
                                printf("Recevied Packet with Sequence Number: %d\n",seq);
                                ack = ack+1;
                                write(newsockfd,&ack,sizeof(ack));
                                printf("Sent Acknowlegement: %d\n",ack);
                        }
                }
                else
                {
                        printf("TimeOut !!! Error in Reciving\n");
                }
        }
        close(newsockfd);
        close(sockfd);
}
