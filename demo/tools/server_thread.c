#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 9000
#define CONNECTIONS 2
#define MAX_BUFFER_SIZE 1024

struct sockaddr_in socket_address;
socklen_t socket_address_size = sizeof(socket_address);
int server_fd;

void *handle_request(void *fd) {
    int *client_fd = (int *) fd;
    char buffer[MAX_BUFFER_SIZE] = {0};
    
    // while (scanf("%s", buffer) != EOF) {
    //     send(client_fd, buffer, strlen(buffer), 0);
    // }

    for (int i = INT_MAX; send(*client_fd, buffer, strlen(buffer), 0) >= 0 && i >= 0; i--) {
        printf("%d\r\n", i);
        sprintf(buffer, "%d", i);
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    int option = 1;
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

    if (listen(server_fd, CONNECTIONS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    pthread_t threads[CONNECTIONS];
    int client_fds[CONNECTIONS];
    for (int i = 0; i < CONNECTIONS; i++) {
        client_fds[i] = accept(server_fd, (struct sockaddr *) &socket_address, &socket_address_size);
        if (client_fds[i] < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        if (pthread_create(&threads[i], NULL, handle_request, &client_fds[i]) < 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < CONNECTIONS; i++) {
        if (pthread_join(threads[i], NULL) < 0) {
            perror("pthread_join");
        }
    }

    close(server_fd);
    return EXIT_SUCCESS;
}
