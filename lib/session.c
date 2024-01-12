/**
 * @file session.c
 * @author Tomas TRENY, Leevan DAVID
 * @version 1.0
 * @date 2024-01-12
*/

#include "session.h"
#define CHECK(status, msg)                                                   \
    if ((status) == -1) {                                                    \
        perror(msg);                                                         \
        exit(EXIT_FAILURE);                                                  \
    }


/* ---------------------------------------- FONCTIONS ---------------------------------------- */
/**
 * \fn socket_t creerSocket(int mode);
 * 
 * @brief Création d'une socket dans un mode donné
 * @param mode Fournit le mode de la socket à créer (DGRAM / STREAM)
 * @return structure socket_t créée
*/
socket_t creerSocket(short mode) {
    if (mode != SOCK_DGRAM && mode != SOCK_STREAM) {
        printf("Mode de socket invalide %d\n", mode);
        exit(-1);
    }
    socket_t sock;
    sock.mode = mode;
    CHECK(sock.fd = socket(PF_INET, sock.mode, 0), "Impossible de créer la socket");
    return sock;
}

/**
 * \fn struct sockaddr_in creerAddr_in(char *ip, short port);
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
 * \fn socket_t creerSocketAddr_in(short mode, char *ip, short port);
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
    sock = creerSocket(sock.mode);

    // Bind de la socket
    printf("ip: %s\n", ip);
    printf("Socket: fd %d, mode %d, adresse %s, port %d\n", sock.fd, sock.mode, inet_ntoa(sock.addr.sin_addr), ntohs(sock.addr.sin_port));
    CHECK(bind(sock.fd, (struct sockaddr *)&sock.addr, sizeof(sock.addr)), "Impossible de lier la socket");
    return sock;
}

/**
 * \fn socket_t creerSocketEcoute(char *ip, short port);
 * 
 * @brief Création d'une socket d'écoute en STREAM
 * @param ip Fournit l'adresse IP de la socket à créer
 * @param port Fournit le port de la socket à créer
 * @note Modifier la structure pour le mode DGRAM
 * @return structure socket_t
*/
socket_t creerSocketEcoute(char *ip, short port, short maxClts) {
    if(maxClts <= 0) {
        printf("Nombre de clients maximum invalide\n");
        exit(-1);
    }
    
    // Association des paramètres à la structure de la socket
    socket_t sock;

    // Création dans le mode STREAM et mise sur écoute
    sock = creerSocketAddr_in(SOCK_STREAM, ip, port);
    CHECK(listen(sock.fd, maxClts), "Impossible de mettre la socket en écoute");
    return sock;
}

/**
 * \fn socket_t connecterSocket(char *ip, short port);
 * 
 * @brief Création d'une socket d'écoute en STREAM
 * @param ip Fournit l'adresse IP de la socket à créer
 * @param port Fournit le port de la socket à créer
 * @note Modifier la structure pour le mode DGRAM
 * @return structure socket_t
*/
socket_t connecterSocket (char *ip, short port) {

    socket_t sock;
    sock = creerSocket(SOCK_STREAM);
    sock.ip = ip;
    sock.port = port;
    sock.addr = creerAddr_in(sock.ip, sock.port);

    CHECK(connect(sock.fd, (struct sockaddr *)&sock.addr, sizeof(sock.addr)), "Impossible de se connecter au serveur");
    return sock;
}

/**
 * \fn void ecrireSocket(socket_t sock, char *msg);
 * 
 * @brief Ecrit un message sur une socket
 * @param sock Fournit la socket
 * @param msg Fournit le message à écrire
*/
void ecrireSocket(socket_t sock, char *msg) {
    // TODO: Actuellement, ça ne marche que pour les sockets STREAM, il faut faire le cas DGRAM
    CHECK(write(sock.fd, msg, strlen(msg)+1), "Impossible d'écrire sur la socket");
}

/**
 * \fn void lireSocket(socket_t sock);
 * 
 * @brief Lit un message sur une socket
 * @param sock Fournit la socket
*/
void lireSocket(socket_t sockEcoute) {

    struct sockaddr_in addr;
    socklen_t addrLen = sizeof(sockEcoute.addr);
    int sd;
    char msg[1024];

    CHECK(sd = accept(sockEcoute.fd, (struct sockaddr *)&sockEcoute.addr, &addrLen), "Impossible d'accepter la connexion");
    CHECK(read(sd, msg, sizeof(msg)), "Impossible de lire sur la socket");

    printf("Message reçu : [%s] de la part de [%s]\n", msg, inet_ntoa(addr.sin_addr));    
}

/**
 * \fn void closeSocket(socket_t sock);
 * 
 * @brief Ferme une socket
 * @param sock Fournit la socket
*/
void closeSocket(socket_t sock) {
    CHECK(close(sock.fd), "Impossible de fermer la socket");
}