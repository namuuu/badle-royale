

int main(int argc, char *argv[])
{
    #ifdef CLIENT
        printf("Client\n");
    #endif
    #ifdef SERVEUR
        printf("Serveur\n");
        int x = 1;
    #endif
    #if !defined(CLIENT) && !defined(SERVEUR)
        printf("Erreur: ni client ni serveur\n");
    #endif
    return 0; 
}