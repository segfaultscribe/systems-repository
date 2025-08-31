#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8000
#define MAX_CLIENTS 10 
/*
refer to this for sockaddr_in struct internals
struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET
    unsigned short   sin_port;     // e.g. htons(3490)
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};
*/
/*
why do we need client_data struct?
we'll be passing this to a thread and we need it to uniquely identify
each client, but thread handle functions can only have one argument(void *arg)
so we'll create a struct and pass it to the thread.
the thread handle function - handle_client() will use this struct
*/
typedef struct {
    int client_socket;
    struct sockaddr_in client_address; //look above for the struct
} client_data;
// function to pass to the thread
void *handle_client(void *arg){
    /*
    we need to typecast the argument to client_data struct type.
    why not pass client_data argument instead of void?
    pthread posix specification requires you to pass 
    a void pointer function with one void pointer argument

    int pthread_create(pthread_t *thread,
                   const pthread_attr_t *attr,
                   void *(*start_routine)(void *),
                   void *arg);
    */
    client_data *data = (client_data *) arg;
    // get socket info from the agument struct
    int sock = data->client_socket;
    // a buffer to receive message from the client
    // anything the client sends is stored in this buffer
    char buffer[1024];

    while(1) {
        // infinite loop cause... server lol

        // clear the buffer, set each byte to 0 before each read
        mmset(buffer, 0, sizeof(buffer));
        // recv is used to receive data from the socket
        // prototype :
        // ssize_t recv(int sockfd, void *buf, size_t len, int flags);
        int read_size = recv(sock, buffer, sizeof(buffer), 0);
        //error handling
        if(read_size == 0){
            printf("[#] Client disconnected\n");
            break;
        }
        if(read_size == -1){
            perror("[$] recv FAILED");
            break;
        }
        // once the message is received we'll echo it back to the client
        // so whatever the client sends it'll get the same message back
        printf("Received: %s\n", buffer);
        // send is similar to recv()
        // prototype :
        // ssize_t send(int sockfd, const void *buf, size_t len, int flags);
        send(sock, buffer, strlen(buffer), 0);
    }
    //close the socket and free the struct memory
    close(sock);
    free(data);
    return NULL;
}

int main(){
    // server_sock - listening socket
    // clinet_sock - client socket :)
    int server_sock, client_sock;
    // sockaddr_in - socket address struct
    struct sockaddr_in server, client;
    socklen_t client_size = sizeof(client);
    /* 
    socket() function creates a socket descriptor 
    which is like a file descriptor but for network communication
    prototype : int socket(int domain, int type, int protocol);

    AF_INET = IPv4
    SOCK_STREAM = TCP communication
    the last 0 - protocol value, 0 means: "Choose the default protocol for the given domain/type combination.
    This parameter lets you specify the underlying transport protocol explicitly
    For AF_INET + SOCK_STREAM, the kernel will default to: IPPROTO_TCP (which is defined as 6)
    for 99% real world cases 0 is enough
    so we're creating a socket that behaves like a TCP stream over IPv4.
    */
    if((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("Socket failed");
        return -1;
    }
    // server is sockaddr_in instance
    // INADDR_ANY - allows conn.s from any network interface
    // htons - converts port order in host byte order to netwrok byte order
    // network byte order is mostly big endian for TCP
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    // bind associates the socet with specific IP and port number
    // notice how we type cast sockaddr_in server to sockaddr
    // this is safe and idiomatic because sockaddr_in is specifically designed to be used like this 
    if(bind(server_sock, (struct sockaddr *)&server, sizeof(server)) < 0){
        perror("[$] BIND FAILED!");
        return -1;
    }
    // listen will let the socket accpet connections
    if(listen(server_sock, MAX_CLIENTS) < 0){
        perror("[$] LISTEN FAILED!");
        return -1;
    }
    printf("Server is listening on port %d...\n", PORT);
    while(1){
        // accept blocks until a client connects and then returns a new socket descriptor
        // this also handles the handshake for the connection
        // - checks incoming connections
        // - handles handshake
        // - creates a new socket
        // - fills client info in client struct
        client_sock = accept(server_sock, (struct sockaddr *)&client, &client_size);
        if(client_sock < 0){
            perror("[$] ACCEPT FAILED!");
            continue;
        }
        printf("Clinet connected\n");
        //allocate memory for client data and create the struct for the new client
        client_data *data = malloc(sizeof(client_data));
        data->client_socket = client_sock;
        data->client_address = client;
        //initialize a new thread
        pthread_t thread;
        // create a thread for each client and call the handle_client function
        if(pthread_create(&thread, NULL, handle_client, (void *)data != 0)){
            perror("Thread creation failed!");
        }
    }
    close(server_sock)
    return 0;
}