//
// Created by mathi on 04/10/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/token.h"
#include "../headers/parser.h"
#include "../headers/lexer.h"

#define BUFFER_SIZE 1024
#define SHUTTINGYARDSTRING_SIZE 1024

// Comprend le code pour l'éxecuter ensuite
void parser(char *input) {
    // crée la suite de token en fonction de l'input
    Token *token = NULL;
    token = lexer(input, token);

    // Si il y a bien des instructions
    if (token != NULL) {
        char **buffer = NULL;
        int bufferSize = 0;
        char *shuttingYardString = malloc(sizeof(char) * 1024);
        strcpy(shuttingYardString, "");
        // on copie le resultat de la fonction qui permet de l'avoir dans notre string
        strcpy(shuttingYardString, tokensToShuttingYardString(token, &buffer, &bufferSize, shuttingYardString));

        // verif
        printf("shuttingYardString : %s\n", shuttingYardString);

        // On free tout
        freeBuffer(buffer, bufferSize);
        free(shuttingYardString);
        freeToken(token);
    }
}

// Ajoute un élément dans le buffer
void pushBuffer(char ***buffer, int *bufferSize, char *value) {
    // Vérifie si le buffer n'est pas rempli
    if (*bufferSize >= BUFFER_SIZE) {
        printf("(push) Buffer is empty\n");
        return;
    }

    // Tableau temporaire pour ne pas gacher celui existant en cas d'erreur
    char **tempBuffer = realloc(*buffer, (*bufferSize + 1) * sizeof(char *));
    if (tempBuffer == NULL) {
        printf("realloc failed\n");
        return;
    }

    // Si pas d'erreurs :
    *buffer = tempBuffer;
    (*buffer)[*bufferSize] = malloc(sizeof(char) * (strlen(value) + 1));
    strcpy((*buffer)[*bufferSize], value);
    *bufferSize += 1;
}

// Enleve le dernier element ajouté du buffer
void popBuffer(char ***buffer, int *bufferSize) {
    // verifie si le buffer n'est pas vide
    if (*bufferSize <= 0) {
        printf("(pop) Buffer is empty\n");
        return;
    }

    // on libère la mémoire de l'emplacement puis retréci la taille
    free((*buffer)[*bufferSize - 1]);
    *bufferSize -= 1;
    *buffer = realloc(*buffer, *bufferSize * sizeof(char *));
}

// Affiche le buffer dans l'ordre pour l'utilisateur (le plus haut est le dernier ajouté)
void printBuffer(char **buffer, int bufferSize) {
    // verif si vide
    if (bufferSize <= 0) {
        printf("(print) Buffer is empty\n");
        return;;
    }

    // affiche dans le sens inverse
    for (int i = 0; i < bufferSize; i++) {
        printf("%s\n", buffer[bufferSize - i - 1]);
    }
}

// Libère la mémoire du buffer
void freeBuffer(char **buffer, int bufferSize) {
    // verif si buffer vide
    if (buffer == NULL) {
        printf("(free) Buffer is empty\n");
        return;
    }

    // libere les chaines de caractère
    for (int i = 0; i < bufferSize; i++) {
        free(buffer[i]);
    }

    // puis libere le buffer en lui même
    free(buffer);
}
