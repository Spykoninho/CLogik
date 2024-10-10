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
    printToken(token);
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

    int count = 0;
    int verifDot = 0;

    while (token != NULL) {
        // NOM DE LA VARIABLE
        if (strcmp(getType(token->type), "IDENTIFIER") == 0 && count == 0) {
            newVar->name = malloc(sizeof(char) * strlen(token->value)+1);
            if (newVar->name == NULL) {
                printf("Erreur pour l'allocation en mémoire du nom de la variable\n");
                freeVariable(newVar);
                return headVar;
            }
            strcpy(newVar->name, token->value);
            token = token->nextToken;
            count++;
            continue;
        }

        // VERIF DU =
        if (strcmp(getType(token->type), "ASSIGN") == 0 && count != 1) {
            printf("Erreur de format pour initialiser la variable\n");
            free(newVar->name);
            return headVar;
        }

        // POUR STRING
        if (strcmp(getType(token->type), "QUOTES") == 0) {
            newVar->type=STRING;
            token = token->nextToken;

            char * longString = malloc(sizeof(char)*255);
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
                if(countForSpace % 2 == 0) {
                    strcat(longString, " ");
                    continue;
                }
                countForSpace++;
            }
            if (token == NULL || strcmp(getType(token->type), "QUOTES") != 0) {
                printf("Erreur de format pour initialiser la variable\n");
                freeVariable(newVar);
                free(longString);
                return headVar;
            }
            newVar->value = malloc(sizeof(char) * strlen(longString)+1);
            if (newVar->value == NULL) {
                printf("Erreur pour l'allocation en mémoire de la valeur de la variable\n");
                free(longString);
                freeVariable(newVar);
                return headVar;
            }
            strcpy(newVar->value, longString);
            free(longString);
            token = token->nextToken;
            count++;
            continue;
        }

        if (strcmp(getType(token->type), "NUMBER") == 0) {
            newVar->value = malloc(sizeof(char) * strlen(token->value)+1);
            if (newVar->value == NULL) {
                printf("Erreur pour l'allocation en mémoire de la valeur de la variable\n");
                freeVariable(newVar);
                return headVar;
            }
            strcpy(newVar->value, token->value);
            Token * temp = token->nextToken;
            if (temp != NULL && strcmp(getType(temp->type), "DOT") == 0) {
                // Il y a un point, donc on traite ça comme un double
                verifDot++;
                strcat(newVar->value, ".");
                temp = temp->nextToken;
                newVar->type = DOUBLE;

                // Si un nombre suit le point, on le concatène, sinon on ajoute "0"
                if (temp != NULL && strcmp(getType(temp->type), "NUMBER") == 0) {
                    strcat(newVar->value, temp->value);
                    token = temp->nextToken;
                } else {
                    // Cas où il n'y a pas de chiffre après le point (e.g. 12.)
                    strcat(newVar->value, "0");
                    token = token->nextToken; // Avancer après le point
                }
            } else {
                newVar->type = INT;
            }
            token = token->nextToken;
            count++;
            continue;
        }


        // Espace réservé au reste
        if (strcmp(getType(token->type), "IDENTIFIER") == 0) {
            // Voir pour mettre un type calcul ou quoi pour indiquer qu'il y a une var dedans pour ensuite faire une fonction qui calcul avec la variable
        }

        count++;
        token = token->nextToken;
    }

    if (verifDot > 1) {
        printf("Erreur de format pour initialiser la variable 1\n");
        freeVariable(newVar);
        return headVar;
    }

    newVar->nextVar = NULL;
    if(headVar == NULL) {
        return newVar;
    }
    Var * temp = headVar;
    while (temp->nextVar != NULL) {
        temp = temp->nextVar;
    }
    temp->nextVar = newVar;

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
        var = var->nextVar;}
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

char *getVarType(int type) {
    switch (type) {
        case STRING:
            return "STRING";
        case INT:
            return "INT";
        case DOUBLE:
            return "DOUBLE";
        case CALCUL:
            return "CALCUL";
        default:
            return "UNKNOWN";
    }
}
