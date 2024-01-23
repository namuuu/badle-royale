#include <stdio.h>

#include "../lib/data.h"

//HUB
void serveur(char *ip, short port);
void waitForInput(socket_t sock, generic msg);


int main(int argc, char *argv[]){
   
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

    recevoir(sock, msg, NULL);

}