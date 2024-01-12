/**
 * @file data.h
 * @author Tomas TRENY
 * @brief Data class
 * @version 1.0
 * @date 2024-01-12
 * 
*/

/* INCLUDE */
#include "data.h"

/* FONCTIONS */
/**
 * @brief Envoyer 
 * @param sock Fournit la socket
 * @param quoi Fournit la donnée à envoyer
 * @param serial Fournit la fonction de sérialisation
*/
void envoyer(socket_t sock, generic quoi, pFct serial) {
    buffer_t buffer;

    if(serial != NULL) 
        buffer = serialiser(quoi);

    if(sock.mode == SOCK_DGRAM) {
        // Fonctionnement DGRAM
        envoyerMSGDgram(sock, (char *) quoi);
    } else if (sock.mode == SOCK_STREAM) {
        // Fonctionnement STREAM
        envoyerMSGStream(sock, (char *) quoi);
    } else {
        // Mode inconnu
        perror("Erreur de mode de socket");
    }
}

/**
 * @brief Recevoir 
 * @param sock Fournit la socket
 * @param quoi Fournit la donnée à recevoir
 * @param deSerial Fournit la fonction de désérialisation
*/
void recevoir(socket_t sock, generic quoi, pFct deSerial) {
     if(sock.mode == SOCK_DGRAM) {
        // Fonctionnement DGRAM
    } else if (sock.mode == SOCK_STREAM) {
        // Fonctionnement STREAM
    } else {
        perror("Erreur de mode de socket");
    }
}

/**
 * @brief Transforme une struct de données en string
 * @param buffer Fournit le buffer
 * @param quoi Fournit la donnée à sérialiser
 * @return buffer_t
*/
void serialiser(char *buffer, generic quoi) {
    // TODO
}
