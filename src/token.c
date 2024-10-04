//
// Created by mathi on 04/10/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/token.h"
#include "../headers/lexer.h"

// Ajouter un token
Token * addToken(Token * token, const Type type, const char* value) {
    Token * newToken = malloc(sizeof(Token));
    newToken->type = type;
    newToken->value = malloc(sizeof(char) * (strlen(value) + 1));
    strcpy(newToken->value, value);
    newToken->nextToken = token;
    return newToken;
}

// Print un token à partir de la fin (pour les mettre dans l'ordre)
void printToken(Token * token) {
    if(token != NULL) {
        printToken(token->nextToken);
        printf("{type: \"%s\", value: \"%s\"},\n", getType(token->type) ,token->value);
    }
}

// Libère les tokens
void freeToken(Token * token) {
    if(token != NULL) {
        free(token->value);
        freeToken(token->nextToken);
        free(token);
    }
}