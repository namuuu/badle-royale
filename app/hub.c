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

    received_t msg;

    socket_t sock = prepareForClient(ip, port, SOCK_STREAM);

    waitForInput(sock, &msg);

    close(sock.fd);
}

void waitForInput(socket_t sock, generic msg){
    recevoir(sock, msg, deserial);
}

void deserial(generic quoi, char *msg){
    // Séparer les données selon le séparateur "-" et les ranger dans une array de strings
    char *token = strtok(msg, "-");
    ((received_t*)quoi)->code = atoi(token);
    ((received_t*)quoi)->nbArgs = 0;
    token = strtok(NULL, "-");
    int i = 0;
    int switchToken = atoi(token);
    switch (switchToken)
    {
    default:
        while(token != NULL){
            ((received_t*)quoi)->args[i] = token;
            ((received_t*)quoi)->nbArgs++;
            token = strtok(NULL, "-");
            i++;
        }
        break;
    }
}