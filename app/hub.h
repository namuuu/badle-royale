/* ---------------------------------------  INCLUDES ------------------------------------------ */
#include <stdio.h>

#include "../lib/data.h"
#include <pthread.h>
#include <time.h>
#include <signal.h>

/* ---------------------------------------  DEFINE ------------------------------------------ */
#define HUB_IP "0.0.0.0"
#define HUB_PORT 5000

#define PLAYER_MAX 4

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
    char ip[15];
    unsigned short port;
} playerData_t;

typedef struct {
    char *ip;
    unsigned short port;
    char code[6];
    playerData_t players[PLAYER_MAX];
    int pidLobby;
} lobbyData_t;

/* ---------------------------------------  PROTOTYPES ------------------------------------------ */

void serveur();
void waitForInput(socket_t sock, generic msg);
void serveurLobby(int idLobby);
void serial(generic quoi, char* req);
void deserial(generic quoi, char *msg);
void genererCode(char* code);
void generateLobbyCode(char *code);