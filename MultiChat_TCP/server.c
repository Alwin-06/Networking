#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

void* handle_client(void* arg) {
    int client_sock = *(int*)arg;
    char buffer[1024];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = read(client_sock, buffer, sizeof(buffer));
        if (bytes <= 0 || strncmp(buffer, "exit", 4) == 0) {
            printf("Client disconnected.\n");
            break;
        }

        printf("Client says: %s", buffer);

        memset(buffer,0,sizeof(buffer));
        fgets(buffer,sizeof(buffer),stdin);
        strcat(buffer, " (ack from server)");
        write(client_sock, buffer, strlen(buffer));
    }

    close(client_sock);
    free(arg);
    pthread_exit(NULL);
}

int main() {
    int sockfd, *newsockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t cli_len;
    pthread_t tid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9006);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(sockfd, 5);

    printf("Server listening on port 9006...\n");

    while (1) {
        cli_len = sizeof(cliaddr);
        newsockfd = malloc(sizeof(int));
         *newsockfd = accept(sockfd, (struct sockaddr*)&cliaddr, &cli_len);

        printf("New client connected.\n");

        pthread_create(&tid, NULL, handle_client, (void*)newsockfd);
        pthread_detach(tid);
    }

    close(sockfd);
    return 0;
}
