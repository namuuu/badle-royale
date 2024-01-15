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
    printc(BOLDYELLOW, "Lancement du Client\n");
    printc(YELLOW, "| IP: ");
    printf("%s\n", ip);
    printc(YELLOW, "| Port: ");
    printf("%d\n", port);
    printc(YELLOW, "| Mode: ");
    printf("%s\n", DATA_MODE == SOCK_DGRAM ? "DGRAM" : "STREAM");
    socket_t sock = connectToServer(ip, port, DATA_MODE);

    char *msg = "Hello World";
    envoyer(sock, msg, NULL);
}

void serveur(char *ip, short port) {
    printc(BOLDYELLOW, "Lancement du Serveur\n");
    printc(YELLOW, "| IP: ");
    printf("%s\n", ip);
    printc(YELLOW, "| Port: ");
    printf("%d\n", port);
    printc(YELLOW, "| Mode: ");
    printf("%s\n", DATA_MODE == SOCK_DGRAM ? "DGRAM" : "STREAM");
    char *msg = NULL;

    socket_t sock = prepareForClient(ip, port, DATA_MODE);

    recevoir(sock, msg, NULL);
}