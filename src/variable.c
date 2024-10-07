//
// Created by mathi on 07/10/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include "../headers/variable.h"

#include <string.h>

#include "../headers/lexer.h"

// addVariable
Var *addVariable(char *input, Var *headVar) {
    Token *token = NULL;
    token = lexer(input, token);
    printToken(token);
    if (token == NULL) {
        printf("Error in lexer\n");
        return NULL;
    }
    Var *newVar = malloc(sizeof(Var));
    int count = 0;
    int verifAssign = 0;
    int verifDot = 0;
    while (token->nextToken != NULL) {
        if (strcmp(getType(token->type), "IDENTIFIER") == 0 && count == 0) {
            newVar->name = malloc(sizeof(char) * strlen(token->value));
            strcpy(newVar->name, token->value);
        } else if (strcmp(getType(token->type), "IDENTIFIER") != 0 && count == 0) {
            printf("Error in assignment 1\n");
            freeVariable(newVar);
            return headVar;
        }

        if (strcmp(getType(token->type), "ASSIGN") == 0 && count == 1) {
            verifAssign++;
        } else if (strcmp(getType(token->type), "ASSIGN") == 0 && count != 1) {
            printf("Error in assignment 2\n");
            freeVariable(newVar);
            return headVar;
        }

        if (strcmp(getType(token->type), "QUOTES") == 0) {
            token = token->nextToken;
            char longString[255] = "";
            while (token->nextToken != NULL || strcmp(getType(token->type), "QUOTES") != 0) {
                strncat(longString, token->value, 255 - strlen(token->value) - 1);
            }
            if (token->nextToken == NULL && strcmp(getType(token->type), "QUOTES") != 0) {
                printf("Error in assignment 3\n");
                freeVariable(newVar);
                return headVar;
            }
            newVar->value = malloc(sizeof(char) * strlen(longString));
            strcpy(newVar->value, longString);
        }
        if (strcmp(getType(token->type), "NUMBER") == 0) {
            newVar->value = malloc(sizeof(char) * strlen(token->value));
            strcpy(newVar->value, token->value);
            token = token->nextToken;
            if (token != NULL && strcmp(getType(token->type), "DOT") != 0) {
                verifDot++;
                Token * nextToken = token->nextToken;
                newVar->type = DOUBLE;
                if(nextToken != NULL && strcmp(getType(nextToken->type), "NUMBER") != 0) {
                    strcat(newVar->value, token->value);
                    strcat(nextToken->value, token->value);
                }
            } else if (newVar == NULL) {
                newVar->type = INT;
            }
        }
        count++;
        token = token->nextToken;
    }
    if (verifDot > 1 || verifAssign != 1) {
        printf("Error in assignment 4\n");
        freeVariable(newVar);
        return headVar;
    }
    return headVar;
}

// getVariable
Var * getVariable(Var *var) {
    // Boucle qui fait des nextVariable tant qu'il est pas null et dès qu'il trouve le nom de la variable ça renvoi VAr
}

// printVariables
void printVariables(Var *var) {
    // Faire une fonction récursive qui affiche tant que var n'est pas NULL
}
// freeVariables
void freeVariable(Var *variable) {
    if (variable != NULL) {
        free(variable->name);
        free(variable->value);
        freeVariable(variable->nextVar);
        free(variable);
    }
}
