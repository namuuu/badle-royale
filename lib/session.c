/**
 * @file session.c
 * @author Tomas TRENY
 * @brief Stream class
 * @version 1.0
 * @date 2023-12-14
*/

#include "session.h"
#define CHECK(status, msg)                                                   \
    if ((status) == -1) {                                                    \
        perror(msg);                                                         \
        exit(EXIT_FAILURE);                                                  \
    }
/**
 * \fn int creerSocket(int mode);
 * 
 * @brief Création d'une socket dans un mode donné
 * @param mode Fournit le mode de la socket à créer (DGRAM / STREAM)
 * @return Numéro de fd de la socket créé 
*/
int creerSocket(int mode) {
    if (mode != SOCK_DGRAM && mode != SOCK_STREAM) {
        printf("Mode de socket invalide %d\n", mode);
        exit(-1);
    }
    int sock;
    CHECK(sock = socket(PF_INET, mode, 0), "Impossible de créer la socket");
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

    // Mise au format machine de l'adressage
    addr.sin_family = PF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	memset(&addr.sin_zero,0,8);
    return addr;
}

/**
 * \fn int creerSocketAddr_in(int mode, char *ip, short port);
 * 
 * @brief Création d'une socket dans un mode donné et réalisation de l'adressage en INET
 * @param mode Fournit le mode de la socket à créer (DGRAM / STREAM)
 * @param ip Fournit l'adresse IP de la socket à créer
 * @param port Fournit le port de la socket à créer
 * @return Numéro de fd de la socket créé
*/
int creerSocketAddr_in(int mode, char *ip, short port) {
    if(mode != SOCK_DGRAM && mode != SOCK_STREAM) {
        printf("Mode de socket invalide %d\n", mode);
        exit(-1);
    }
    
    struct sockaddr_in addr;
    int sock;
    addr = creerAddr_in(ip, port);
    sock = creerSocket(mode);
    CHECK(bind(sock, (struct sockaddr *)&addr, sizeof(addr)), "Impossible de lier la socket");
    return sock;
}

/**
 * \fn int creerSocketClient(char *ip, short port);
 * 
 * @brief Création d'une socket d'écoute en STREAM
 * @param ip Fournit l'adresse IP de la socket à créer
 * @param port Fournit le port de la socket à créer
 * @return Numéro de fd de la socket créé
*/
int creerSocketEcoute(char *ip, short port, short maxClts) {

    if(maxClts <= 0) {
        printf("Nombre de clients maximum invalide\n");
        exit(-1);
    }

    int sock = creerSocketAddr_in(SOCK_STREAM, ip, port);
    CHECK(listen(sock, maxClts), "Impossible de mettre la socket en écoute");
    return sock;

}
/**
 * \fn int connecterSocket(char *ip, short port);
 * 
 * @brief Création d'une socket d'écoute en STREAM
 * @param ip Fournit l'adresse IP de la socket à créer
 * @param port Fournit le port de la socket à créer
 * @return Numéro de fd de la socket créé
*/
int connecterSocket (char *ip, short port) {

    int sock = creerSocket(SOCK_STREAM);
    struct sockaddr_in addr = creerAddr_in(ip, port);
    //struct sockaddr_in monAddr;
    CHECK(connect(sock, (struct sockaddr *)&addr, sizeof(addr)), "Impossible de se connecter au serveur");
    //CHECK(getsockname(sock, (struct sockaddr *)&monAddr, sizeof(monAddr)), "Impossible de récupérer l'adresse de la socket");
    //fprintf(stderr, "L'adresse IP client = [%s], Port Client = [%d]\n", inet_ntoa(monAddr.sin_addr), ntohs(monAddr.sin_port));
    return sock;
}

/**
 * \fn int ecrireSocket(int sock, char *msg,;
 * 
 * @brief Ecrit un message sur une socket
 * @param sock Fournit la socket
 * @param msg Fournit le message à écrire
*/
void ecrireSocket(int sock, char *msg) {
    CHECK(write(sock, msg, strlen(msg)+1), "Impossible d'écrire sur la socket");
}

/**
 * \fn int lireSocket(int sock, char *msg,;
 * 
 * @brief Lit un message sur une socket
 * @param sock Fournit la socket
*/
void lireSocket(int sockEcoute) {

    struct sockaddr_in addr;
    socklen_t addrLen = sizeof(addr);
    int sock, sd;
    char msg[1024];

    CHECK(sd = accept(sockEcoute, (struct sockaddr *)&addr, &addrLen), "Impossible d'accepter la connexion");
    CHECK(read(sd, msg, sizeof(msg)), "Impossible de lire sur la socket");

    printf("Message reçu : [%s] de la part de [%s]\n", msg, inet_ntoa(addr.sin_addr));    
}

/**
 * \fn void closeSocket(int sock);
 * 
 * @brief Ferme une socket
 * @param sock Fournit la socket
*/
void closeSocket(int sock) {
    CHECK(close(sock), "Impossible de fermer la socket");
}