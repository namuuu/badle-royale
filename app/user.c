/**
 * @file user.c
 * @author Tomas TRENY
 * @version 1.0
 * @date 2024-01-23
*/

/* INCLUDE */
#include "../lib/data.h"
#include "string.h"


char *ipServeur = "0.0.0.0";
short portServeur = 5000;



void requireLobbyFromCode();
void createLobbyWithCode();
void menu();

int main() {
    menu();
}


/**
 * @fn void menu();
 * 
 * @brief Menu principal
 * 
*/
void menu() {
    int choix = 0;

    printf("Bienvenue sur Badle Royale ! Veuillez sélectionner votre choix : \n");
    printf("\t[1] Rejoindre une partie via un code\n");
    printf("\t[2] Créer une partie\n");
    printf("\t[3] Quitter\n");
    printf("Votre choix : ");
    scanf("%d", &choix);

    switch(choix) {
        case 1:
            requireLobbyFromCode();
            break;
        case 2:
            createLobbyWithCode();
            break;
        case 3:
            printf("\nAu revoir !\n\n");
            break;
        default:
            printf("\nErreur de saisie\n\n");
            menu();
            break;
    }
}

/**
 * @fn void requireLobbyFromCode();
 * 
 * @brief Connexion au serveur
 * @return 
*/
void requireLobbyFromCode() {
    // TODO: compléter fct

    // Params hardcoder
    char *ipClient = "127.0.0.1";
    short portClient = 5001;

    printf("Quel est le code de la partie que vous souhaitez rejoindre ? (5 charactères MAX) ");
    char *code = malloc(sizeof(char) * 5);
    scanf("%s", code);

    // Requête de connexion au serveur
    char *req = malloc(sizeof(char) * 30);
    strcat(req, "requireLobbyFromCode-");
    strcat(req, code);

    // Connexion au serveur en STREAM
    socket_t sock = connectToServer(ipClient, portClient, ipServeur, portServeur, SOCK_STREAM);

    envoyer(sock, req, NULL);
    printf("Requête envoyée : %s\n", req);
}

void createLobbyWithCode(){

    // Params hardcoder
    char *ipClient = "127.0.0.1";
    short portClient = 5001;

    printf("Quel code voulez vous donner à votre partie ? (5 charactères MAX)");
    char *code = malloc(sizeof(char) * 5);
    scanf("%s", code);

    //Requête de création de lobby
    char *req = malloc(sizeof(char) * 30);
    strcat(req, "createLobby-");
    strcat(req, code);

    // Connexion au serveur en STREAM
    socket_t sock = connectToServer(ipClient, portClient, ipServeur, portServeur, SOCK_STREAM);

    envoyer(sock, req, NULL);
    printf("Requête envoyée : %s\n", req);
    
}