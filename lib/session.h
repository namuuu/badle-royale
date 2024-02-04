/**
 * @file session.h
 * @author Tomas TRENY, Leevan DAVID
 * @version 1.0
 * @date 2024-01-12
*/

/* ----------------------------------------- INCLUDE ----------------------------------------- */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <stdlib.h>

/* ----------------------------------------- DEFINE ----------------------------------------- */

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

#define CHECK(sts,msg) if ((sts) == -1) {perror(msg); exit(-1);}

#define printc(color, msg) printf(color "%s" RESET, msg)
#define printcf(color, msg,  ...) printf(color msg RESET, __VA_ARGS__)

#define MAX_BUFF 1024

/* ---------------------------------------- STRUCTURE ---------------------------------------- */

typedef struct {
    int fd;
    short mode; // DGRAM / STREAM
    struct sockaddr_in addr;
    char *ip;
    unsigned short port;
    // @Loc, @Dist
    // flag
    // ...
} socket_t;

typedef char buffer_t[MAX_BUFF];
typedef void * generic;
typedef void (*pFct)(generic, char *);

/* --------------------------------- PROTOTYPES DES FONCTIONS --------------------------------- */

/**
 * @fn int creerSocket(short mode);
 * 
 * @brief Création d'une socket dans un mode donné
 * @param mode Fournit le mode de la socket à créer (DGRAM / STREAM)
 * @return structure socket_t créée
*/
int creerSocket(short mode);

/**
 * @fn struct sockaddr_in creerAddr_in(char *ip, short port);
 * 
 * @brief Réalisation de l'adressage d'une socket en INET
 * @param ip Fournit l'adresse IP de la socket à créer
 * @param port Fournit le port de la socket à créer
 * @return Structure sockaddr_in créée
*/
struct sockaddr_in creerAddr_in(char *ip, short port);

/**
 * @fn socket_t creerSocketAddr_in(short mode, char *ip, short port);
 * 
 * @brief Création d'une socket dans un mode donné et réalisation de l'adressage en INET 
 * @param mode Fournit le mode de la socket à créer (DGRAM / STREAM)
 * @param ip Fournit l'adresse IP de la socket à créer
 * @param port Fournit le port de la socket à créer
 * @return structure socket_t 
*/
socket_t creerSocketAddr_in(short mode, char *ip, short port);

/**
 * @fn socket_t creerSocketEcouteStream(char *ip, short port, short maxClts);
 * 
 * @brief Création d'une socket d'écoute en STREAM
 * @param ip Fournit l'adresse IP de la socket à créer
 * @param port Fournit le port de la socket à créer
 * @return structure socket_t
*/
socket_t creerSocketEcouteStream(char *ip, short port, short maxClts);

/**
 * @fn socket_t creerSocketEcritureStream(char *ip, short port);
 * 
 * @brief Création d'une socket d'écriture en STREAM
 * @param ip Fournit l'adresse IP de la socket à créer
 * @param port Fournit le port de la socket à créer
 * @return structure socket_t
*/
socket_t creerSocketEcritureStream(char *ipClient, short portClient, char *ipServeur, short portServeur);

/**
 * @fn socket_t creerSocketEcouteDgram(char *ip, short port);
 * 
 * @brief Création d'une socket d'écoute en DGRAM
 * @param ip Fournit l'adresse IP de la socket à créer
 * @param port Fournit le port de la socket à créer
 * @return structure socket_t
*/
socket_t creerSocketEcouteDgram(char *ip, short port);

/**
 * @fn socket_t creerSocketEcritureDgram(char *ip, short port);
 * 
 * @brief Création d'une socket d'écriture en DGRAM
 * @param ip Fournit l'adresse IP de la socket à créer
 * @param port Fournit le port de la socket à créer
 * @return structure socket_t
*/
socket_t creerSocketEcritureDgram(char *ip, short port);

/**
 * @fn socket_t connecterSocket(char *ip, short port, short mode);
 * 
 * @brief Création d'une socket d'écoute en STREAM
 * @param ip Fournit l'adresse IP de la socket à créer
 * @param port Fournit le port de la socket à créer
 * @return structure socket_t
*/
socket_t connecterSocket(char *ip, short port, short mode);

/**
 * @fn void ecrireSocket(socket_t sock, char *msg, short mode); 
 * 
 * @brief Ecrit un message sur une socket
 * @param sock Fournit la socket
 * @param mode Fournit le mode de la socket (DGRAM / STREAM)
 * @param msg Fournit le message à écrire
*/
void ecrireSocket(socket_t sock, char *msg, short mode);

/**
 * @fn void lireSocket(socket_t sockEcoute);
 * 
 * @brief Lit un message sur une socket
 * @param sock Fournit la socket
*/
int lireSocket(socket_t sockEcoute, char* receivedData);
int lireSocketNext(socket_t sockEcoute, char* receivedData);
/**
 * @fn void closeSocket(socket_t sock);
 * 
 * @brief Ferme une socket
 * @param sock Fournit la socket
*/
void closeSocket(socket_t sock);