#include <stdio.h>

int main(int argc, char *argv[])
{
    if(argc != 3) {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        return -1;
    }

    printf("IP: %s\n", argv[1]);
    printf("Port: %s\n", argv[2]);

    #ifdef CLIENT
        printf("Client\n");
    #endif
    #ifdef SERVEUR
        printf("Serveur\n");
    #endif
    #if !defined(CLIENT) && !defined(SERVEUR)
        printf("Erreur: ni client ni serveur\n");
    #endif
    return 0; 
}