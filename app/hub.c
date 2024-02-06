
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
                serveurLobby(nbLobby);
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
 * @fn void serveurLobby();
 * 
 * @brief Lance un lobby
 * @param idLobby Emplacement du lobby dans le tableau
*/
void serveurLobby(int idLobby) {
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

    received_t recData;

    while (1)
    {
        sockPlayer = accepterConnexion(sock);
        printf(YELLOW "[%s]"  RESET " %d se connecte...\n", tabLobby[idLobby].code, sockPlayer.port);

        int pidPlayer;
        CHECK(pidPlayer = fork(), "fork()");

        if(pidPlayer == 0) {
            fermerConnexion(sock);
            // Fils
            recevoirSuivant(sockPlayer, &recData, deserial);
            int idPlayerInLobby = recognizePlayer(idLobby, sockPlayer.ip, sockPlayer.port);
            printf(YELLOW "[%s]"  RESET " %d (id: %d) a rejoint le lobby\n", tabLobby[idLobby].code, sockPlayer.port, idPlayerInLobby);

            // Envoi de confirmation de connexion au Lobby
            send_t sendData;
            sendData.code = 202;
            sendData.nbArgs = 1;
            if (idPlayerInLobby==0)
                sendData.args[0] = "1"; // si le user est le chef 
            else 
                sendData.args[0] = "0"; 
            

            envoyer(sockPlayer, &sendData, serial);

            while(1);
        }
    }

    fermerConnexion(sock);
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
    strcpy(tabLobby[idLobby].players[tabLobby[idLobby].playerCount].ip, ip);
    tabLobby[idLobby].players[tabLobby[idLobby].playerCount].port = port;
    tabLobby[idLobby].players[tabLobby[idLobby].playerCount].lobbyHost = 0;  

    if (tabLobby[idLobby].playerCount ==0)
        tabLobby[idLobby].players[tabLobby[idLobby].playerCount].lobbyHost = 1 ;

    tabLobby[idLobby].players[tabLobby[idLobby].playerCount].pidPlayer = getpid();
    tabLobby[idLobby].playerCount++;
    return tabLobby[idLobby].playerCount - 1;
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
 * @fn      void installerGestionSignal(int sigNum, void (*handler)(int))
 * @brief   Installer le traitement handler pour un déclenchement sur occurence du signal sigNum
 * @param   sigNum : Numéro du signal déclencheur
 * @param   handler : Nom de la fonction de traitement du signal sigNum
 * @note    handler peut valoir SIG_DFL (traitement par défaut) ou SIG_IGN (pour ignorer le signal)
 */
void installerGestionSignal(int sigNum, void (*handler)(int)) {
    // Gestion des signaux
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    CHECK(sigaction(sigNum, &action, NULL), "__sigaction__");
}

/*      ****    FCTS GESTION SIGNAUX    *** Q3  *** _________________________*/
/**
 * @fn      void traiterSignal(int sigNum)
 * @brief   Traitement du signal sigNum
 * @param   sigNum : Numéro du signal déclencheur
 * @note    Signaux implémentés : SIGALRM
 */
void traiterSignal(int sigNum) {
    switch (sigNum) {
        case SIGALRM:
            printf("Signal SIGALRM reçu\n");
            break;
        default:
            printf("Signal %d reçu\n", sigNum);
            break;
    }
}