/**
 * @file user.c
 * @author Tomas TRENY
 * @version 1.0
 * @date 2024-01-23
*/

/* INCLUDE */
#include "user.h"

short portClient = 0;

int main() {
    char* choix = malloc(sizeof(char) * 10);
    char* returnMsg;
    int returnCode;
    system("clear");
    menu();

    while(choix[0] != 'Q') {
        printf(YELLOW "$ " RESET);
        scanf("%s", choix);

        switch(choix[0]) {
            case '1':
                returnCode = requireLobbyFromCode();
                switch (returnCode)
                {
                case 500:
                    returnMsg = RED "ERR:" RESET " Le lobby n'existe pas!";
                    break;
                default:
                    break;
                }
                break;
            case '2':
                returnCode = createLobbyWithCode();
                switch (returnCode)
                {  
                case 503:
                    returnMsg = RED "ERR:" RESET " Le lobby est plein!";
                    break;
                case 504:
                    returnMsg = RED "ERR:" RESET " Le lobby est en cours de partie!";
                    break;
                }
                break;
            case 'Q':
                printf("Fermeture du système..........................Au revoir!\n");
                exit(EXIT_SUCCESS);
            case '\n':
                returnMsg = "Veuillez entrer une option valide";
                break;
            default:
                break;
        }

        system("clear");
        menu();
        if(returnCode != 0)
            printf("\n%s\n\n", returnMsg);
        returnCode = 0;
    }
    
}


/**
 * @fn void menu();
 * 
 * @brief Menu principal
 * 
*/

void menu() {
    printc(BOLDBLUE, "Bienvenue sur BadleRoyale !\n");
    printc(BLUE, "Veuillez sélectionner une option: \n");
    printf(CYAN "\t[1]" RESET " Rejoindre une partie via un code\n");
    printf(CYAN "\t[2]" RESET " Créer une partie\n");
    printf(CYAN "\t[Q]" RESET " Quitter\n");
}

/**
 * @fn void requireLobbyFromCode();
 * 
 * @brief Connexion au serveur
 * @return 
*/
int requireLobbyFromCode() {
    int c;
    /* discard all characters up to and including newline */
    while ((c = getchar()) != '\n' && c != EOF); 

    printf(BLUE "Quel est le code de la partie que vous souhaitez rejoindre ?" RESET " (5 caractères MAX)\n" YELLOW "$ " RESET);
    char *code = malloc(sizeof(char) * MAX_BUFF);
    fgets(code, MAX_BUFF, stdin);

    // Suppression du \n
    code[strlen(code) - 1] = '\0';

    // Requête de connexion au serveur
    send_t sendData;
    sendData.code = 100;
    sendData.nbArgs = 1;
    sendData.args[0] = code;

    // Connexion au serveur en STREAM
    socket_t sock = connectToServer(IP_CLIENT, portClient, IP_HUB, PORT_HUB, SOCK_STREAM);

    envoyer(sock, &sendData, serial);
    printf("Recherche du lobby %s...\n", code);

    received_t recData;
    recevoirSuivant(sock, &recData, deserial);
    
    switch (recData.code)
    {
    case 200:
        printf("Connexion au lobby %s...\n", code);
        break;
    case 500:
        return recData.code;
    }
    close(sock.fd);
    return 0;
}

int createLobbyWithCode() {
    //Requête de création de lobby
    send_t reqData;
    reqData.code = 101;
    reqData.nbArgs = 0;

    // Connexion au serveur en STREAM
    socket_t sock = connectToServer(IP_CLIENT, portClient, IP_HUB, PORT_HUB, SOCK_STREAM);

    envoyer(sock, &reqData, serial);

    received_t recData;
    recevoirSuivant(sock, &recData, deserial);

    close(sock.fd);

    return connectToLobby(recData.args[0], atoi(recData.args[2]), recData.args[1]);
}

int connectToLobby(char* ip, unsigned short port, char* code) {
    socket_t sockLobby = connectToServer(IP_CLIENT, portClient, ip, port, SOCK_STREAM);

    send_t reqDataLobby;
    reqDataLobby.code = 102;
    reqDataLobby.nbArgs = 0;


    envoyer(sockLobby, &reqDataLobby, serial);
    printf("Connexion au lobby %s...\n", code);

    received_t recDataLobby;
    recevoirSuivant(sockLobby, &recDataLobby, deserial);

    switch (recDataLobby.code)
    {
    case 202:
        printf("Lobby %s connecté !\n", code);
        while(1);
        break;
    default:
        printf("Lobby %s reçu ! (code: %d)\n", code, recDataLobby.code);
        return recDataLobby.code;
    }

    return 0;
}

void serial(generic quoi, char* req) {
    send_t transQuoi = (*(send_t*)quoi);

    sprintf(req, "%d", transQuoi.code); // Convertit le code en char
    if(transQuoi.nbArgs == 0)
        return;
    for(int i = 0; i < transQuoi.nbArgs; i++) {
        strcat(req, "-");
        strcat(req, transQuoi.args[i]);
    }
}

void deserial(generic quoi, char *msg) {

    // Séparer les données selon le séparateur "-" et les ranger dans une array de strings
    char *token = strtok(msg, "-");
    ((received_t*)quoi)->code = atoi(token);
    ((received_t*)quoi)->nbArgs = 0;
    token = strtok(NULL, "-");
    int i = 0;
    if(token == NULL)
        return;
    int switchToken = atoi(token);
    switch (switchToken)
    {
    default:
        while(token != NULL){
            ((received_t*)quoi)->args[i] = token;
            ((received_t*)quoi)->nbArgs++;
            token = strtok(NULL, "-");
            i++;
        }
        break;
    }
}

void waitForInput(socket_t sock, generic msg){
    printf("En attente de connexion\n");
    recevoir(sock, msg, deserial);
}