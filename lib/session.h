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

/* ---------------------------------------- STRUCTURE ---------------------------------------- */

typedef struct {
    int fd;
    short mode; // DGRAM / STREAM
    struct sockaddr_in addr;
    char *ip;
    short port;
    // @Loc, @Dist
    // flag
    // ...
} socket_t;

/* --------------------------------- PROTOTYPES DES FONCTIONS --------------------------------- */

/**
 * \fn socket_t creerSocket(short mode);
 * 
 * @brief Création d'une socket dans un mode donné
 * @param mode Fournit le mode de la socket à créer (DGRAM / STREAM)
 * @return structure socket_t créée
*/
int creerSocket(short mode);

/**
 * \fn struct sockaddr_in creerAddr_in(char *ip, short port);
 * 
 * @brief Création d'une structure sockaddr_in
 * @param ip Fournit l'adresse IP de la socket à créer
 * @param port Fournit le port de la socket à créer
 * @return Structure sockaddr_in créée
*/
struct sockaddr_in creerAddr_in(char *ip, short port);

/**
 * \fn socket_t creerSocketAddr_in(short mode, char *ip, short port);
 * 
 * @brief Création d'une socket dans un mode donné et réalisation de l'adressage en INET 
 * @param mode Fournit le mode de la socket à créer (DGRAM / STREAM)
 * @param ip Fournit l'adresse IP de la socket à créer
 * @param port Fournit le port de la socket à créer
 * @return structure socket_t 
*/
socket_t creerSocketAddr_in(short mode, char *ip, short port);

/**
 * \fn socket_t creerSocketClient(char *ip, short port);
 * 
 * @brief Création d'une socket d'écoute en STREAM
 * @param ip Fournit l'adresse IP de la socket à créer
 * @param port Fournit le port de la socket à créer
 * @note Modifier la structure pour le mode DGRAM
 * @return structure socket_t
*/
socket_t creerSocketEcoute(char *ip, short port, short maxClts); 

/**
 * \fn socket_t connecterSocket(char *ip, short port);
 * 
 * @brief Création d'une socket d'écoute en STREAM
 * @param ip Fournit l'adresse IP de la socket à créer
 * @param port Fournit le port de la socket à créer
 * @note Modifier la structure pour le mode DGRAM
 * @return structure socket_t
*/
socket_t connecterSocket (char *ip, short port);

/**
 * \fn void ecrireSocket(socket_t sock, char *msg);
 * 
 * @brief Ecrit un message sur une socket
 * @param sock Fournit la socket
 * @param msg Fournit le message à écrire
*/
void ecrireSocket(socket_t sock, char *msg);

/**
 * \fn void lireSocket(socket_t sock);
 * 
 * @brief Lit un message sur une socket
 * @param sock Fournit la socket
*/
void lireSocket(socket_t sock);

/**
 * \fn void closeSocket(socket_t sock);
 * 
 * @brief Ferme une socket
 * @param sock Fournit la socket
*/
void closeSocket(socket_t sock);