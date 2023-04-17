#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_LENGTH 1024

void DieWithError(char *errorMessage);

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in serv_addr, cli_addr;
    int cli_len = sizeof(cli_addr);
    char buffer[MAX_LENGTH];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        DieWithError("socket() failed");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        DieWithError("bind() failed");

    int sum = 0;

    while (1) {
        printf("Handling client %s:%d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
        memset(buffer, 0, MAX_LENGTH);
        int recv_len = recvfrom(sockfd, buffer, MAX_LENGTH, 0, (struct sockaddr *) &cli_addr, &cli_len);
        if (recv_len < 0) DieWithError("recvfrom() failed");

        int num = atoi(buffer);
        sum += num;

        memset(buffer, 0, MAX_LENGTH);
        sprintf(buffer, "%d", sum);
        int send_len = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *) &cli_addr, cli_len);
        if (send_len != strlen(buffer)) DieWithError("sendto() sent a different number of bytes than expected");
    }


    close(sockfd);
    return 0;
}

void DieWithError(char *errorMessage) {
    perror(errorMessage);
    exit(1);
}
