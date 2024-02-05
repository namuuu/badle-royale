/**
 * @file data.h
 * @author Tomas TRENY
 * @brief Data class
 * @version 1.0
 * @date 2024-01-12
*/

/* INCLUDE */
#include "data.h"

/* FONCTIONS */
/**
 * @fn void envoyer(socket_t sock, generic quoi, pFct serial);
 * 
 * @brief Création d'une socket dans un mode donné
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
            ecrireSocket(sock, quoi, sock.mode);
        }
    } else if (sock.mode == SOCK_STREAM) {
        // Fonctionnement STREAM
        if(serial != NULL) {
            serial(quoi, buffer);
            ecrireSocket(sock, buffer, sock.mode);
        } else {
            ecrireSocket(sock, quoi, sock.mode);
        }
    } else {
        // Mode inconnu
        perror("Erreur de mode de socket");
    }
}

/**
 * @fn void recevoir(socket_t sock, generic quoi, pFct deSerial);
 * 
 * @brief Réception d'une donnée sur une socket
 * @param sock Fournit la socket
 * @param quoi Fournit la donnée à recevoir
 * @param deSerial Fournit la fonction de désérialisation
*/
socket_t recevoir(socket_t sock, generic quoi, pFct deSerial) {
    buffer_t receivedData;
    socket_t sockEcoute;
    sockEcoute.mode = sock.mode;

     if(sock.mode == SOCK_DGRAM) {
        // Fonctionnement DGRAM
        int sd = lireSocket(sock, receivedData);
        sockEcoute.fd = sd;
        if(deSerial != NULL) 
            deSerial(quoi, receivedData);
        else
            quoi = &receivedData;
        return sockEcoute;
    } else if (sock.mode == SOCK_STREAM) {
        // Fonctionnement STREAM
        int sd = lireSocket(sock, receivedData);
        sockEcoute.fd = sd;
        if(deSerial != NULL) {
            deSerial(quoi, receivedData);
        } else
            quoi = &receivedData;
        return sockEcoute;
    } else {
        perror("Erreur de mode de socket");
    }
    exit(EXIT_FAILURE);
}

socket_t recevoirSuivant(socket_t sock, generic quoi, pFct deSerial) {
    buffer_t receivedData;
    socket_t sockEcoute;
    sockEcoute.mode = sock.mode;

     if(sock.mode == SOCK_DGRAM) {
        // Fonctionnement DGRAM
        int sd = lireSocketNext(sock, receivedData);
        sockEcoute.fd = sd;
        if(deSerial != NULL) 
            deSerial(quoi, receivedData);
        else
            quoi = &receivedData;
        return sockEcoute;
    } else if (sock.mode == SOCK_STREAM) {
        // Fonctionnement STREAM
        int sd = lireSocketNext(sock, receivedData);
        sockEcoute.fd = sd;
        if(deSerial != NULL) {
            deSerial(quoi, receivedData);
        } else
            quoi = &receivedData;
        return sockEcoute;
    } else {
        perror("Erreur de mode de socket");
    }
    exit(EXIT_FAILURE);
}

socket_t accepterConnexion(socket_t sock) {
    socklen_t addrLen = sizeof(sock.addr);
    int sd;

    if(sock.mode != SOCK_STREAM) {
        printf("Mode de socket invalide %d\n", sock.mode);
        exit(-1);
    }
    socket_t sockEcoute;
    CHECK(sd = accept(sock.fd, (struct sockaddr *)&sock.addr, &addrLen), "Impossible d'accepter la connexion");
    // CHECK(close(sock.fd), "Impossible de fermer la socket");
    sockEcoute.fd = sd;
    sockEcoute.mode = SOCK_STREAM;
    sockEcoute.ip = inet_ntoa(sock.addr.sin_addr);
    sockEcoute.port = ntohs(sock.addr.sin_port);

    return sockEcoute;
}

/**
 * @fn socket_t connectToServer(char *ip, short port, short mode);
 * 
 * @brief Création d'une socket dans un mode donné
 * @param mode Fournit le mode de la socket à créer (DGRAM / STREAM)
 * @return structure socket_t créée
*/
socket_t connectToServer(char *ipClient, short portClient, char *ipServeur, short portServeur, short mode) {
    if(mode == SOCK_DGRAM) {
        return creerSocketEcritureDgram(ipServeur, portServeur);
    } else if (mode == SOCK_STREAM) {   
        return creerSocketEcritureStream(ipClient, portClient, ipServeur, portServeur);
    } else {
        perror("Erreur de mode de socket");
        exit(-1);
    }
}

/**
 * @fn socket_t prepareForClient(char *ip, short port, short mode);
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

/** 
 * @fn void fermerConnexion(socket_t sock)
 * 
 * @brief Fermeture d'une connexion
 * @param sock Fournit la socket à fermer
*/
void fermerConnexion(socket_t sock) {
    closeSocket(sock);
}