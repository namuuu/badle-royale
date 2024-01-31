#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#define NB_LIGNES 21074
#define MAX_LENGTH 20

char *getRandomWord();
bool validateWord(char *word);
void testWord();

int main() {
    testWord();
    return 0;
}

void testWord() {
    char *word = getRandomWord();
    char wordToValidate[MAX_LENGTH];
    if(word == NULL) {
        printf("Erreur lors de la récupération du mot\n");
        return;
    }
    printf("Le mot était %s", word);

    printf("Veuillez entrer le mot : ");
    scanf("%s", wordToValidate);

    wordToValidate[strlen(wordToValidate)] = '\n';

    if(validateWord(wordToValidate)) {
        printf("Le mot est valide\n");
    } else {
        printf("Le mot n'est pas valide\n");
    }

    free(word);
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

    f = fopen("dico.txt", "r");
    if (f == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        return NULL;
    }

    srand(time(NULL));
    random = (rand() % NB_LIGNES)+1;

    while (fgets(word, MAX_LENGTH, f) != NULL) {
        if (i == random) {
            return word;
        }
        i++;
    }

    fclose(f);
}

/**
 * \fn bool validateWord(char *word);
 * 
 * @brief Vérifie si le mot est valide, c'est-à-dire s'il est présent dans le fichier dico.txt
 * @param word Mot à vérifier
 * @return true si le mot est valide
 * @return false si le mot n'est pas valide
*/
bool validateWord(char *word) {
    FILE *f;
    char *wordInFile = malloc(MAX_LENGTH * sizeof(char));

    //printf("DEBUG MOT ENTREE: %s\n", word);

    f = fopen("dico.txt", "r");
    if (f == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        return true;
    }

    while (fgets(wordInFile, MAX_LENGTH, f) != NULL) {
        if (strcmp(wordInFile, word) == 0 && strlen(wordInFile) == strlen(word)) {
            return true;
        }
    }

    fclose(f);
    return false;
}
