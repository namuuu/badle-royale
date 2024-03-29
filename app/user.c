/**
 * @file user.c
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
        return connectToLobby(recData.args[0], atoi(recData.args[2]), recData.args[1]);
        break;
    case 500:
        return recData.code;
    }
    fermerConnexion(sock);
    return 0;
}

/**
 * \fn int createLobbyWithCode() ;
 * 
 * @brief permet d'envoyer une demande de création de lobby  
 */
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

    fermerConnexion(sock);

    return connectToLobby(recData.args[0], atoi(recData.args[2]), recData.args[1]);
}

/**
 * \fn int connectToLobby(char* ip, unsigned short port, char* code) ;
 * 
 * @brief envoie une requete pour se connecter au lobby en prennant en compte le statut du joueur pour savoir si il peut aussi lancer la partie   
 * @param ip
 * @param port
 * @param code
 */
int connectToLobby(char* ip, unsigned short port, char* code) {
    socket_t sockLobby = connectToServer(IP_CLIENT, portClient, ip, port, SOCK_STREAM);
    char* choix = malloc(sizeof(char) * 10); // Will be used to store the user's choice

    system("clear");

    send_t reqDataLobby;
    reqDataLobby.code = 102;
    reqDataLobby.nbArgs = 0;

    envoyer(sockLobby, &reqDataLobby, serial);

    received_t recDataLobby;
    recevoirSuivant(sockLobby, &recDataLobby, deserial);
    int idPlayer = atoi(recDataLobby.args[0]);
    printf(YELLOW "[%s]" RESET " Vous êtes le joueur %d\n\n", code, idPlayer);

    switch (recDataLobby.code){
    case 202:
        if(idPlayer == 0) {
            // Le joueur est considéré comme "hôte"
            while(choix[0] != 'Q') {
                printf(RED "\t[G]" RESET " Lancer une partie\n");
                // printf(RED "\t[Q]" RESET " Quitter\n");
                printf(YELLOW "$ " RESET);
                scanf("%s", choix);
                switch(choix[0]) {
                    case 'G':
                        reqDataLobby.code = 107;
                        reqDataLobby.nbArgs = 0;
                        envoyer(sockLobby, &reqDataLobby, serial);
                        recevoirSuivant(sockLobby, &recDataLobby, deserial);
                        if(recDataLobby.code == 106) {
                            mainToLobby(sockLobby, idPlayer);
                        }
                        break;
                    // case 'Q':
                    //     printf("Fermeture du système..........................Au revoir!\n");
                    //     exit(EXIT_SUCCESS);
                    default:
                        break;
                }
            }
        } else {
            // Le joueur est considéré comme "client"
            printf(GREEN "\tEn attente de l'hôte...\n" RESET);
            recevoirSuivant(sockLobby, &recDataLobby, deserial);
            if(recDataLobby.code == 106) {
                mainToLobby(sockLobby, idPlayer);
            } else {
                printf("Erreur de code: %d\n", recDataLobby.code);
                exit(EXIT_FAILURE);
            }
        }

        while(1);
        break;
    default:
        printf("Lobby %s reçu ! (code: %d)\n", code, recDataLobby.code);
        return recDataLobby.code;
    }

    return 0;
}

/**
 * \fn void mainToLobby(socket_t socketLobby, int idPlayer)  ;
 * 
 * @brief fonction permettant de lancer le jeu 
 * @param socketLobby
 * @param idPlayer
 */
void mainToLobby(socket_t socketLobby, int idPlayer) {
    printf("Lancement du jeu...\n");
    received_t recDataLobby;
    recDataLobby.code = 0;
    while(1) {
        // Wait for the round to start and to receive word length
        recevoirSuivant(socketLobby, &recDataLobby, deserial);
        if(recDataLobby.code == 126) {
            printf(YELLOW "Vous avez gagné ! :D\n" RESET);
            printf("\t Appuyez sur une touche pour quitter...\n");
            getchar();
            exit(EXIT_SUCCESS);
        }
        // system("clear");
        printf("Début du round...\n");
        printf("Longueur du mot: %s (code: %d)\n\t", recDataLobby.args[0], recDataLobby.code);
        for(int i = 0; i < atoi(recDataLobby.args[0]); i++) {
            printf("_ ");
        }
        printf("\n");

        while(recDataLobby.code != 109 && recDataLobby.code != 204) {
             // Fork write To Lobby
            int pidWriter;
            CHECK(pidWriter = fork(), "fork()");
            if(pidWriter == 0) {
                writerToLobby(socketLobby.ip, socketLobby.port, idPlayer, atoi(recDataLobby.args[0]));
                exit(EXIT_SUCCESS);
            }
            // Main
            recevoirSuivant(socketLobby, &recDataLobby, deserial);
            switch (recDataLobby.code)
            {
            case 109:
                printf("\nVous avez perdu ! Le mot était %s\n", recDataLobby.args[0]);
                kill(pidWriter, SIGKILL);
                while(1);
                break;
            case 203: // Incorrect word
                printf(RED "Le mot n'est pas correct !\n" RESET);
                // printWord(recDataLobby.args[1], recDataLobby.args[0]);
                break;
            case 204: // Correct word
                printf(GREEN "Vous avez deviné le mot ! En attente des autres joueurs...\n" RESET);
                break; 
            default:
                printf("Data received: %d\n", recDataLobby.code);
                break;
            }
        }
    }
    

}

/**
 * @fn void writerToLobby(char * ip, unsigned short port);
 * 
 * @brief This function will handle writing words to the Lobby in a fork while the main one is listening to the lobby
 * @param ip :  ip du lobby 
 * @param port : port du lobby 
 * @param idPlayer : id du joueur
 * @param wordSize : taille du mot
*/
void writerToLobby(char * ip, unsigned short port, int idPlayer, long unsigned int wordSize) {
    socket_t sockLobby = connectToServer(IP_CLIENT, 0, ip, port, SOCK_STREAM);

    send_t reqDataLobby;
    reqDataLobby.code = 103;
    reqDataLobby.nbArgs = 2;
    
    char* choix = malloc(sizeof(char) * 10); // Will be used to store the user's choice

    printf(YELLOW "$ " RESET);
    
    while(strlen(choix) != wordSize) {
        scanf("%s", choix);
        if(strlen(choix) != wordSize) {
            printf(RED "ERR:" RESET " Le mot doit faire %ld caractères\n", wordSize);
            printf(YELLOW "$ " RESET);
        }
    }

    // arg 0 = idplayer
    reqDataLobby.args[0] = malloc(sizeof(char) * 10);
    sprintf(reqDataLobby.args[0], "%d", idPlayer);
    // arg 1 = message
    reqDataLobby.args[1] = malloc(sizeof(char) * 10);
    strcpy(reqDataLobby.args[1], choix);

    envoyer(sockLobby, &reqDataLobby, serial);
    // printf("Connexion au lobby %s...\n", code);
}

/**
 * \fn void serial(generic quoi, char* req) ;
 * 
 * @brief transforme les données en chaine de charactères
 * @param quoi 
 * @param req
 */
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

/**
 * \fn void deserial(generic quoi, char* msg) ;
 * 
 * @brief transforme les chaine de charactères en données
 * @param quoi 
 * @param msg 
 */
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

/**
 * \fn void printWord(char *word, char *wordlized);
 * 
 * @brief Affiche les caractères du mot en fonction du code de wordlized (vert => '!' ; rouge => '.' ; jaune => '?')
 * @param word Mot à afficher
 * @param wordlized Code de word
*/
void printWord(char *word, char *wordlized) {
    int i;
    int wL = strlen(word);
    for(i = 0; i < wL; i++) {
        if(wordlized[i] == '!') printcf(GREEN, "\t%c ", word[i]);
        else if(wordlized[i] == '.') printf("\t%c ", word[i]);
        else if(wordlized[i] == '?') printcf(YELLOW, "\t%c ", word[i]);
    }
    printf("\n");

}