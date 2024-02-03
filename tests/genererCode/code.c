#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

char *generateLobbyCode();
void genererCode(FILE* fichier, char code[6]);
void suppressionCode(const char *code);
void wordlize();

int main(int argc, char *argv[]) {
    char *code = generateLobbyCode();
    suppressionCode(code);
    free(code);
    return 0;
}

/**
 * \fn char *generateLobbyCode();
 * 
 * @brief Génération d'un code de session
*/
char *generateLobbyCode() {
    FILE* fichier;
    char *code = malloc(6 * sizeof(char));
    char codeLu[6];

    srand(time(NULL));
    fichier = fopen("code.txt", "a+");

    if(fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        exit(1);
    }

    genererCode(fichier, code);

    while(fgets(codeLu, 6, fichier) != NULL) {
        if(strcmp(codeLu, code) == 0) {
            genererCode(fichier, code);
        }
    }

    printf("Le code de votre session est : %s\n", code);
    fprintf(fichier, "%s\n", code);

    fclose(fichier);

    return code;
}

/**
 * \fn void genererCode(FILE* fichier, char code[6]);
 * 
 * @brief Génération d'un code aléatoire
 * @param fichier Nom du fichier à modifier
 * @param code Code à ajouter au fichier
*/
void genererCode(FILE *fichier, char code[6]) {
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
