/**
 * @file data.h
 * @author Tomas TRENY
 * @brief Data class
 * @version 1.0
 * @date 2024-01-12
 * 
*/

/* INCLUDE */

/* DEFINITIONS */
#define MAX_BUFF 1024
typedef char buffer_t[MAX_BUFF];
typedef void * generic;
typedef void (*pFct)(generic, generic);

/* DECLARATIONS DE FONCTIONS */
void envoyer(int sock, generic quoi, pFct serial);

void recevoir(int sock, generic quoi, pFct deSerial);
