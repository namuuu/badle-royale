/**
 * @file session.h
 * @author Tomas TRENY, Leevan DAVID
 * @version 1.0
 * @date 2024-01-12
*/

/* INCLUDE */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/**
 * \fn int creerSocket(int mode);
 * 
 * @brief Création d'une socket dans un mode donné
 * @param mode Fournit le mode de la socket à créer (DGRAM / STREAM)
 * @return num de fd de la socket créé
*/
int creerSocket(int mode);

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
 * \fn int creerSocketAddr_in(int mode, char *ip, short port);
 * 
 * @brief Création d'une socket dans un mode donné et réalisation de l'adressage en INET 
 * @param mode Fournit le mode de la socket à créer (DGRAM / STREAM)
 * @param ip Fournit l'adresse IP de la socket à créer
 * @param port Fournit le port de la socket à créer
 * @return Numéro de fd de la socket créé
*/
int creerSocketAddr_in(int mode, char *ip, short port);

/**
 * \fn int creerSocketClient(char *ip, short port);
 * 
 * @brief Création d'une socket d'écoute en STREAM
 * @param ip Fournit l'adresse IP de la socket à créer
 * @param port Fournit le port de la socket à créer
 * @return Numéro de fd de la socket créé
*/
int creerSocketEcoute(char *ip, short port, short maxClts); 

/**
 * \fn int connecterSocket(char *ip, short port);
 * 
 * @brief Création d'une socket d'écoute en STREAM
 * @param ip Fournit l'adresse IP de la socket à créer
 * @param port Fournit le port de la socket à créer
 * @return Numéro de fd de la socket créé
*/
int connecterSocket (char *ip, short port);

/**
 * \fn int ecrireSocket(int sock, char *msg,;
 * 
 * @brief Ecrit un message sur une socket
 * @param sock Fournit la socket
 * @param msg Fournit le message à écrire
*/
void ecrireSocket(int sock, char *msg);

/**
 * \fn int lireSocket(int sock, char *msg,;
 * 
 * @brief Lit un message sur une socket
 * @param sock Fournit la socket
*/
void lireSocket(int sock);

/**
 * \fn void closeSocket(int sock);
 * 
 * @brief Ferme une socket
 * @param sock Fournit la socket
*/
void closeSocket(int sock);