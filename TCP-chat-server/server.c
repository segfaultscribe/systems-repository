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
    int server_sock, client_sock;
    struct sockaddr_in server, client;
    socklen_t client_size = sizeof(client);

    if((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("Socket failed");
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if(bind(server_sock, (struct sockaddr *)&server, sizeof(server)) < 0){
        perror("[$] BIND FAILED!");
        return -1;
    }

    if(listen(server_sock, MAX_CLIENTS) < 0){
        perror("[$] LISTEN FAILED!");
        return -1;
    }

    printf("Server is listening on port %d...\n", PORT);

    while(1){
        client_sock = accept(server_sock, (struct sockaddr *)&client, &client_size);
        if(client_sock < 0){
            perror("[$] ACCEPT FAILED!");
            continue;
        }

        printf("Clinet connected\n");

        client_data *data = malloc(sizeof(client_data));
        data->client_socket = client_sock;
        data->client_address = client;

        pthread_t thread;
        if(pthread_create(&thread, NULL, handle_client, (void *)data != 0)){
            perror("Thread creation failed!");
        }
    }

    close(server_sock)
    return 0;

}