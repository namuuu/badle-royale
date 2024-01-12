/**
 * @file data.h
 * @author Tomas TRENY
 * @brief Data class
 * @version 1.0
 * @date 2024-01-12
*/

/* INCLUDE */

/* DEFINITIONS */
#define MAX_BUFF 1024
typedef char buffer_t[MAX_BUFF];
typedef void * generic;
typedef void (*pFct)(generic, generic);

/* DECLARATIONS DE FONCTIONS */

/**
 * @brief Envoyer en DGRAM
 * @param sock Fournit la socket
 * @param quoi Fournit la donnée à envoyer
 * @param serial Fournit la fonction de sérialisation
*/
void envoyer(socket_t sock, generic quoi, pFct serial);

/**
 * @brief Recevoir en DGRAM
 * @param sock Fournit la socket
 * @param quoi Fournit la donnée à recevoir
 * @param deSerial Fournit la fonction de désérialisation
*/
void recevoir(socket_t sock, generic quoi, pFct deSerial);

