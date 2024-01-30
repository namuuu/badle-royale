
#include "hub.h"

int main(int argc, char *argv[]){
    system("clear");
   
    if(argc != 3) { 
        printf(RED);
        printf("Usage: %s <ip> <port>\n", argv[0]);
        printf(RESET);
        return -1;
    }
    serveur(argv[1], atoi(argv[2]));


    int fd; // file descriptor
    fd = shm_open("tabLobby", O_CREAT | O_RDWR, S_IRWXU);  
    // Set the size of the shared memory object
    int pageSize = sysconf(_SC_PAGE_SIZE);
    CHECK(ftruncate(fd, pageSize), "__ftruncate__");
    // Map the tabEtats object into the virtual address space of the calling process
    tabLobby = mmap(0, pageSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
   
    return 0; 
}

/**
 * @fn void serveur(char *ip, short port);
 * 
 * @brief Lance un serveur HUB
 * @param ip Fournit l'ip du serveur HUB
 * @param port Fournit le port du serveur HUB
*/
void serveur(char *ip, short port) {
    printc(BOLDYELLOW, "Lancement du HUB\n");
    printc(YELLOW, "| IP: ");
    printf("%s\n", ip);
    printc(YELLOW, "| Port: ");
    printf("%d\n", port);

    received_t data;

    while(1) {
        socket_t sock = prepareForClient(ip, port, SOCK_STREAM);
        waitForInput(sock, &data);

        switch (data.code)
        {
        case 100: // joinLobby - code
            printc(RED, "Requête joinLobby non implémentée\n");
            break;
        case 101: // createLobby
            //creation d'un lobby
            int pidLobby;
            CHECK(pidLobby = fork(), "fork()");
            if(pidLobby == 0) {
                // Fils
                shutdown(sock.fd, SHUT_RDWR);
                serveurLobby();
                exit(EXIT_SUCCESS);
            } else {
                // Père, attends l'update de la création du Lobby
                while(1);
            }

            break;
        default:
            break;
        }

        close(sock.fd);
    }
    
}

/**
 * @fn void serveurLobby();
 * 
 * @brief Lance un lobby
 * @param ip Fournit l'ip du lubby
 * @param port Fournit le port du lobby
*/
void serveurLobby() {
    // Préparation de la socket
    char* ip = "0.0.0.0";
    int port = 0;
    socket_t sock = prepareForClient(ip, port, SOCK_STREAM);

    socklen_t len = sizeof(sock.addr);
    CHECK(getsockname(sock.fd, (struct sockaddr *)&sock.addr, &len), "getsockname()");
    port = ntohs(sock.addr.sin_port);

    // Envoi de l'ip et du port au client
    printf(BOLDGREEN "Lancement du Lobby\n");
    printf(GREEN "| IP: " RESET "%s\n", ip);
    printf(GREEN "| Port: " RESET "%d\n", port);

    char *msg = NULL;   
    waitForInput(sock, msg);
}

void waitForInput(socket_t sock, generic msg){
    printf("En attente de connexion\n");
    recevoir(sock, msg, deserial);
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