#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8000 

int main(){
    //first we create the socket
    int sock;
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("Socket failed!");
        return -1;
    }

    //now we prepare the server address
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT)

    if(inet_pton(AF_INET, SERVER_IP, &server.sin_addr)<=0 ){
        perror("Invalid address");
        return 1;
    }

}