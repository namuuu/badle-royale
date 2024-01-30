/**
 * @file user.c
 * @author Tomas TRENY
 * @version 1.0
 * @date 2024-01-23
*/

/* INCLUDE */
#include "user.h"

/* SERVER HARDCODED DATA */
#define IP_HUB "0.0.0.0"
#define PORT_HUB 5000

/* CLIENT DATA */
#define IP_CLIENT "127.0.0.1"
short portClient = 0;

void requireLobbyFromCode();
void createLobbyWithCode();
void menu();
void serial(generic quoi, char* req);
void waitForInput(socket_t sock, generic msg);

int main() {
    char choix = '0';
    system("clear");
    menu();

    while(choix != 'Q') {

        printf(YELLOW "$ " RESET);
        scanf("%c", &choix);

        switch(choix) {
            case '1':
                requireLobbyFromCode();
                exit(EXIT_SUCCESS);
            case '2':
                createLobbyWithCode();
                exit(EXIT_SUCCESS);
            case 'Q':
                printf("Fermeture du système..........................Au revoir!\n");
                exit(EXIT_SUCCESS);
            default:
                menu();
                printf("\nErreur de saisie\n\n");
                break;
        }
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
void requireLobbyFromCode() {

    printf("Quel est le code de la partie que vous souhaitez rejoindre ? (5 charactères MAX) ");
    char *code = malloc(sizeof(char) * MAX_BUFF);
    fgets(code, MAX_BUFF, stdin);
    printf("%s\n", code);
    printf("%ld\n", strlen(code));

    // Requête de connexion au serveur
    char *req = malloc(sizeof(char) * 30);
    strcat(req, "100-");
    strcat(req, code);

    // Connexion au serveur en STREAM
    socket_t sock = connectToServer(IP_CLIENT, portClient, IP_HUB, PORT_HUB, SOCK_STREAM);

    envoyer(sock, req, NULL);
    printf("Requête envoyée : %s\n", req);
  
    close(sock.fd);
}

void createLobbyWithCode() {
    int c;
    /* discard all characters up to and including newline */
    while ((c = getchar()) != '\n' && c != EOF); 

    printf(BLUE "(Optionnel) Indiquez un code pour votre partie: " WHITE "(5 caractères max)\n" YELLOW "$ " RESET);
    char *code = malloc(sizeof(char) * MAX_BUFF);
    fgets(code, MAX_BUFF, stdin);

    // Supprime le \n à la fin de la chaine
    code[strcspn(code, "\n")] = 0;
    // Limite le code à 5 caractères
    if(strlen(code) > 4)
        code[5] = '\0';

    //Requête de création de lobby
    send_t reqData;
    reqData.code = 101;
    reqData.nbArgs = 1;
    reqData.args[0] = code;

    buffer_t buffer;
    serial(&reqData, buffer);

    // Connexion au serveur en STREAM
    socket_t sock = connectToServer(IP_CLIENT, portClient, IP_HUB, PORT_HUB, SOCK_STREAM);

    envoyer(sock, &reqData, serial);

    char *recData = NULL;   
    recevoir(sock, recData, deserial);

    close(sock.fd);
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