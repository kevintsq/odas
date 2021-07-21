#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 9000
#define MAX_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    struct sockaddr_in socket_address;
    socklen_t socket_address_size = sizeof(socket_address);
    int server_fd, client_fd, option = 1, message_len;
    char buffer[MAX_BUFFER_SIZE] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
    socket_address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *) &socket_address, socket_address_size) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 1) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((client_fd = accept(server_fd, (struct sockaddr *) &socket_address, &socket_address_size)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    for (int i = 11111; send(client_fd, buffer, strlen(buffer), 0) >= 0 && i >= 0; i--) {
        printf("%d\n", i);
        sprintf(buffer, "%d", i);
    }
    
    while ((message_len = recv(client_fd, buffer, MAX_BUFFER_SIZE, 0)) > 0) {
        buffer[message_len] = '\0';
        puts(buffer);
    }

    close(client_fd);
    close(server_fd);
    return EXIT_SUCCESS;
}
