#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define IP_ADDRESS "127.0.0.1"
#define PORT 9000
#define MAX_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int socket_fd;
    struct sockaddr_in socket_address;
    socklen_t socket_address_size = sizeof(socket_address);
    ssize_t message_len;
    char buffer[MAX_BUFFER_SIZE] = {0};

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    socket_address.sin_port = htons(PORT);

    if (connect(socket_fd, (struct sockaddr *) &socket_address, socket_address_size) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }
    
    while ((message_len = recv(socket_fd, buffer, MAX_BUFFER_SIZE, 0)) > 0) {
        buffer[message_len] = '\0';
        puts(buffer);
    }

    for (int i = 11111; send(socket_fd, buffer, strlen(buffer), 0) >= 0 && i >= 0; i--) {
        printf("%d\n", i);
        sprintf(buffer, "%d", i);
    }

    close(socket_fd);
    return EXIT_SUCCESS;
}
