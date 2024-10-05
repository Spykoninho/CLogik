//
// Created by mathi on 04/10/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/token.h"
#include "../headers/lexer.h"
#include "../headers/parser.h"

// Ajouter un token
Token *addToken(Token *head, const Type type, const char *value) {
    // Crée le token
    Token *newToken = malloc(sizeof(Token));
    if (newToken == NULL) {
        return NULL;
    }

    // Met les valeurs
    newToken->type = type;
    newToken->value = malloc(sizeof(char) * (strlen(value) + 1));
    strcpy(newToken->value, value);
    newToken->nextToken = NULL; // met NULL car on va le mettre en dernier

    // si c'est le 1er de la liste on le return tout de suite
    if (head == NULL) {
        return newToken;
    }

    // on se prépare à parcourir la liste
    Token *temp = head;
    while (temp->nextToken != NULL) {
        temp = temp->nextToken;
    }
    // on est à la fin de la liste
    // on remplace l'ancien null par le nouveau token qui est maintenant à la fin
    temp->nextToken = newToken;

    // on return le même que celui d'entrée vu qu'on ne modifie que le dernier de la liste
    return head;
}

// Print les tokens dans l'ordre (déjà trié lors de l'ajout)
void printToken(Token *token) {
    if (token != NULL) {
        printf("{type: \"%s\", value: \"%s\"},\n", getType(token->type), token->value);
        printToken(token->nextToken);
    }
}

// Converti la liste de token en string shuttingyard
char *tokensToShuttingYardString(Token *token, char ***buffer, int *bufferSize, char *shuttingYardString) {
    // Si il y a des tokens
    if (token != NULL) {
        // Si la valeur est un nombre on le met directement dans l'output
        if (strcmp(getType(token->type), "NUMBER") == 0) {
            // Si c'est le premier ajout on fait un copy car strcat se fait que sur une chaine existante
            if (*bufferSize == 0) {
                strncpy(shuttingYardString, token->value, 1024 - strlen(token->value) - 1);
                shuttingYardString[1024 - strlen(token->value)] = '\0';
            } else {
                strncat(shuttingYardString, token->value, 1024 - strlen(token->value) - 1);
            }
        }
        // si c'est un opérateur on le met dans le buffer
        if (strcmp(getType(token->type), "OPERATOR") == 0) {
            pushBuffer(buffer, bufferSize, token->value);
        }
        // on passe au token suivant
        tokensToShuttingYardString(token->nextToken, buffer, bufferSize, shuttingYardString);
        return shuttingYardString; // on return la string
    }
    // Une fois qu'on à tout fait sur la liste on rajoute à la fin les opérateurs
    for (int i = 0; i < *bufferSize; ++i) {
        strcat(shuttingYardString, (*buffer)[*bufferSize - i - 1]);
    }
    return NULL;
}

// Libère les tokens
void freeToken(Token *token) {
    if (token != NULL) {
        free(token->value);
        freeToken(token->nextToken);
        free(token);
    }
}
