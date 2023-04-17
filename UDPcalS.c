#include <stdio.h>      
#include <sys/socket.h> 
#include <arpa/inet.h>  
#include <stdlib.h>     
#include <string.h>     
#include <unistd.h> 

#define BUF_SIZE 1024

int calculate(char *operation) {
    // Perform the arithmetic operation based on the string received
    int result = 0;
    sscanf(operation, "%d", &result);
    char *token;
    token = strtok(operation, "+-*/");
    while(token != NULL) {
        int operand;
        sscanf(token, "%d", &operand);
        if(strchr("+", *operation) != NULL) {
            result += operand;
        }
        else if(strchr("-", *operation) != NULL) {
            result -= operand;
        }
        else if(strchr("*", *operation) != NULL) {
            result *= operand;
        }
        else if(strchr("/", *operation) != NULL) {
            result /= operand;
        }
        token = strtok(NULL, "+-*/");
    }
    return result;
}

int main() {
    struct sockaddr_in server_addr, client_addr;
    int server_sock, client_sock, addr_len, recv_len;
    char buf[BUF_SIZE];

    // Create a UDP socket
    if((server_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set server address and port
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(10000);

    // Bind socket to address and port
    if(bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    addr_len = sizeof(client_addr);

    while(1) {
        // Receive operation from client
        if((recv_len = recvfrom(server_sock, buf, BUF_SIZE, 0, (struct sockaddr *) &client_addr, &addr_len)) < 0) {
            perror("Error receiving data");
            exit(EXIT_FAILURE);
        }

        // Calculate result
        buf[recv_len] = '\0';
        int result = calculate(buf);

        // Send result to client
        char result_str[BUF_SIZE];
        sprintf(result_str, "%d", result);
        if(sendto(server_sock, result_str, strlen(result_str), 0, (struct sockaddr *) &client_addr, addr_len) < 0) {
            perror("Error sending data");
            exit(EXIT_FAILURE);
        }
    }

    close(server_sock);
    return 0;
}

