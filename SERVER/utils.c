
#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int initialization()
{
    int socket_desc;
    struct sockaddr_in server;
    
    // create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_desc == -1) {
        printf("Could not create socket");
    }
    puts("Socket created");
    
    // prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( PORT_NUM );
    
    // bind
    if(bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
        // print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
    
    return socket_desc;
}

int connection(int socket_desc)
{
    struct sockaddr_in client;
    int client_sock, c, read_size;
    
    // listen
    listen(socket_desc, 3);
    
    // accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    
    // accept connection from an incoming client
    client_sock = accept( socket_desc,
                          (struct sockaddr *) &client,
                          (socklen_t*) &c );
    
    if(client_sock < 0) {
        perror("accept failed");
        exit(1);
    }
    puts("Connection accepted");
    
    return client_sock;
}
