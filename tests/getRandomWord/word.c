#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "../../lib/session.h"

#define NB_LIGNES 9081
#define MAX_LENGTH 9

char *getRandomWord();
bool validateWord(char *word);
void testWord();
void wordlize(char *word, char *wordToValidate);

int main() {
    testWord();
    return 0;
}

void testWord() {
    char *word = getRandomWord();
    char wordToValidate[MAX_LENGTH];
    int nbEssai = 1;

    if(word == NULL) {
        printc(RED, "Erreur lors de la récupération du mot\n");
        return;
    }

    printf("DEBUG : Le mot aléatoire est : %s\n", word);
    printcf(BLUE, " ● Le mot est de %d lettres\n", strlen(word) - 1);

    while(strcmp(word, wordToValidate) != 0 && nbEssai < strlen(word)) {

        // on vide le wordToValidate pour éviter les erreurs
        memset(wordToValidate, 0, sizeof(wordToValidate));

        // On demande à l'utilisateur de rentrer un mot
        printf("\t| Veuillez entrer le mot : ");
        scanf("%s", wordToValidate);

        // On ajoute un retour chariot uniquement s'il n'y en a pas
        if(wordToValidate[strlen(wordToValidate)-1] != '\n') {
            wordToValidate[strlen(wordToValidate)] = '\n';
        }

            if (validateWord(wordToValidate) && strlen(wordToValidate) == strlen(word)) 
                wordlize(word, wordToValidate);
            else if(strlen(wordToValidate) != strlen(word)) 
                printc(RED, "\t| Le mot n'a pas le bon nombre de lettres !\n");
            else 
                printc(RED, "\t| Ce mot n'est pas dans notre dictionnaire\n");
            
        nbEssai++;
    }
    if(nbEssai == strlen(word)) printc(RED, "Vous avez épuisé tous vos essais !\n");
    else printc(GREEN, "Bravo ! Vous avez trouvé le mot !\n");
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
        printc(RED, "ERREUR : Ne pas ouvrir le fichier\n");
        return true;
    }

    while (fgets(wordInFile, MAX_LENGTH, f) != NULL) if (strcmp(wordInFile, word) == 0) return true;
    
    fclose(f);
    return false;
}

/**
 * \fn void wordlize();
 * 
 * @brief Vérifie pour chaque caractère s'il est présent dans le mot et renvoie des indices pour chaque caractère
 * @param wordToValidate
 * @param word
*/
void wordlize(char *word, char *wordToValidate) {

    int i, j;
    int wtvL = strlen(wordToValidate);
    char *wordlized = malloc(wtvL * sizeof(char));

    // Initialisation du tableau de réponse
    for(i = 0; i < wtvL; i++) {
        wordlized[i] = '.';
    }

    // Vérification des lettres
    for(i = 0 ; i < wtvL-1; i++) {
        if(word[i] == wordToValidate[i]) {
            wordlized[i] = '!';
        } else if(strchr(word, wordToValidate[i]) != NULL){
            for(j = 0; j < wtvL-1; j++) {
                if(word[j] == wordToValidate[i]) {
                    wordlized[i] = '?';
                }
            }
        }
    }

    // Affichage
    for(i = 0; i < wtvL; i++) printcf(BLUE, "\t%c ", wordToValidate[i]);
    for(i = 0; i < wtvL-1; i++) printcf(GREEN, "\t%c ", wordlized[i]);

    printf("\n");
    free(wordlized);

}
