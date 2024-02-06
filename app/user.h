/* ---------------------------------------- INCLUDES ---------------------------------------- */

#include "../lib/data.h"
#include "string.h"
#include <sys/types.h>
#include <signal.h>


/* ---------------------------------------  DEFINE ------------------------------------------ */
#define CHECK(sts,msg) if ((sts) == -1) {perror(msg); exit(-1);}

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

/**
 * @fn void requireLobbyFromCode();
 * 
 * @brief Connexion au serveur
 * @return 
*/
int requireLobbyFromCode();

/**
 * \fn int createLobbyWithCode() ;
 * 
 * @brief permet d'envoyer une demande de création de lobby  
 */
int createLobbyWithCode();

/**
 * \fn int connectToLobby(char* ip, unsigned short port, char* code) ;
 * 
 * @brief envoie une requete pour se connecter au lobby en prennant en compte le statut du joueur pour savoir si il peut aussi lancer la partie   
 * @param ip
 * @param port
 * @param code
 */
int connectToLobby(char* ip, unsigned short port, char* code);

/**
 * @fn void menu();
 * 
 * @brief Menu principal
 * 
*/
void menu();

/**
 * \fn void serial(generic quoi, char* req) ;
 * 
 * @brief transforme les données en chaine de charactères
 * @param quoi 
 * @param req
 */
void serial(generic quoi, char* req);

/**
 * \fn void deserial(generic quoi, char* msg) ;
 * 
 * @brief transforme les chaine de charactères en données
 * @param quoi 
 * @param msg 
 */
void deserial(generic quoi, char *msg);

// void waitForInput(socket_t sock, generic msg);
// void waitForStartOfTheGame(socket_t sockLobby, received_t recDataLobby);

/**
 * \fn void mainToLobby(socket_t socketLobby, int idPlayer)  ;
 * 
 * @brief fonction principale permettant de lancer une partie 
 * @param socketLobby
 * @param idPlayer
 */
void mainToLobby(socket_t socketLobby, int idPlayer);

/**
 * @fn void writerToLobby(char * ip, unsigned short port);
 * 
 * @brief This function will handle writing words to the Lobby in a fork while the main one is listening to the lobby
 * @param ip :  ip du lobby 
 * @param port : port du lobby 
*/
void writerToLobby(char * ip, unsigned short port, int idPlayer);
