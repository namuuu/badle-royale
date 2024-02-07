
#include "hub.h"

// tabLobby est le tableau permettant de stocker les données des différents lobbys
lobbyData_t* tabLobby;
int nbLobby = 0;

// action est la structure permettant de gérer les signaux
struct sigaction action;

int main() {
    system("clear");

    int fd = shm_open("tabLobby", O_CREAT | O_RDWR, S_IRWXU);  
    // Set the size of the shared memory object
    int pageSize = sysconf(_SC_PAGE_SIZE);
    CHECK(ftruncate(fd, pageSize), "__ftruncate__");
    // Map the tabEtats object into the virtual address space of the calling process
    tabLobby = mmap(0, pageSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    serveur();
    return 0; 
}

/**
 * @fn void serveur(char *ip, short port);
 * 
 * @brief Lance un serveur HUB
*/
void serveur() {
    printc(BOLDYELLOW, "Lancement du HUB\n");
    printf(YELLOW "| IP:" RESET " %s\n" , HUB_IP);
    printf(YELLOW "| Port:" RESET " %d\n", HUB_PORT);

    received_t data;
    socket_t sock = prepareForClient(HUB_IP, HUB_PORT, SOCK_STREAM);

    while(1) {
        socket_t sockClient = recevoir(sock, &data, deserial);


        switch (data.code)
        {
        case 100: // joinLobby - code
            if(data.nbArgs == 0) {
                send_t sendData;
                sendData.code = 506;
                sendData.nbArgs = 0;
                envoyer(sockClient, &sendData, serial);
            }
            for(int i = 0; i < nbLobby; i++) {
                if(strncmp(data.args[0], tabLobby[i].code, 5) == 0) {
                    // Envoi de l'ip et du port au client
                    printf(GREEN "Connexion au Lobby %s (Port %d)\n" RESET, tabLobby[i].code, tabLobby[i].port);
                    send_t sendData;
                    sendData.code = 200;
                    sendData.nbArgs = 3;
                    sendData.args[0] = tabLobby[i].ip;
                    sendData.args[1] = tabLobby[i].code;
                    char* portChar = malloc(sizeof(char) * 5);
                    sprintf(portChar, "%d", tabLobby[i].port);
                    sendData.args[2] = portChar;
                    envoyer(sockClient, &sendData, serial);
                    break;
                }
            }
            // Si le code n'existe pas
            send_t sendData;
            sendData.code = 500;
            sendData.nbArgs = 0;
            envoyer(sockClient, &sendData, serial);
            break;
        case 101: // createLobby
            //creation d'un lobby
            int pidLobby;
            tabLobby[nbLobby].pidLobby = 0;
            CHECK(pidLobby = fork(), "fork()");
            if(pidLobby == 0) {
                // Fils
                pregameRoutine(nbLobby);
                exit(EXIT_SUCCESS);
            } else {
                // Père, attends l'update de la création du Lobby
                while(tabLobby[nbLobby].pidLobby == 0) {
                    sleep(1);
                }
                // Envoi de l'ip et du port au client
                printf(GREEN "Création du Lobby %s (Port %d)\n" RESET, tabLobby[nbLobby].code, tabLobby[nbLobby].port);
                send_t sendData;
                sendData.code = 200;
                sendData.nbArgs = 3;
                sendData.args[0] = tabLobby[nbLobby].ip;
                sendData.args[1] = tabLobby[nbLobby].code;
                char* portChar = malloc(sizeof(char) * 5);
                sprintf(portChar, "%d", tabLobby[nbLobby].port);
                sendData.args[2] = portChar;
                nbLobby++;
                envoyer(sockClient, &sendData, serial);
            }

            break;
        default:
            break;
        }
    }
    
}

/**
 * @fn void pregameRoutine();
 * 
 * @brief Lance un lobby
 * @param idLobby Emplacement du lobby dans le tableau
*/
void pregameRoutine(int idLobby) {
    // Préparation de la socket
    char* ip = "0.0.0.0";
    int port = 0;
    socket_t sock = prepareForClient(ip, port, SOCK_STREAM);
    socket_t sockPlayer;

    socklen_t len = sizeof(sock.addr);
    CHECK(getsockname(sock.fd, (struct sockaddr *)&sock.addr, &len), "getsockname()");
    port = ntohs(sock.addr.sin_port);

    // Emplacement du lobby dans le tableau
    tabLobby[idLobby].ip = ip;
    tabLobby[idLobby].port = port;
    generateLobbyCode(tabLobby[idLobby].code);
    tabLobby[idLobby].pidLobby = getpid();
    tabLobby[idLobby].playerCount = 0;
    tabLobby[idLobby].state = PREGAME;
    tabLobby[idLobby].round = 1;
    strcpy(tabLobby[idLobby].word, ":)");

    received_t recData;

    while (1)
    {
        sockPlayer = accepterConnexion(sock);

        int pidPlayer;
        CHECK(pidPlayer = fork(), "fork()");

        if(pidPlayer == 0) {
            fermerConnexion(sock);
            // Fils
            recevoirSuivant(sockPlayer, &recData, deserial);
            switch (recData.code)
            {
            case 102: // joinLobby
                int idPlayerInLobby = recognizePlayer(idLobby, sockPlayer.ip, sockPlayer.port);
                printf(YELLOW "[%s]"  RESET " %d (id: %d) a rejoint le lobby\n", tabLobby[idLobby].code, sockPlayer.port, idPlayerInLobby);

                // Envoi de confirmation de connexion au Lobby
                send_t sendData;
                sendData.code = 202;
                sendData.nbArgs = 1;
                char idPlayerChar[5];
                sprintf(idPlayerChar, "%d", idPlayerInLobby);
                sendData.args[0] = idPlayerChar;
                
                envoyer(sockPlayer, &sendData, serial);

                if(idPlayerInLobby == 0) {
                    // Le joueur est le lobbyHost
                    recevoirSuivant(sockPlayer, &recData, deserial);
                    if(recData.code == 107) {
                        // Le joueur est prêt à commencer la partie
                        tabLobby[idLobby].state = STARTED;
                        tabLobby[idLobby].playerRemaining = tabLobby[idLobby].playerCount;
                        printf(YELLOW "[%s]"  RESET " Le lobby est prêt à commencer la partie\n", tabLobby[idLobby].code);
                        sendData.code = 106;
                        sendData.nbArgs = 0;
                        envoyer(sockPlayer, &sendData, serial);
                        // fork host routine
                        int pidHost;
                        CHECK(pidHost = fork(), "fork()");
                        if(pidHost == 0) {
                            hostRoutine(idLobby);
                            exit(EXIT_SUCCESS);
                        }

                        gameRoutine(sockPlayer, idLobby, idPlayerInLobby);
                    }
                } else {
                    // Le joueur n'est pas le lobbyHost
                    while(tabLobby[idLobby].state != STARTED) {
                        sleep(1);
                    }
                    sendData.code = 106;
                    sendData.nbArgs = 0;
                    envoyer(sockPlayer, &sendData, serial);
                    gameRoutine(sockPlayer, idLobby, idPlayerInLobby);
                }
                while(1);
                break;
            case 103: // playWord
                // while(1) {
                    strcpy(tabLobby[idLobby].players[atoi(recData.args[0])].lastPlayedWord, recData.args[1]);

                //     recevoirSuivant(sockPlayer, &recData, deserial);
                // }
                exit(EXIT_SUCCESS);
            default:
                fermerConnexion(sock);;
            }
        }

        
    }
}

/**
 * @fn void gameRoutine(socket_t sockPlayer, int idLobby, int idPlayer);
 * 
 * @brief Lance un lobby
 * @param idLobby Emplacement du lobby dans le tableau
 * @param sockPlayer 
 * @param idPlayer Id du joeur dans le lobby 
*/
void gameRoutine(socket_t sockPlayer, int idLobby, int idPlayer) {
    sleep(2);
    char bufferMot[MAX_LENGTH] = ":)";
    char bufferLastWord[MAX_LENGTH] = "NULL";
    int bufferRound = tabLobby[idLobby].round;

    send_t sendData;
    while(tabLobby[idLobby].playerRemaining > 1) {
        // Sélection mot
        while(strcmp(bufferMot, tabLobby[idLobby].word) == 0) {
            // Sleep for 0.3 seconds
            usleep(300000);
        }
        char* motLength = malloc(sizeof(char) * 5);
        sprintf(motLength, "%ld", strlen(tabLobby[idLobby].word) - 1);
        bufferRound = tabLobby[idLobby].round;

        sendData.code = 108;
        sendData.nbArgs = 1;
        strcpy(sendData.args[0], motLength);

        envoyer(sockPlayer, &sendData, serial);

        int currentTimer = 0;
        // 200 * 0.3 = 60 secondes
        while(tabLobby[idLobby].round != tabLobby[idLobby].players[idPlayer].score) {
            while(strcmp(bufferLastWord, tabLobby[idLobby].players[idPlayer].lastPlayedWord) == 0) {
                // printf("Attente de match (%s %s)\n", bufferLastWord, tabLobby[idLobby].players[idPlayer].lastPlayedWord);
                // Sleep for 0.3 second
                usleep(300000);
                currentTimer++;

                if(tabLobby[idLobby].round != bufferRound/* || currentTimer >= 200*/) {
                    // Le joueur a été éliminé
                    sendData.code = 109;
                    sendData.nbArgs = 1;
                    sendData.args[0] = tabLobby[idLobby].word;
                    envoyer(sockPlayer, &sendData, serial);
                    exit(EXIT_SUCCESS);
                }
            }

            // Réception d'un mot
            char* codeword = malloc(sizeof(char) * MAX_LENGTH);
            strcpy(codeword, wordlize(tabLobby[idLobby].word, tabLobby[idLobby].players[idPlayer].lastPlayedWord));
            codeword[strlen(tabLobby[idLobby].players[idPlayer].lastPlayedWord)] = '\0';
            
            // Envoi du codeword au joueur selon le mot joué
            if(checkword(codeword)) {
                sendData.code = 204;
                sendData.nbArgs = 1;
                sendData.args[0] = tabLobby[idLobby].word;
                envoyer(sockPlayer, &sendData, serial);
                tabLobby[idLobby].players[idPlayer].score++;
            } else {
                sendData.code = 203;
                sendData.nbArgs = 1;
                sendData.args[0] = codeword;
                // strcpy(sendData.args[1], tabLobby[idLobby].players[idPlayer].lastPlayedWord);
                envoyer(sockPlayer, &sendData, serial);
            }

            strcpy(bufferLastWord, tabLobby[idLobby].players[idPlayer].lastPlayedWord);
        }

        strcpy(bufferLastWord, "NULL");
        strcpy(tabLobby[idLobby].players[idPlayer].lastPlayedWord, "NULL");
        
        // Attente nouveau round
        while(tabLobby[idLobby].round == bufferRound) {
            sleep(1);
        }
    }
    printf(YELLOW "[%s]" RESET " %d a gagné la partie\n", tabLobby[idLobby].code, idPlayer);
    sendData.code = 126;
    sendData.nbArgs = 0;
    envoyer(sockPlayer, &sendData, serial);

}

void hostRoutine(int idLobby) {
    while(tabLobby[idLobby].playerRemaining > 1) {
        printf(YELLOW "[%s]" RESET " Début du round %d\n", tabLobby[idLobby].code, tabLobby[idLobby].round);
        // Choix du mot
        char bufferMot[MAX_LENGTH];
        strcpy(bufferMot, getRandomWord());
        // printf("Mot choisi : %s\n", bufferMot);
        if(bufferMot[strlen(bufferMot) - 1] == '\n')
            bufferMot[strlen(bufferMot) - 1] = '\0';
        strcpy(tabLobby[idLobby].word, bufferMot);
        printf(YELLOW "[%s]" RESET " Mot choisi : %s\n", tabLobby[idLobby].code, tabLobby[idLobby].word);

        // Attente de la fin du round
        while(!waitForPlayersToFinish(idLobby)) {
            sleep(1);
        }

        for(int i = 0; i < tabLobby[idLobby].playerCount; i++) {
            if(tabLobby[idLobby].players[i].score == tabLobby[idLobby].round) {
                printf(YELLOW "[%s]" RESET " %d a terminé le round %d\n", tabLobby[idLobby].code, i, tabLobby[idLobby].round);
            } else {
                printf(YELLOW "[%s]" RESET " %d n'a pas terminé le round %d\n", tabLobby[idLobby].code, i, tabLobby[idLobby].round);
            }
        }

        tabLobby[idLobby].playerRemaining--;
        tabLobby[idLobby].round++;
    }
}

int waitForPlayersToFinish(int idLobby) {
    int finishedPlayers = 0;
    for(int i = 0; i < tabLobby[idLobby].playerCount; i++) {
        if(tabLobby[idLobby].players[i].score == tabLobby[idLobby].round) {
            finishedPlayers++;
        }
    }
    if(finishedPlayers >= tabLobby[idLobby].playerCount - 1) {
        return 1;
    }
    return 0;
}

/**
 * \fn int recognizePlayer(int idLobby, char* ip, unsigned short port, int num ) ;
 * 
 * @brief reconnaitre les differents joueurs 
 * @param idLobby Emplacement du lobby dans le tableau
 * @param ip : adresse ip du lobby
 * @param port : port du lobby 
 */
int recognizePlayer(int idLobby, char* ip, unsigned short port) {
    int idPlayer = tabLobby[idLobby].playerCount;
    strcpy(tabLobby[idLobby].players[idPlayer].ip, ip);
    tabLobby[idLobby].players[idPlayer].port = port;
    tabLobby[idLobby].players[idPlayer].lobbyHost = 0;  

    if (idPlayer == 0)
        tabLobby[idLobby].players[idPlayer].lobbyHost = 1 ;

    tabLobby[idLobby].players[idPlayer].pidPlayer = getpid();
    tabLobby[idLobby].playerCount++;
    tabLobby[idLobby].players[idPlayer].score = 0;
    strcpy(tabLobby[idLobby].players[idPlayer].lastPlayedWord, "NULL");
    return idPlayer;
}

/**
 * \fn void generateLobbyCode();
 *
 * @brief Génération d'un code de session
  *@param code : code donné au lobby 
*/
void generateLobbyCode(char *code) {
    FILE* fichier;
    char codeLu[6];

    srand(time(NULL));
    fichier = fopen("code.txt", "a+");

    if(fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        exit(1);
    }

    genererCode(code);

    while(fgets(codeLu, 6, fichier) != NULL) {
        if(strcmp(codeLu, code) == 0) {
            genererCode(code);
        }
    }

    fprintf(fichier, "%s\n", code);

    fclose(fichier);
}

/**
 * \fn void genererCode(FILE* fichier, char code[6]);
 * 
 * @brief Génération d'un code aléatoire
 * @param code Code à ajouter au fichier
*/
void genererCode(char* code) {
    int i;
    for (i = 0; i < 5; i++) {
        int random = rand() % 36;
        if(random < 10) {
            code[i] = random + 48;
        } else {
            code[i] = random + 55;
        }
    }
    code[5] = '\0'; 
}

/**
 * \fn void suppressionCode(FILE* fichier, const char *code);
 * 
 * @brief Suppression d'un code dans le fichier
 * @param fichier Nom du fichier à modifier
 * @param code Code à supprimer
 */
void suppressionCode(const char *code) {
    FILE* fichier;
    FILE* fichierTemp;
    char buffer[7];  // Un espace supplémentaire pour le caractère de fin de ligne

    fichier = fopen("code.txt", "r");
    fichierTemp = fopen("temp.txt", "w");

    if(fichier == NULL || fichierTemp == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        exit(1);
    }

    while(fgets(buffer, 7, fichier) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';  // Enlever le saut de ligne
        if(strcmp(buffer, code) != 0) {
            fprintf(fichierTemp, "%s\n", buffer);
        }
    }

    fclose(fichier);
    fclose(fichierTemp);

    remove("code.txt");
    rename("temp.txt", "code.txt");
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
 * \fn char *getRandomWord();
 * 
 * @brief Récupère un mot aléatoire dans le fichier dico.txt
 * @return Mot aléatoire
 * @return NULL si le fichier n'a pas pu être ouvert
*/
char *getRandomWord() {
    FILE *f; 
    char *word = malloc(MAX_LENGTH * sizeof(char));
    int i = 1, random;

    f = fopen("./exe/dico.txt", "r");
    if (f == NULL) {
        printc(RED, "ERREUR : Ne pas ouvrir le fichier\n");
        return NULL;
    }

    srand(time(NULL));
    random = (rand() % NB_LIGNES)+1;

    while (fgets(word, MAX_LENGTH, f) != NULL) {
        if (i == random) return word;
        i++;
    }

    fclose(f);
    return NULL;
}

/**
 * \fn void wordlize();
 * 
 * @brief Vérifie pour chaque caractère s'il est présent dans le mot et renvoie des indices pour chaque caractère
 * @param wordToValidate
 * @param word
*/
char *wordlize(char *word, char *wordToValidate) {

    int i, j;
    int wtvL = strlen(wordToValidate);
    char *wordlized = malloc(wtvL * sizeof(char));

    // Initialisation du tableau de réponse
    for(i = 0; i < wtvL; i++) {
        wordlized[i] = '.';
    }

    // Vérification des lettres
    for(i = 0 ; i < wtvL; i++) {
        if(word[i] == wordToValidate[i]) {
            wordlized[i] = '!';
        } else if(strchr(word, wordToValidate[i]) != NULL){
            for(j = 0; j < wtvL; j++) {
                if(word[j] == wordToValidate[i]) {
                    wordlized[i] = '?';
                }
            }
        }
    }
    return wordlized;
}

/**
 * \fn void printWord(char *word, char *wordlized);
 * 
 * @brief Affiche les caractères du mot en fonction du code de wordlized (vert => '!' ; rouge => '.' ; jaune => '?')
*/
int checkword(char *codeword) {
    long unsigned int i;
    for (i = 0; i < strlen(codeword) - 1; i++) {
        if (codeword[i] != '!') {
            return 0;
        }
    }
    return 1;
}