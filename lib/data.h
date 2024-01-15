/**
 * @file data.h
 * @author Tomas TRENY
 * @brief Data class
 * @version 1.0
 * @date 2024-01-12
*/

/* INCLUDE */
#include "session.h"

/* DEFINITIONS */
#define MAX_BUFF 1024
typedef char buffer_t[MAX_BUFF];
typedef void * generic;
typedef void (*pFct)(generic, generic);

/* DECLARATIONS DE FONCTIONS */

/**
 * @fn void envoyer(socket_t sock, generic quoi, pFct serial);
 * 
 * @brief Création d'une socket dans un mode donné
 * @param sock Fournit la socket
 * @param quoi Fournit la donnée à envoyer
 * @param serial Fournit la fonction de sérialisation
*/
void envoyer(socket_t sock, generic quoi, pFct serial);

/**
 * @fn void recevoir(socket_t sock, generic quoi, pFct deSerial);
 * 
 * @brief Réception d'une donnée sur une socket
 * @param sock Fournit la socket
 * @param quoi Fournit la donnée à recevoir
 * @param deSerial Fournit la fonction de désérialisation
*/
void recevoir(socket_t sock, generic quoi, pFct deSerial);

/**
 * @fn socket_t connectToServer(char *ip, short port, short mode);
 * 
 * @brief Création d'une socket dans un mode donné
 * @param mode Fournit le mode de la socket à créer (DGRAM / STREAM)
 * @return structure socket_t créée
*/
socket_t connectToServer(char *ip, short port, short mode);

/**
 * @fn socket_t prepareForClient(char *ip, short port, short mode);
 * 
 * @brief Création d'une socket dans un mode donné
 * @param mode Fournit le mode de la socket à créer (DGRAM / STREAM)
 * @return structure socket_t créée
*/
socket_t prepareForClient(char *ip, short port, short mode);



