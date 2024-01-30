/* ---------------------------------------  INCLUDES ------------------------------------------ */
#include <stdio.h>

#include "../lib/data.h"
#include <pthread.h>
#include <time.h>
#include <signal.h>

/* ---------------------------------------  DEFINE ------------------------------------------ */
#define CHECK(sts,msg) if ((sts) == -1) {perror(msg); exit(-1);}


/* ---------------------------------------- STRUCTURE ---------------------------------------- */

typedef struct
{
    int code;
    char* args[10];
    int nbArgs;
} send_t;


typedef struct {
    int code;
    char *args[10];
    int nbArgs;
} received_t;

typedef struct {
    char *ip;
    unsigned short port;
    char *code;
    int pidLobby;
} lobbyData_t;

/* ---------------------------------------  PROTOTYPES ------------------------------------------ */

void serveur();
void waitForInput(socket_t sock, generic msg);
void serveurLobby(int idLobby);
void serial(generic quoi, char* req);
void deserial(generic quoi, char *msg);
void genererCode(char code[6]);
char *generateLobbyCode();