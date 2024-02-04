/* ---------------------------------------- INCLUDES ---------------------------------------- */

#include "../lib/data.h"
#include "string.h"


/* ---------------------------------------  DEFINE ------------------------------------------ */

/* SERVER HARDCODED DATA */
#define IP_HUB "0.0.0.0"
#define PORT_HUB 5000

/* CLIENT DATA */
#define IP_CLIENT "127.0.0.1"
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

/* ---------------------------------------  PROTOTYPES ------------------------------------------ */

int requireLobbyFromCode();
int createLobbyWithCode();
int connectToLobby(char* ip, unsigned short port, char* code);
void menu();
void serial(generic quoi, char* req);
void deserial(generic quoi, char *msg);
void waitForInput(socket_t sock, generic msg);