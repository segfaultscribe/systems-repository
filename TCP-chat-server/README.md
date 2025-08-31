# 🖧 TCP Chat Server in C

A simple multithreaded TCP chat server written in C using POSIX sockets and `pthread`. Each client that connects is handled in its own thread, allowing parallel communication between multiple clients and the server.


- Multithreaded client handling using `pthread`
- Simple echo logic (server sends back whatever the client sends)
- proper handling of client disconnection
- minimal dependencies
- Localhost testing and LAN-compatible 

This one is a stepping stone to building a full scale http web server in C.
## 🛠️ Build Instructions

Open a terminal in the project directory and compile both files:

### Server:

```bash
gcc -o server server.c -pthread
gcc -o client client.c
```

In two seperate terminals

```
./server
```

```
./client
```
Start seding messages on client

`NOTE`: You can open multiple client terminals and parallely communicate with the server.
