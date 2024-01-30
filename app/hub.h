/* ---------------------------------------  INCLUDES ------------------------------------------ */
#include <stdio.h>

#include "../lib/data.h"
#include <pthread.h>


/* ---------------------------------------  DEFINE ------------------------------------------ */
#define CHECK(sts,msg) if ((sts) == -1) {perror(msg); exit(-1);}

/* ---------------------------------------- STRUCTURE ---------------------------------------- */

typedef struct {
    int code;
    char *args[10];
    int nbArgs;
} received_t;

typedef struct {
    char *ip;
    short port;
    char *code;
    int tidLobby;
} lobbyData_t;
lobbyData_t* tabLobby;

/* ---------------------------------------  PROTOTYPES ------------------------------------------ */

void serveur(char *ip, short port);
void waitForInput(socket_t sock, generic msg);
void serveurLobby();
void deserial(generic quoi, char *msg);