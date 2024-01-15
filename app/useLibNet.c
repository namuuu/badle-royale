#include <stdio.h>

#include "../lib/data.h"

#ifdef DGRAM
    #define DATA_MODE SOCK_DGRAM
#endif
#ifdef STREAM
    #define DATA_MODE SOCK_STREAM
#endif
#if !defined(STREAM) && !defined(DGRAM)
    #define DATA_MODE SOCK_STREAM
#endif

#define CHECK(status, msg)                                                   \
    if ((status) == -1) {                                                    \
        perror(msg);                                                         \
        exit(EXIT_FAILURE);                                                  \
    }


void client(char *ip, short port);
void serveur(char *ip, short port);

int main(int argc, char *argv[])
{
    if(argc != 3) {
        printf(RED);
        printf("Usage: %s <ip> <port>\n", argv[0]);
        printf(RESET);
        return -1;
    }

    printf("IP: %s\n", argv[1]);
    printf("Port: %s\n", argv[2]);

    #ifdef CLIENT
        client(argv[1], atoi(argv[2]));
    #endif
    #ifdef SERVEUR
        serveur(argv[1], atoi(argv[2]));
    #endif
    #if !defined(CLIENT) && !defined(SERVEUR)
        perror("Aucun mode défini. Veuillez définir CLIENT ou SERVEUR");
    #endif
    return 0; 
}

void client(char *ip, short port) {
    printf("Client\n");
    socket_t sock = connectToServer(ip, port, DATA_MODE);

    char *msg = "Hello World";
    envoyer(sock, msg, NULL);
}

void serveur(char *ip, short port) {
    printf("Serveur\n");
    char *msg = NULL;

    socket_t sock = prepareForClient(ip, port, DATA_MODE);

    recevoir(sock, msg, NULL);
}