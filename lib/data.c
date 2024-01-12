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
#include "session.h"

/* FONCTIONS */
/**
 * @brief Envoyer 
 * @param sock Fournit la socket
 * @param quoi Fournit la donnée à envoyer
 * @param serial Fournit la fonction de sérialisation
*/
void envoyer(socket_t sock, generic quoi, pFct serial) {
    buffer_t buffer;

    if(sock.mode == SOCK_DGRAM) {
        // Fonctionnement DGRAM
        if(serial == NULL) 
            envoyerMSGDgram(sock, (char *) quoi);
        else {
            buffer = serial(quoi);
            envoyerMSGDgram(sock, (char *) quoi);
        }
    } else if (sock.mode == SOCK_STREAM) {
        // Fonctionnement STREAM
        if(serial == NULL) 
            envoyerMSGStream(sock, (char *) quoi);
        else {
            buffer = serial(quoi);
            envoyerMSGStream(sock, (char *) quoi);
        }
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
