#include <stdio.h>

#define PORT 8000
#define MAX_CLIENTS 10 
#include <sys/socket.h>

/*
struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET
    unsigned short   sin_port;     // e.g. htons(3490)
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};
*/
typedef struct {
    int client_socket;
    struct sockaddr_in client_address;
} client_data;

void *handle_client(void *arg){
    client_data *data = (client_data *) arg;

    int sock = data->client_socket;
    char buffer[1024];

    while(1) {
        mmset(buffer, 0, sizeof(buffer));
        int read_size = recv(sock, buffer, sizeof(buffer), 0);

        if(read_size == 0){
            printf("[#] Client disconnected\n");
            break;
        }

        if(read_size == -1){
            perror("[$] recv FAILED");
            break;
        }

        printf("Received: %s\n", buffer);

        send(sock, buffer, strlen(buffer), 0);
    }

    close(sock);
    free(data);
    return NULL;
}







int main(){

    return 0;

}