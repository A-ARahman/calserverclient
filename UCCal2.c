#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RCVBUFSIZE 32

void DieWithError(char *errorMessage);

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in echoServAddr;
    unsigned short echoServPort;
    char *servIP;
    char echoBuffer[RCVBUFSIZE];
    unsigned int echoStringLen;
    int respStringLen;

    if ((argc < 3) || (argc > 4))
    {
        fprintf(stderr, "Usage: %s <Server IP> <Echo Port> [<Echo Word>]\n",
                argv[0]);
        exit(1);
    }

    servIP = argv[1];
    echoServPort = atoi(argv[2]);

    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");

    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);
    echoServAddr.sin_port = htons(echoServPort);

    printf("Masukkan Angka: ");
    scanf("%s", echoBuffer);
    echoStringLen = strlen(echoBuffer);

    if (sendto(sock, echoBuffer, echoStringLen, 0, (struct sockaddr *)
               &echoServAddr, sizeof(echoServAddr)) != echoStringLen)
        DieWithError("sendto() sent a different number of bytes than expected");

    if ((respStringLen = recvfrom(sock, echoBuffer, RCVBUFSIZE, 0,
                                  NULL, 0)) < 0)
        DieWithError("recvfrom() failed");

    printf("Server response (Hasil): %s\n", echoBuffer);

    close(sock);
    exit(0);
}

void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}
