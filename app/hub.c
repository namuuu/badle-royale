#include <stdio.h>

#include "../lib/data.h"
#include "hub.h"

//HUB
void serveur(char *ip, short port);
void waitForInput(socket_t sock, generic msg);
void deserial(generic quoi, char *msg);


int main(int argc, char *argv[]){
    system("clear");
   
    if(argc != 3) {
        printf(RED);
        printf("Usage: %s <ip> <port>\n", argv[0]);
        printf(RESET);
        return -1;
    }
    serveur(argv[1], atoi(argv[2]));
   
    return 0; 
}
/**
 * @fn void serveur(char *ip, short port);
 * 
 * @brief Lance un serveur HUB
 * @param ip Fournit l'ip du serveur HUB
 * @param port Fournit le port du serveur HUB
*/
void serveur(char *ip, short port) {
    printc(BOLDYELLOW, "Lancement du HUB\n");
    printc(YELLOW, "| IP: ");
    printf("%s\n", ip);
    printc(YELLOW, "| Port: ");
    printf("%d\n", port);

    char *msg = NULL;

    socket_t sock = prepareForClient(ip, port, SOCK_STREAM);

    waitForInput(sock, msg);
}

void waitForInput(socket_t sock, generic msg){
    recevoir(sock, msg, deserial);

    received_t *received = (received_t *) msg;
    printf("test\n");
    printf("Received data: %d\n", received->code);
}

void deserial(generic quoi, char *msg){
    // Séparer les données selon le séparateur "-" et les ranger dans une array de strings
    quoi = malloc(sizeof(received_t));
    received_t *received = (received_t *) quoi;
    printf("check\n");
    char *token = strtok(msg, "-");
    int requestCode = atoi(token);
    printf("checking\n");
    received->code = requestCode;
    token = strtok(NULL, "-");
    int i = 0;
    printf("checkfuck\n");
    while(token != NULL){
        received->args[i] = token;
        received->nbArgs++;
        token = strtok(NULL, "-");
        i++;
    }
    printf("checkok %d\n", received->code);


    received_t *rece = (received_t *) quoi;
    printf("checkok2 %d\n", rece->code);
}