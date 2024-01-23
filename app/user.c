/**
 * @file user.c
 * @author Tomas TRENY
 * @version 1.0
 * @date 2024-01-23
*/

/* INCLUDE */
#include "../lib/data.h"

void requireLobbyFromCode();
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
            // TODO: créer partie
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
    char *ipClient = "172.0.0.1";
    short portClient = 5001;

    char *ipServeur = "172.0.0.1";
    short portServeur = 5000;

    // Connexion au serveur en STREAM
    socket_t sock = connectToServer(ipClient, portClient, ipServeur, portServeur, SOCK_STREAM);
}