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
    printTokens(token);
    if (token == NULL) {
        printf("Erreur dans a récupération des tokens\n");
        freeVariable(headVar);
        exit(0);
    }

    Var *newVar = NULL;
    // Assignation du nom de la variable
    if (strcmp(getType(token->type), "IDENTIFIER") == 0) {
        if (isVarExists(headVar, token->value)) {
            newVar = getVariable(headVar, token->value);
        } else {
            // Initialisation de la variable
            newVar = malloc(sizeof(Var));
            if (newVar == NULL) {
                printf("Erreur pour l'allocation en mémoire de la variable\n");
                freeVariable(headVar);
                exit(0);
            }
            newVar->name = malloc(sizeof(char) * strlen(token->value) + 1);
            if (newVar->name == NULL) {
                printf("Erreur pour l'allocation en mémoire du nom de la variable\n");
                freeVariable(headVar);
                freeVariable(newVar);
                return headVar;
            }
            strcpy(newVar->name, token->value);
            token = token->nextToken;
        }
    } else {
        printf("Erreur de format pour initialiser la variable 1\n");
        freeVariable(headVar);
        freeVariable(newVar);
        exit(0);
    }

    // verification du =
    if (strcmp(getType(token->type), "ASSIGN") != 0) {
        printf("Erreur de format pour initialiser la variable 2\n");
        freeVariable(newVar);
        freeVariable(headVar);
        exit(0);
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

    if (isCalcul) {
        // Si c'est un calcul
        if (token != NULL && strcmp(getType(token->type), "QUOTES") == 0) {
            newVar->type = STRING;
            token = token->nextToken; // on saute la première "

            char *longString = malloc(sizeof(char) * 255);
            if (longString == NULL) {
                printf("Erreur d'allocation en mémoire\n");
                freeVariable(newVar);
                freeVariable(headVar);
                exit(0);
            }
            longString[0] = '\0';

            // Ajout de la première partie de la chaîne
            while (token != NULL && strcmp(getType(token->type), "QUOTES") != 0) {
                strncat(longString, token->value, 255 - strlen(longString));
                token = token->nextToken;
            }

            // Vérification du + pour concaténer d'autres chaînes
            while (token != NULL && strcmp(getType(token->type), "PLUS") == 0) {
                token = token->nextToken; // On saute le +

                // Ajouter un espace avant de concaténer la prochaine chaîne
                strcat(longString, " ");

                if (token != NULL && strcmp(getType(token->type), "QUOTES") == 0) {
                    token = token->nextToken; // On saute la première "

                    while (token != NULL && strcmp(getType(token->type), "QUOTES") != 0) {
                        strncat(longString, token->value, 255 - strlen(longString));
                        token = token->nextToken;
                    }
                    token = token->nextToken; // On saute la fermeture des guillemets
                }
            }

            // Affectation du résultat à la variable
            newVar->value = malloc(sizeof(char) * strlen(longString) + 1);
            if (newVar->value == NULL) {
                printf("Erreur pour l'allocation en mémoire\n");
                free(longString);
                freeVariable(newVar);
                freeVariable(headVar);
                exit(0);
            }
            strcpy(newVar->value, longString);
            free(longString);
        } else {
            Token *tempTokenCalcul = token;
            while (tempTokenCalcul != NULL) {
                if (strcmp(getType(tempTokenCalcul->type), "IDENTIFIER") == 0) {
                    if (isVarExists(headVar, tempTokenCalcul->value)) {
                        Var *var = getVariable(headVar, tempTokenCalcul->value);
                        tempTokenCalcul->value = realloc(tempTokenCalcul->value, sizeof(char) * strlen(var->value) + 1);
                        strcpy(tempTokenCalcul->value, var->value);
                        tempTokenCalcul->type = NUMBER;
                    } else {
                        printf("La variable %s n'existe pas !\n", tempTokenCalcul->value);
                        freeVariable(headVar);
                        freeVariable(newVar);
                        exit(0);
                    }
                }
                tempTokenCalcul = tempTokenCalcul->nextToken;
            }
            double result = calcul(token);
            char *resultString = malloc(sizeof(char) * 100);
            if (isDot) {
                newVar->type = DOUBLE;
                sprintf(resultString, "%lf", result);
            } else {
                newVar->type = INT;
                sprintf(resultString, "%d", (int) result);
            }
            newVar->value = malloc(sizeof(char) * strlen(resultString) + 1);
            strcpy(newVar->value, resultString);
        }
    } else if (token != NULL && strcmp(getType(token->type), "QUOTES") == 0) {
        // POUR STRING
        newVar->type = STRING;
        token = token->nextToken;

        char *longString = malloc(sizeof(char) * 255);
        if (longString == NULL) {
            printf("Erreur pour l'allocation en mémoire du nom de la variable\n");
            freeVariable(newVar);
            freeVariable(headVar);
            exit(0);
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
            freeVariable(headVar);
            free(longString);
            exit(0);
        }
        newVar->value = malloc(sizeof(char) * strlen(longString) + 1);
        if (newVar->value == NULL) {
            printf("Erreur pour l'allocation en mémoire de la valeur de la variable\n");
            free(longString);
            freeVariable(newVar);
            freeVariable(headVar);
            exit(0);
        }
        strcpy(newVar->value, longString);
        free(longString);
    } else if (token != NULL && strcmp(getType(token->type), "NUMBER") == 0) {
        // Pour les nombres
        Token *tempToken = token->nextToken;
        if (tempToken != NULL && strcmp(getType(tempToken->type), "DOT") == 0) {
            // Il y a un point, donc on traite ça comme un double
            tempToken = tempToken->nextToken;
            newVar->type = DOUBLE;

            // Si un nombre suit le point, on le concatène, sinon on ajoute "0"
            if (tempToken != NULL && strcmp(getType(tempToken->type), "NUMBER") == 0) {
                newVar->value = malloc(sizeof(char) * strlen(token->value) + 2 + strlen(tempToken->value) + 1);
                // pour le . le \0 et le chiffre après la virgule
                if (newVar->value == NULL) {
                    printf("Erreur pour l'allocation en mémoire de la valeur de la variable\n");
                    freeVariable(newVar);
                    freeVariable(headVar);
                    exit(0);
                }
                strcpy(newVar->value, token->value);
                strcat(newVar->value, ".");
                strcat(newVar->value, tempToken->value);
            } else {
                newVar->value = malloc(sizeof(char) * strlen(token->value) + 1);
                strcpy(newVar->value, token->value);
            }
        } else {
            newVar->type = INT;
            newVar->value = malloc(sizeof(char) * strlen(token->value) + 1);
            if (newVar->value == NULL) {
                printf("Erreur pour l'allocation en mémoire de la valeur de la variable\n");
                freeVariable(newVar);
                exit(0);
            }
            strcpy(newVar->value, token->value);
        }
    } else if (token != NULL && strcmp(getType(token->type), "IDENTIFIER") == 0) {
        if (isVarExists(headVar, token->value)) {
            newVar->value = malloc(sizeof(char) * strlen(token->value) + 1);
            strcpy(newVar->value, getVariable(headVar, token->value)->value);
            newVar->type = getVariable(headVar, token->value)->type;
        } else {
            printf("La variable %s n'existe pas !\n", token->value);
            freeVariable(newVar);
            freeVariable(headVar);
            exit(0);
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
    freeVariable(var);
    exit(0);
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

int isVarExists(Var *var, const char *searchedVar) {
    while (var != NULL) {
        if (strcmp(var->name, searchedVar) == 0) {
            return 1;
        }
        var = var->nextVar;
    }
    return 0;
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
