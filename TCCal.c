#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define RCVBUFSIZE 32

void DieWithError(char *errorMessage);

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in servAddr;
    char echoString[RCVBUFSIZE];
    char echoBuffer[RCVBUFSIZE];
    unsigned short servPort;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <Server IP> <Server Port>\n", argv[0]);
        exit(1);
    }

    servPort = atoi(argv[2]);

    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(servPort);

    if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        DieWithError("connect() failed");

    printf("Masukkan operasi (e.g. 2 + 3): ");
    fgets(echoString, RCVBUFSIZE, stdin);

    if (send(sock, echoString, strlen(echoString), 0) != strlen(echoString))
        DieWithError("send() sent a different number of bytes than expected");

    if (recv(sock, echoBuffer, RCVBUFSIZE, 0) < 0)
        DieWithError("recv() failed");

    printf("Hasil: %s\n", echoBuffer);

    close(sock);
    exit(0);
}

void DieWithError(char *errorMessage) {
    perror(errorMessage);
    exit(1);
}

