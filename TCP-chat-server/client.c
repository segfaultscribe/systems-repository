#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8000 
#define BUFFER_SIZE 1024

int main(){
    //first we create the socket
    int sock;
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Socket failed!");
        return -1;
    }
    //make a buffer to hold messages
    char buffer[BUFFER_SIZE];
    //now we prepare the server address
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    // inet_pton is used to convert a string - server IP - to binary in_addr
    if(inet_pton(AF_INET, SERVER_IP, &server.sin_addr)<=0 ){
        perror("Invalid address");
        return 1;
    }
    //connect to the server
    if(connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0){
        perror("Connection failed!");
        return 1;
    }
    //opening comms
    while(1){
        printf("Enter a message to send: ");
        fgets(buffer, sizeof(buffer), stdin);
        if(send(sock, buffer, strlen(buffer), 0) < 0){
            perror("Send failed!");
            break;
        }

        int bytes_received = recv(sock, buffer, sizeof(buffer), 0);
        if(bytes_received <= 0){
            printf("Server disconnected or error!\n");
            break;
        }

        buffer[bytes_received] = '\0';
        printf("Server echoed: %s\n", buffer);
    }

    close(sock);
    return 0;
}