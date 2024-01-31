#include <stdio.h>
#include <stdlib.h>


char *getRandomWord();

int main() {
    char *word = getRandomWord();
    printf("Le mot était %s", word);
    free(word);
    return 0;
}


/**
 * \fn char *getRandomWord();
 * 
 * @brief Récupère un mot aléatoire dans le fichier words.txt
 * @return Mot aléatoire
 * @return NULL si le fichier n'a pas pu être ouvert
*/
char *getRandomWord() {
    FILE *f; 
    char *word = malloc(20 * sizeof(char));
    int i = 0;
    int rand = 3;

    f = fopen("dico.txt", "r");
    if (f == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        return NULL;
    }

    while (fgets(word, 20, f) != NULL) {
        if (i == rand) {
            return word;
        }
        i++;
    }

}