#include <stdio.h>

#include "../lib/data.h"

void client(char *ip, short port);
void serveur(char *ip, short port);

int main(int argc, char *argv[])
{
    if(argc != 3) {
        printf("Usage: %s <ip> <port>\n", argv[0]);
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
        printf("Erreur: ni client ni serveur\n");
    #endif
    return 0; 
}

void client(char *ip, short port) {
    printf("Client\n");
    socket_t sock = connecterSocket(ip, port, SOCK_DGRAM);
    sock.mode = SOCK_DGRAM;

    char *msg = "Hello World";
    envoyer(sock, msg, NULL);
}

void serveur(char *ip, short port) {
    printf("Serveur\n");
    //socket_t sock = creerSocketEcoute(ip, port, 5); // STREAM
    socket_t sock = creerSocketAddr_in(SOCK_DGRAM, ip, port); // DGRAM
    char *msg = NULL;

    recevoir(sock, msg, NULL);
}