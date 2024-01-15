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

    if(sock.mode == SOCK_DGRAM) {
        // Fonctionnement DGRAM
        if(serial != NULL) {
            serial(quoi, buffer);
            ecrireSocket(sock, buffer, sock.mode);
        } else {
            ecrireSocket(sock, (char *) quoi, sock.mode);
        }
    } else if (sock.mode == SOCK_STREAM) {
        // Fonctionnement STREAM
        if(serial != NULL) {
            serial(quoi, buffer);
            ecrireSocket(sock, buffer, sock.mode);
        } else {
            ecrireSocket(sock, (char *) quoi, sock.mode);
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
    generic receivedData = NULL;

     if(sock.mode == SOCK_DGRAM) {
        // Fonctionnement DGRAM
        lireSocket(sock/*, (char *) receivedData*/);
        // if(deSerial != NULL) 
        //     deSerial(quoi, receivedData);
        // else
        //     quoi = receivedData;
    } else if (sock.mode == SOCK_STREAM) {
        // Fonctionnement STREAM
        lireSocket(sock/*, (char *) receivedData*/);
        // if(deSerial != NULL) 
        //     deSerial(quoi, receivedData);
        // else
        //     quoi = receivedData;
    } else {
        perror("Erreur de mode de socket");
    }
}

/**
 * \fn socket_t connectToServer(char *ip, short port, short mode);
 * 
 * @brief Création d'une socket dans un mode donné
 * @param mode Fournit le mode de la socket à créer (DGRAM / STREAM)
 * @return structure socket_t créée
*/
socket_t connectToServer(char *ip, short port, short mode) {
    if(mode == SOCK_DGRAM) {
        return creerSocketEcritureDgram(ip, port);
    } else if (mode == SOCK_STREAM) {   
        return creerSocketEcritureStream(ip, port);
    } else {
        perror("Erreur de mode de socket");
        exit(-1);
    }
}

/**
 * \fn socket_t prepareForClient(char *ip, short port, short mode);
 * 
 * @brief Création d'une socket dans un mode donné
 * @param mode Fournit le mode de la socket à créer (DGRAM / STREAM)
 * @return structure socket_t créée
*/
socket_t prepareForClient(char *ip, short port, short mode) {
    if(mode == SOCK_DGRAM) {
        return creerSocketEcouteDgram(ip, port);
    } else if (mode == SOCK_STREAM) {
        return creerSocketEcouteStream(ip, port, 5);
    } else {
        perror("Erreur de mode de socket");
        exit(-1);
    }
}