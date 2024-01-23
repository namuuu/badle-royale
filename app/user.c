/**
 * @file user.c
 * @author Tomas TRENY
 * @version 1.0
 * @date 2024-01-23
*/

/* INCLUDE */
#include "../lib/data.h"

void requireLobbyFromCode();

int main() {
    requireLobbyFromCode();
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