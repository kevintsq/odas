#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <getopt.h>

int main(int argc, char* argv[]) {

    char verbose = 0;

    int server_id;
    struct sockaddr_in server_address;
    int connection_id;
    char *message;
    int messageSize;

    int c;
    unsigned int port_number = 9000;
    char *file_name = NULL;
    FILE *fp = stdout;

    const unsigned int nBytes = 1024;

    while ((c = getopt(argc, argv, "p:o:hv")) != -1) {

        switch (c) {

            case 'o':

                file_name = (char *) malloc(sizeof(char) * (strlen(optarg)+1));
                strcpy(file_name, optarg);   

            break;

            case 'p':

                port_number = atoi(optarg);

            break;

            case 'h':
            default:

                printf("+----------------------------------------------------+\n");
                printf("|        ODAS (Open embeddeD Audition System)        |\n");
                printf("+----------------------------------------------------+\n");
                printf("| Author:      Francois Grondin                      |\n");
                printf("| Email:       francois.grondin2@usherbrooke.ca      |\n");
                printf("| Website:     introlab.3it.usherbrooke.ca           |\n");
                printf("| Repository:  github.com/introlab/odas              |\n");
                printf("| Version:     1.0                                   |\n");
                printf("+----------------------------------------------------+\n");        
                printf("| -h       Help                                      |\n");
                printf("| -o       Output file [default: stdout]             |\n");
                printf("| -p       Port number [default: 9000]               |\n");
                printf("| -v       Verbose                                   |\n");
                printf("+----------------------------------------------------+\n");

                exit(EXIT_SUCCESS);                     
        }

    }

    server_id = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port_number);

    printf("Opening file............. "); fflush(stdout);
    if (file_name != NULL) {
        fp = fopen(file_name, "wb");
    }
    printf("[OK]\n");

    printf("Binding socket........... ");  fflush(stdout);
    bind(server_id, (struct sockaddr *) &server_address, sizeof(server_address));
    printf("[OK]\n");

    printf("Listening socket......... ");  fflush(stdout);
    listen(server_id, 1);
    printf("[OK]\n");

    printf("Waiting for connection... "); fflush(stdout);
    connection_id = accept(server_id, (struct sockaddr *) NULL, NULL);
    printf("[OK]\n");

    message = (char *) malloc(sizeof(char) * nBytes);

    printf("Receiving data........... "); fflush(stdout);
    while ((messageSize = recv(connection_id, message, nBytes, 0)) > 0) {
        message[messageSize] = '\0';
        fwrite(message, messageSize, sizeof(char), fp);
    }
    printf("[OK]\n");

    printf("Closing file............. "); fflush(stdout);
    if (file_name != NULL) {
        fclose(fp);
    }
    printf("[OK]\n");
    return EXIT_SUCCESS;
}
