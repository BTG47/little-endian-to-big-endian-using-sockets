
#include "server.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

int recv_integer(int client_sock);

int main(int argc, char *argv[])
{
    int socket_desc = initialization();
    
    while(1) {
        
        int client_sock = connection(socket_desc);
        
        int integer = recv_integer(client_sock);
        printf("%d\n", integer);
    }
    
    close(socket_desc);
    return 0;
}

int recv_integer(int client_sock)
{
    int read_size, integer;
    
    // receive a message from client
    read_size = recv(client_sock, &integer, sizeof(integer), 0);
    
    if(read_size == 0) {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1) {
        perror("recv failed");
    }
    
    return integer;
}
