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


void client(char *ipClient, short portClient, char *ipServeur, short portServeur);
void serveur(char *ip, short port);

int main(int argc, char *argv[])
{
    #ifdef CLIENT
        if(argc != 5) {
            printf(RED);
            printf("Usage: %s <ip-client> <port-client> <ip-serveur> <port-serveur>\n", argv[0]);
            printf(RESET);
            return -1;
        }
        client(argv[1], atoi(argv[2]), argv[3], atoi(argv[4]));
    #endif
    #ifdef SERVEUR
        if(argc != 3) {
            printf(RED);
            printf("Usage: %s <ip> <port>\n", argv[0]);
            printf(RESET);
            return -1;
        }
        serveur(argv[1], atoi(argv[2]));
    #endif
    #if !defined(CLIENT) && !defined(SERVEUR) 
        perror("Aucun mode défini. Veuillez définir CLIENT ou SERVEUR");
    #endif
    return 0; 
}

/**
 * @fn void client(char *ip, short port, char *ipServeur, short portServeur);
 * 
 * @brief Lance un client
 * @param ipClient Fournit l'ip du client
 * @param portClient Fournit le port du client
 * @param ipServeur Fournit l'ip du serveur
 * @param portServeur Fournit le port du serveur
*/
void client(char *ipClient, short portClient, char *ipServeur, short portServeur) {
    printc(BOLDCYAN, "Lancement du Client\n");
    printc(CYAN, "| IP: ");
    printf("%s\n", ipClient);
    printc(CYAN, "| Port: ");
    printf("%d\n", portClient);
    printc(CYAN, "| Mode: ");
    printf("%s\n", DATA_MODE == SOCK_DGRAM ? "DGRAM" : "STREAM");
    printc(CYAN, "| Connexion vers: ");
    printf("%s %d\n", ipServeur, portServeur);
    socket_t sock = connectToServer(ipClient, portClient, ipServeur, portServeur, DATA_MODE);

    char *msg = "Hello World";
    envoyer(sock, msg, NULL);
}

/**
 * @fn void serveur(char *ip, short port);
 * 
 * @brief Lance un serveur
 * @param ip Fournit l'ip du serveur
 * @param port Fournit le port du serveur
*/
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