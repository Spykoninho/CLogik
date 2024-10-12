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
    // Conversion de l'input en tokens
    token = lexer(input, token);
    if (token == NULL) {
        printf("Erreur dans a récupération des tokens\n");
        return NULL;
    }

    // Initialisation de la variable
    Var *newVar = malloc(sizeof(Var));
    if (newVar == NULL) {
        printf("Erreur pour l'allocation en mémoire de la variable\n");
        return NULL;
    }

    // Assignation du nom de la variable
    if (strcmp(getType(token->type), "IDENTIFIER") == 0) {
        newVar->name = malloc(sizeof(char) * strlen(token->value) + 1);
        if (newVar->name == NULL) {
            printf("Erreur pour l'allocation en mémoire du nom de la variable\n");
            freeVariable(newVar);
            return headVar;
        }
        strcpy(newVar->name, token->value);
        token = token->nextToken;
    } else {
        printf("Erreur de format pour initialiser la variable 1\n");
        freeVariable(newVar);
        return headVar;
    }

    // verification du =
    if (strcmp(getType(token->type), "ASSIGN") != 0) {
        printf("Erreur de format pour initialiser la variable 2\n");
        free(newVar->name);
        return headVar;
    }
    token = token->nextToken; // on saute l'espace

    // on verif s'il y a un calcul dans l'operation
    int isCalcul = 0;
    int isDot = 0;
    Token *temp = token;
    while (temp != NULL) {
        if (isOperator(temp->type)) {
            isCalcul = 1;
        }
        if (strcmp(getType(temp->type), "DOT") == 0) {
            isDot = 1;
        }
        temp = temp->nextToken;
    }

    // POUR STRING
    if (token != NULL && strcmp(getType(token->type), "QUOTES") == 0) {
        newVar->type = STRING;
        token = token->nextToken;

        char *longString = malloc(sizeof(char) * 255);
        if (longString == NULL) {
            printf("Erreur pour l'allocation en mémoire du nom de la variable\n");
            freeVariable(newVar);
            return headVar;
        }
        longString[0] = '\0';
        int countForSpace = 0;
        while (token != NULL && strcmp(getType(token->type), "QUOTES") != 0) {
            strncat(longString, token->value, 255 - strlen(longString));
            token = token->nextToken;
            if (countForSpace % 2 == 0) {
                strcat(longString, " ");
                continue;
            }
            countForSpace++;
        }
        if (token == NULL || strcmp(getType(token->type), "QUOTES") != 0) {
            printf("Erreur de format pour initialiser la variable 3\n");
            freeVariable(newVar);
            free(longString);
            return headVar;
        }
        newVar->value = malloc(sizeof(char) * strlen(longString) + 1);
        if (newVar->value == NULL) {
            printf("Erreur pour l'allocation en mémoire de la valeur de la variable\n");
            free(longString);
            freeVariable(newVar);
            return headVar;
        }
        strcpy(newVar->value, longString);
        free(longString);

    } else if (isCalcul) { // Si c'est un calcul
        if (isDot) {
            newVar->type = DOUBLE;
            double result = 0;
            while (token != NULL) {
                token = token->nextToken;
            }
        }else {

            newVar->type = INT;
            int result = 0;
        }
    }else if (token != NULL && strcmp(getType(token->type), "NUMBER") == 0) { // Pour les nombres
        Token *tempToken = token->nextToken;
        if (tempToken != NULL && strcmp(getType(tempToken->type), "DOT") == 0) {
            // Il y a un point, donc on traite ça comme un double
            tempToken = tempToken->nextToken;
            newVar->type = DOUBLE;

            // Si un nombre suit le point, on le concatène, sinon on ajoute "0"
            if (tempToken != NULL && strcmp(getType(tempToken->type), "NUMBER") == 0) {
                newVar->value = malloc(sizeof(char) * strlen(token->value) + 2 + strlen(tempToken->value) + 1); // pour le . le \0 et le chiffre après la virgule
                if (newVar->value == NULL) {
                    printf("Erreur pour l'allocation en mémoire de la valeur de la variable\n");
                    freeVariable(newVar);
                    return headVar;
                }
                strcpy(newVar->value, token->value);
                strcat(newVar->value, ".");
                strcat(newVar->value, tempToken->value);
            }else {
                newVar->value = malloc(sizeof(char) * strlen(token->value) + 1);
                strcpy(newVar->value, token->value);
            }
        } else {
            newVar->type = INT;
            newVar->value = malloc(sizeof(char) * strlen(token->value) + 1);
            if (newVar->value == NULL) {
                printf("Erreur pour l'allocation en mémoire de la valeur de la variable\n");
                freeVariable(newVar);
                return headVar;
            }
            strcpy(newVar->value, token->value);
        }
    }

    // ajout de la variable à la fin
    newVar->nextVar = NULL;
    if (headVar == NULL) {
        return newVar;
    }
    Var *lastVar = headVar;
    while (lastVar->nextVar != NULL) {
        lastVar = lastVar->nextVar;
    }
    lastVar->nextVar = newVar;

    return headVar;
}

// getVariable
Var *getVariable(Var *var, char *searchedVar) {
    while (var != NULL) {
        if (strcmp(var->name, searchedVar) == 0) {
            return var;
        }
        var = var->nextVar;
    }
    printf("La variable %s n'existe pas.", searchedVar);
    return NULL;
}

// printVariables
void printVariables(Var *var) {
    printf("Variables enregistrees :\n");
    while (var != NULL) {
        printf("%s %s %s\n", getVarType(var->type), var->name, var->value);
        var = var->nextVar;
    }
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

int getListLength(Token *token) {
    if (token == NULL) return 0;
    int count = 0;
    while (token != NULL) {
        count++;
        token = token->nextToken;
    }
    return count;
}

int isCalcul(Token *token) {
    if (token == NULL) return 0;
    int count = 0;
    while (token != NULL) {
        count++;
        token = token->nextToken;
    }
    return count;
}

char *getVarType(int type) {
    switch (type) {
        case STRING:
            return "STRING";
        case INT:
            return "INT";
        case DOUBLE:
            return "DOUBLE";
        default:
            return "UNKNOWN";
    }
}
