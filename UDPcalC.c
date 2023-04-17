#include <stdio.h>      
#include <sys/socket.h> 
#include <arpa/inet.h>  
#include <stdlib.h>     
#include <string.h>     
#include <unistd.h> 
#define BUF_SIZE 1024

int main() {
    struct sockaddr_in server_addr;
    int client_sock, addr_len, send_len, recv_len;
    char buf[BUF_SIZE], result_buf[BUF_SIZE];

    // Create a UDP socket
    if((client_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set server address and port
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Change to server IP address
    server_addr.sin_port = htons(10000); // Change to server port

    while(1) {
        // Get arithmetic operation from user
        printf("Enter arithmetic operation (+, -, *, /): ");
        fgets(buf, BUF_SIZE, stdin);

        // Send operation to server
        if((send_len = sendto(client_sock, buf, strlen(buf), 0, (struct sockaddr *) &server_addr, sizeof(server_addr))) < 0) {
            perror("Error sending data");
            exit(EXIT_FAILURE);
        }

        // Receive result from server
        if((recv_len = recvfrom(client_sock, result_buf, BUF_SIZE, 0, NULL, NULL)) < 0) {
            perror("Error receiving data");
            exit(EXIT_FAILURE);
        }

        // Print result
        result_buf[recv_len] = '\0';
        printf("Result: %s\n", result_buf);
    }

    close(client_sock);
    return 0;
}

