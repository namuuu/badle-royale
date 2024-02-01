/**
 * @file session.c
 * @author Tomas TRENY, Leevan DAVID
 * @version 1.0
 * @date 2024-01-12
*/

#include "session.h"

/* ---------------------------------------- FONCTIONS ---------------------------------------- */
/**
 * @fn int creerSocket(short mode);
 * 
 * @brief Création d'une socket dans un mode donné
 * @param mode Fournit le mode de la socket à créer (DGRAM / STREAM)
 * @return structure socket_t créée
*/
int creerSocket(short mode) {
    if (mode != SOCK_DGRAM && mode != SOCK_STREAM) {
        printf("Mode de socket invalide %d\n", mode);
        exit(-1);
    }
    int fd;
    CHECK(fd = socket(PF_INET, mode, 0), "Impossible de créer la socket");
    return fd;
}

/**
 * @fn struct sockaddr_in creerAddr_in(char *ip, short port);
 * 
 * @brief Réalisation de l'adressage d'une socket en INET
 * @param ip Fournit l'adresse IP de la socket à créer
 * @param port Fournit le port de la socket à créer
 * @return Structure sockaddr_in créée
*/
struct sockaddr_in creerAddr_in(char *ip, short port) {
    struct sockaddr_in addr;

    if(ip == NULL) {
        printf("Adresse IP invalide\n");
        exit(-1);
    }

    // Mise au format machine de l'adressage
    addr.sin_family = PF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	memset(&addr.sin_zero,0,8);
    return addr;
}

/**
 * @fn socket_t creerSocketAddr_in(short mode, char *ip, short port);
 * 
 * @brief Création d'une socket dans un mode donné et réalisation de l'adressage en INET 
 * @param mode Fournit le mode de la socket à créer (DGRAM / STREAM)
 * @param ip Fournit l'adresse IP de la socket à créer
 * @param port Fournit le port de la socket à créer
 * @return structure socket_t 
*/
socket_t creerSocketAddr_in(short mode, char *ip, short port) {

    // Vérification du mode
    if(mode != SOCK_DGRAM && mode != SOCK_STREAM) {
        printf("Mode de socket invalide %d\n", mode);
        exit(-1);
    }
    
    // Association des paramètres à la structure de la socket
    socket_t sock;
    sock.addr = creerAddr_in(ip, port);
    sock.mode = mode;
    sock.ip = ip;
    sock.port = port;
    sock.fd = creerSocket(sock.mode);

    //CHECK(connect(sock.fd, (struct sockaddr *)&sock.addr, sizeof(sock.addr)), "Impossible de connecter la socket");
    // printf("Socket créée\n");
    return sock;
}

/**
 * @fn socket_t creerSocketEcouteStream(char *ip, short port, short maxClts);
 * 
 * @brief Création d'une socket d'écoute en STREAM
 * @param ip Fournit l'adresse IP de la socket à créer
 * @param port Fournit le port de la socket à créer
 * @return structure socket_t
*/
socket_t creerSocketEcouteStream(char *ip, short port, short maxClts) {
    if(ip == NULL) {
        printf("Adresse IP invalide\n");
        exit(-1);
    }
    if(maxClts <= 0) {
        printf("Nombre de clients maximum invalide\n");
        exit(-1);
    }
    
    // Association des paramètres à la structure de la socket
    socket_t sock;

    // Création dans le mode STREAM et mise sur écoute
    sock = creerSocketAddr_in(SOCK_STREAM, ip, port);
    
    CHECK(bind(sock.fd, (struct sockaddr *)&sock.addr, sizeof(sock.addr)), "Impossible de lier la socket");
    CHECK(listen(sock.fd, maxClts), "Impossible de mettre la socket en écoute");
    return sock;
}

/**
 * @fn socket_t creerSocketEcritureStream(char *ip, short port);
 * 
 * @brief Création d'une socket d'écriture en STREAM
 * @param ip Fournit l'adresse IP de la socket à créer
 * @param port Fournit le port de la socket à créer
 * @return structure socket_t
*/
socket_t creerSocketEcritureStream(char *ipClient, short portClient, char *ipServeur, short portServeur) {
    socket_t sock = creerSocketAddr_in(SOCK_STREAM, ipServeur, portServeur);
    struct sockaddr_in addrClient = creerAddr_in(ipClient, portClient);
    CHECK(bind(sock.fd, (struct sockaddr *)&addrClient, sizeof(addrClient)), "Impossible de lier la socket");

    CHECK(connect(sock.fd, (struct sockaddr *)&sock.addr, sizeof(sock.addr)), "Impossible de se connecter au serveur");
    return sock;
}

/**
 * @fn socket_t creerSocketEcouteDgram(char *ip, short port);
 * 
 * @brief Création d'une socket d'écoute en DGRAM
 * @param ip Fournit l'adresse IP de la socket à créer
 * @param port Fournit le port de la socket à créer
 * @return structure socket_t
*/
socket_t creerSocketEcouteDgram(char *ip, short port) {
    // Création dans le mode DGRAM et mise sur écoute
    socket_t sock = creerSocketAddr_in(SOCK_DGRAM, ip, port);

    // associer l'adressage à la socket *serveur
	CHECK(bind(sock.fd, (struct sockaddr *)&sock.addr, sizeof(sock.addr)), "Impossible de lier la socket");
    return sock;
}

/**
 * @fn socket_t creerSocketEcritureDgram(char *ip, short port);
 * 
 * @brief Création d'une socket d'écriture en DGRAM
 * @param ip Fournit l'adresse IP de la socket à créer
 * @param port Fournit le port de la socket à créer
 * @return structure socket_t
*/
socket_t creerSocketEcritureDgram(char *ip, short port) {
    // Création dans le mode DGRAM et mise sur écoute
    socket_t sock = creerSocketAddr_in(SOCK_DGRAM, ip, port);
    sock.mode = SOCK_DGRAM;
    return sock;
}

/**
 * @fn socket_t connecterSocket(char *ip, short port, short mode);
 * 
 * @brief Création d'une socket d'écoute en STREAM
 * @param ip Fournit l'adresse IP de la socket à créer
 * @param port Fournit le port de la socket à créer
 * @return structure socket_t
*/
socket_t connecterSocket(char *ip, short port, short mode) {

    // Vérification du mode
    if(mode != SOCK_DGRAM && mode != SOCK_STREAM) {
        printf("Mode de socket invalide %d\n", mode);
        exit(-1);
    }

    socket_t sock;
    sock.fd = creerSocket(mode);
    sock.ip = ip;
    sock.port = port;
    sock.addr = creerAddr_in(sock.ip, sock.port);

    CHECK(connect(sock.fd, (struct sockaddr *)&sock.addr, sizeof(sock.addr)), "Impossible de se connecter au serveur");
    return sock;
}

/**
 * @fn void ecrireSocket(socket_t sock, char *msg, short mode); 
 * 
 * @brief Ecrit un message sur une socket
 * @param sock Fournit la socket
 * @param mode Fournit le mode de la socket (DGRAM / STREAM)
 * @param msg Fournit le message à écrire
*/
void ecrireSocket(socket_t sock, char *msg, short mode) {
    if(mode != SOCK_DGRAM && mode != SOCK_STREAM) {
        printf("Mode de socket invalide %d\n", mode);
        exit(-1);
    }
    if (mode == SOCK_DGRAM) 
        CHECK(sendto(sock.fd, msg, strlen(msg)+1, 0, (struct sockaddr *)&sock.addr, sizeof(sock.addr)), "Impossible d'écrire sur la socket");
    if (mode == SOCK_STREAM) {
        CHECK(write(sock.fd, msg, strlen(msg)+1), "Impossible d'écrire sur la socket");
    }
}

/**
 * @fn void lireSocket(socket_t sockEcoute);
 * 
 * @brief Lit un message sur une socket
 * @param sock Fournit la socket
*/
int lireSocket(socket_t sockEcoute, char* receivedData) {

    struct sockaddr_in addr;
    socklen_t addrLen = sizeof(sockEcoute.addr);
    int sd;

    if(sockEcoute.mode != SOCK_DGRAM && sockEcoute.mode != SOCK_STREAM) {
        printf("Mode de socket invalide %d\n", sockEcoute.mode);
        exit(-1);
    }

    if(sockEcoute.mode == SOCK_DGRAM) 
        CHECK(recvfrom(sockEcoute.fd, receivedData, sizeof(receivedData), 0, (struct sockaddr *)&addr, &addrLen), "Impossible de lire sur la socket");
    if(sockEcoute.mode == SOCK_STREAM) {
        CHECK(sd = accept(sockEcoute.fd, (struct sockaddr *)&sockEcoute.addr, &addrLen), "Impossible d'accepter la connexion");
        // CHECK(close(sockEcoute.fd), "Impossible de fermer la socket");
        CHECK(read(sd, receivedData, MAX_BUFF), "Impossible de lire sur la socket");
    } 

    return sd;

    //printf("Message reçu : [%s] de la part de [%s]\n", receivedData, inet_ntoa(sockEcoute.addr.sin_addr));  
}

int lireSocketNext(socket_t sockEcoute, char* receivedData) {

    struct sockaddr_in addr;
    socklen_t addrLen = sizeof(sockEcoute.addr);

    if(sockEcoute.mode != SOCK_DGRAM && sockEcoute.mode != SOCK_STREAM) {
        printf("Mode de socket invalide %d\n", sockEcoute.mode);
        exit(-1);
    }

    if(sockEcoute.mode == SOCK_DGRAM) 
        CHECK(recvfrom(sockEcoute.fd, receivedData, sizeof(receivedData), 0, (struct sockaddr *)&addr, &addrLen), "Impossible de lire sur la socket");
    if(sockEcoute.mode == SOCK_STREAM) {
        CHECK(read(sockEcoute.fd, receivedData, MAX_BUFF), "Impossible de lire sur la socket");
    } 

    return sockEcoute.fd;
}

/**
 * @fn void closeSocket(socket_t sock);
 * 
 * @brief Ferme une socket
 * @param sock Fournit la socket
*/
void closeSocket(socket_t sock) {
    CHECK(close(sock.fd), "Impossible de fermer la socket");
}