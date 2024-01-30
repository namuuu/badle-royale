#include <stdio.h>

#include "../lib/data.h"
#include "hub.h"

#define CHECK(status, msg)                                                   \
    if (-1 == (status)) {                                                    \
        perror(msg);                                                         \
        exit(EXIT_FAILURE);                                                  \
    }

//HUB
void serveur(char *ip, short port);
void waitForInput(socket_t sock, generic msg);
void serveurLobby();
void deserial(generic quoi, char *msg);

typedef struct {
    char *ip;
    short port;
    char *code;
    int tidLobby;
} lobbyData_t;
lobbyData_t* tabLobby;

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

    //creation d'un lobby
    // if (1) {
    //     CHECK(pthread_create(&tidLobby, NULL, serveurLobby, 0),"pthread_create(lobby)");
    //     shutdown(sock,2);
        
    // }


    int fd; // file descriptor
    fd = shm_open("tabLobby", O_CREAT | O_RDWR, S_IRWXU);  
    // Set the size of the shared memory object
    int pageSize = sysconf(_SC_PAGE_SIZE);
    CHECK(ftruncate(fd, pageSize), "__ftruncate__");
    // Map the tabEtats object into the virtual address space of the calling process
    tabLobby = mmap(0, pageSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
   

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

/**
 * @fn void serveurLobby();
 * 
 * @brief Lance un lobby
 * @param ip Fournit l'ip du lubby
 * @param port Fournit le port du lobby
*/
void serveurLobby() {
    printc(BOLDGREEN, "Lancement du Lobby\n");
    char* ip = "0.0.0.0";
    printc(GREEN, "| IP: ");
    printf("%s\n", ip);
    short port = 0;
    printc(GREEN, "| Port: ");
    printf("%d\n", port);
   
    // char *msg = NULL;

    // socket_t sock = prepareForClient(ip, port, SOCK_STREAM);

    // waitForInput(sock, msg);
    // pthread_exit(NULL);
}

void waitForInput(socket_t sock, generic msg){
    printf("En attente de connexion");
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