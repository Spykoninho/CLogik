//
// Created by mathi on 07/10/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include "../headers/variable.h"
#include <string.h>
#include "../headers/lexer.h"

// Ajouter une variable dans la liste headVar qui contient toutes les variables
Var *addVariable(Token *token, Var *headVar) {
    // On vérifie que le lexer a bien renvoyé des tokens
    if (token == NULL) {
        printf("Erreur dans la récupération des tokens\n");
        freeVariable(headVar);
        exit(1);
    }

    // On initialise la nouvelle variable
    Var *newVar = NULL;
    int isVarExist = 0;

    // Assignation du nom de la variable
    if (strcmp(getType(token->type), "IDENTIFIER") == 0) {
        // Si la variable appelée existe, newVar devient la variable
        if (isVarExists(headVar, token->value)) {
            newVar = getVariable(headVar, token->value);
            isVarExist = 1;
        } else {
            // Allocation de la mémoire pour la nouvelle variable
            newVar = malloc(sizeof(Var));
            if (newVar == NULL) {
                printf("Erreur d'allocation en mémoire pour la variable\n");
                freeVariable(headVar);
                exit(1);
            }

            newVar->name = malloc(strlen(token->value) + 1);
            if (newVar->name == NULL) {
                printf("Erreur d'allocation en mémoire pour le nom de la variable\n");
                free(newVar);
                freeVariable(headVar);
                exit(1);
            }

            // On met le premier token comme nom de la variable
            strcpy(newVar->name, token->value);
        }
        // On passe au =
        token = token->nextToken;
    } else {
        printf("Erreur : format incorrect pour l'initialisation de la variable\n");
        freeVariable(headVar);
        free(newVar);
        exit(1);
    }

    // Vérification de l'opérateur '='
    if (strcmp(getType(token->type), "ASSIGN") != 0) {
        printf("Erreur : format incorrect, attendu '=' pour l'assignation\n");
        freeVariable(newVar);
        freeVariable(headVar);
        exit(1);
    }

    // On passe à la valeur de la variable (calcul, string, nombre ou variable)
    token = token->nextToken;

    // Vérification s'il y a un calcul ou une simple assignation
    int isCalcul = 0;
    int isString = 0;
    Token *temp = token;
    while (temp != NULL) {
        if (strcmp(getType(temp->type), "TOKENSTRING") == 0) isString++;
        if (isOperator(temp->type)) isCalcul = 1;
        temp = temp->nextToken;
    }
    freeTokens(temp);

    // Si on détecte des opérateurs on déclenche le calcul
    if (isCalcul) {
        // on verif si il y a une variable qui est une string
        int isVarString = 0;
        if (strcmp(getType(token->type), "IDENTIFIER") == 0) {
            if (isVarExists(headVar, token->value)) {
                Var *checkVar = getVariable(headVar, token->value);
                if (strcmp(getVarType(checkVar->type), "STRING") == 0) isVarString = 1;
            } else {
                printf("Erreur : La variable n'existe pas\n");
                freeVariable(headVar);
                free(newVar);
                exit(1);
            }
        }

        // Traitement des chaînes de caractères
        if (strcmp(getType(token->type), "TOKENSTRING") == 0 || isVarString) {
            newVar->type = STRING;

            char *longString = malloc(255 * sizeof(char));
            if (longString == NULL) {
                printf("Erreur d'allocation de memoire\n");
                freeVariable(newVar);
                freeVariable(headVar);
                exit(1);
            }
            longString[0] = '\0';

            if (isVarString) {
                Var *var = getVariable(headVar, token->value);
                strncat(longString, var->value, 255 - strlen(longString));
            } else {
                strncat(longString, token->value, 255 - strlen(longString));
            }
            token = token->nextToken;

            // Traitement des concaténations
            while (token != NULL && strcmp(getType(token->type), "PLUS") == 0) {
                token = token->nextToken; // Sauter le '+'

                if (strcmp(getType(token->type), "TOKENSTRING") == 0) {
                    strncat(longString, token->value, 255 - strlen(longString));
                } else if (strcmp(getType(token->type), "IDENTIFIER") == 0) {
                    Var *var = getVariable(headVar, token->value);
                    if (strcmp(getVarType(var->type), "STRING") != 0) {
                        printf("Erreur : la concatenation doit être avec une chaîne de caractères\n");
                        free(longString);
                        freeVariable(newVar);
                        freeVariable(headVar);
                        exit(1);
                    }
                    strncat(longString, var->value, 255 - strlen(longString));
                } else {
                    printf("Erreur : concatenation invalide\n");
                    free(longString);
                    freeVariable(newVar);
                    freeVariable(headVar);
                    exit(1);
                }
                token = token->nextToken;
            }

            newVar->value = malloc(strlen(longString) + 1);
            if (newVar->value == NULL) {
                printf("Erreur d'allocation mémoire\n");
                free(longString);
                freeVariable(newVar);
                freeVariable(headVar);
                exit(1);
            }
            strcpy(newVar->value, longString);
            free(longString);
        } else {
            // Traitement des calculs avec des nombres
            Token *tempTokenCalcul = token;
            int isDouble = 0;
            while (tempTokenCalcul != NULL) {
                if (strcmp(getType(tempTokenCalcul->type), "IDENTIFIER") == 0) {
                    if (isVarExists(headVar, tempTokenCalcul->value)) {
                        const Var *var = getVariable(headVar, tempTokenCalcul->value);
                        // On prend le token de la variable et on regarde si c'est un double
                        char *tempRealloc = realloc(tempTokenCalcul->value, sizeof(char) * (strlen(var->value) + 1));
                        if (tempRealloc == NULL) {
                            printf("Erreur de réallocation de mémoire pour tempTokenCalcul->value\n");
                            freeVariable(headVar);
                            freeTokens(token);
                            exit(1);
                        }
                        // nouvelle taille pour la value
                        tempTokenCalcul->value = tempRealloc;
                        strcpy(tempTokenCalcul->value, var->value);
                        if (isTokenDouble(var->value)) {
                            isDouble = 1;
                        }
                        tempTokenCalcul->type = NUMBER;
                    } else {
                        printf("Erreur : variable '%s' inexistante\n", tempTokenCalcul->value);
                        freeVariable(headVar);
                        exit(1);
                    }
                } else if (strcmp(getType(tempTokenCalcul->type), "NUMBER") == 0) {
                    if (isTokenDouble(tempTokenCalcul->value)) isDouble = 1;
                }
                tempTokenCalcul = tempTokenCalcul->nextToken;
            }
            double result = calcul(token);
            char resultString[100];
            if (isDouble) {
                newVar->type = DOUBLE;
                sprintf(resultString, "%lf", result);
            } else {
                newVar->type = INT;
                sprintf(resultString, "%d", (int) result);
            }

            newVar->value = malloc(strlen(resultString) + 1);
            if (newVar->value == NULL) {
                printf("Erreur d'allocation mémoire\n");
                freeVariable(newVar);
                freeVariable(headVar);
                exit(1);
            }
            strcpy(newVar->value, resultString);
        }
    } else if (strcmp(getType(token->type), "IDENTIFIER") == 0) {
        if (isVarExists(headVar, token->value)) {
            // on vérifie que l'utilisateur ne fait pas de a=a par exemple
            if (strcmp(token->value, newVar->name) == 0) return headVar;

            Var *var = getVariable(headVar, token->value);
            newVar->type = var->type;
            newVar->value = malloc(strlen(var->value) + 1);
            strcpy(newVar->value, var->value);
        } else {
            printf("Erreur : la variable n'existe pas\n");
            freeVariable(headVar);
            exit(1);
        }
    } else if (strcmp(getType(token->type), "TOKENSTRING") == 0) {
        // Traitement des chaînes simples
        newVar->type = STRING;
        newVar->value = malloc(strlen(token->value) + 1);
        if (newVar->value == NULL) {
            printf("Erreur d'allocation mémoire\n");
            freeVariable(newVar);
            freeVariable(headVar);
            exit(1);
        }
        strcpy(newVar->value, token->value);
    } else if (strcmp(getType(token->type), "NUMBER") == 0) {
        // Traitement des nombres
        if (isTokenDouble(token->value)) {
            newVar->type = DOUBLE;
            newVar->value = malloc(strlen(token->value) + strlen(token->value) + 1);
            if (newVar->value == NULL) {
                printf("Erreur d'allocation mémoire\n");
                freeVariable(newVar);
                freeVariable(headVar);
                exit(1);
            }
            sprintf(newVar->value, "%s", token->value);
        } else {
            newVar->type = INT;
            newVar->value = malloc(strlen(token->value) + 1);
            if (newVar->value == NULL) {
                printf("Erreur d'allocation mémoire\n");
                freeVariable(newVar);
                freeVariable(headVar);
                exit(1);
            }
            strcpy(newVar->value, token->value);
        }
    }

    if (isVarExist == 1) {
        return headVar;
    }
    newVar->nextVar = headVar;
    headVar = newVar;
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
    if (var == NULL) {
        printf("Aucune variable n'est enregistree");
        return;
    }
    if (var->nextVar != NULL) printVariables(var->nextVar);
    printf("%s %s %s\n", getVarType(var->type), var->name, var->value);
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

int isTokenDouble(char *value) {
    if (strpbrk(value, ".") == NULL) return 0;
    return 1;
}
