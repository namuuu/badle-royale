/* ---------------------------------------  INCLUDES ------------------------------------------ */
#include <stdio.h>

#include "../lib/data.h"
#include <pthread.h>
#include <time.h>
#include <signal.h>

/* ---------------------------------------  DEFINE ------------------------------------------ */
#define HUB_IP "0.0.0.0"
#define HUB_PORT 5000

#define NB_LIGNES 835
#define MAX_LENGTH 9

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
    char lastPlayedWord[MAX_LENGTH];
    int score;
} playerData_t;

typedef struct {
    char *ip;
    unsigned short port;
    char code[6];
    playerData_t players[PLAYER_MAX];
    int playerCount;
    int playerRemaining;
    int pidLobby;
    lobbyState state;
    char word[MAX_LENGTH];
    int round;
} lobbyData_t;

/* ---------------------------------------  PROTOTYPES ------------------------------------------ */

/**
 * @fn void serveur(char *ip, short port);
 * 
 * @brief Lance un serveur HUB
*/
void serveur();

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
 * \fn void suppressionCode(FILE* fichier, const char *code);
 * 
 * @brief Suppression d'un code dans le fichier
 * @param fichier Nom du fichier à modifier
 * @param code Code à supprimer
 */
void suppressionCode(const char *code);

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

/**
 * \fn char *getRandomWord();
 * 
 * @brief Récupère un mot aléatoire dans le fichier dico.txt
 * @return Mot aléatoire
 * @return NULL si le fichier n'a pas pu être ouvert
*/
char *getRandomWord();

int waitForPlayersToFinish(int idLobby);

void hostRoutine(int idLobby);

/**
 * \fn void wordlize();
 * 
 * @brief Vérifie pour chaque caractère s'il est présent dans le mot et renvoie des indices pour chaque caractère
 * @param wordToValidate
 * @param word
*/
char *wordlize(char *word, char *wordToValidate);