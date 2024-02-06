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

/* ---------------------------------------  ENUMS ------------------------------------------ */

typedef enum  {
    PREGAME,
    STARTED
} lobbyState;

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
    int pidPlayer;
    int lobbyHost;

} playerData_t;

typedef struct {
    char *ip;
    unsigned short port;
    char code[6];
    playerData_t players[PLAYER_MAX];
    int playerCount;
    int pidLobby;
    lobbyState state;
} lobbyData_t;

/* ---------------------------------------  PROTOTYPES ------------------------------------------ */

/**
 * @fn void serveur(char *ip, short port);
 * 
 * @brief Lance un serveur HUB
*/
void serveur();


void waitForInput(socket_t sock, generic msg);

/**
 * @fn void pregameRoutine();
 * 
 * @brief Lance un lobby
 * @param idLobby Emplacement du lobby dans le tableau
*/
void pregameRoutine(int idLobby);

/**
 * @fn void gameRoutine(socket_t sockPlayer, int idLobby, int idPlayer);
 * 
 * @brief Lance un lobby
 * @param idLobby Emplacement du lobby dans le tableau
 * @param sockPlayer 
 * @param idPlayer Id du joeur dans le lobby 
*/
void gameRoutine(socket_t sockPlayer, int idLobby, int idPlayer);

/**
 * @fn      void installerGestionSignal(int sigNum, void (*handler)(int))
 * @brief   Installer le traitement handler pour un déclenchement sur occurence du signal sigNum
 * @param   sigNum : Numéro du signal déclencheur
 * @param   handler : Nom de la fonction de traitement du signal sigNum
 * @note    handler peut valoir SIG_DFL (traitement par défaut) ou SIG_IGN (pour ignorer le signal)
 */
void installerGestionSignal(int sigNum, void (*handler)(int));

/**
 * \fn void suppressionCode(FILE* fichier, const char *code);
 * 
 * @brief Suppression d'un code dans le fichier
 * @param fichier Nom du fichier à modifier
 * @param code Code à supprimer
 */
void suppressionCode(const char *code);

/**
 * @fn      void traiterSignal(int sigNum)
 * @brief   Traitement du signal sigNum
 * @param   sigNum : Numéro du signal déclencheur
 * @note    Signaux implémentés : SIGALRM
 */
void traiterSignal(int sigNum);

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

/**
 * \fn void genererCode(FILE* fichier, char code[6]);
 * 
 * @brief Génération d'un code aléatoire
 * @param code Code à ajouter au fichier
*/
void genererCode(char* code);

/**
 * \fn void generateLobbyCode();
 *
 * @brief Génération d'un code de session
  *@param code : code donné au lobby 
*/
void generateLobbyCode(char *code);

/**
 * \fn int recognizePlayer(int idLobby, char* ip, unsigned short port, int num ) ;
 * 
 * @brief reconnaitre les differents joueurs 
 * @param idLobby Emplacement du lobby dans le tableau
 * @param ip : adresse ip du lobby
 * @param port : port du lobby 
 */
int recognizePlayer(int idLobby, char* ip, unsigned short port);