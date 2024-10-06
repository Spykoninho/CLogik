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
    if (token != NULL) {
        // Si la valeur est un nombre on le met directement dans l'output
        if (strcmp(getType(token->type), "NUMBER") == 0) {
            // Si c'est le premier ajout on fait un copy car strcat se fait que sur une chaine existante
            if (strlen(shuttingYardString) == 0) {
                strncpy(shuttingYardString, token->value, 1024 - strlen(token->value) - 1);
                shuttingYardString[1024 - strlen(token->value)] = '\0';
            } else {
                strncat(shuttingYardString, token->value, 1024 - strlen(token->value) - 1);
            }
        }

        if (strcmp(getType(token->type), "LPAREN") == 0) {
            pushBuffer(buffer, bufferSize, token->value);
        }

        if (strcmp(getType(token->type), "RPAREN") == 0) {
            while (strcmp((*buffer)[*bufferSize - 1], "(") != 0) {
                if (*bufferSize == 0) {
                    printf("ERREUR, IL MANQUE UNE PARENTHESE");
                    return NULL;
                }
                if (strlen(shuttingYardString) == 0) {
                    strncpy(shuttingYardString, token->value, 1024 - strlen(token->value) - 1);
                } else {
                    strncat(shuttingYardString, (*buffer)[*bufferSize - 1],
                            1024 - strlen((*buffer)[*bufferSize - 1]) - 1);
                }
                popBuffer(buffer, bufferSize);
            }

            if (*bufferSize > 0 && strcmp((*buffer)[*bufferSize - 1], "(") == 0) {
                popBuffer(buffer, bufferSize);
            }
        }

        // si c'est un opérateur on le met dans le buffer
        if (strcmp(getType(token->type), "OPERATOR") == 0) {
            if (*bufferSize == 0) {
                pushBuffer(buffer, bufferSize, token->value);
            } else {
                while (*bufferSize > 0 && strcmp(getType(token->type), "OPERATOR") == 0) {
                    if (isBufferOperatorPriority((*buffer)[*bufferSize - 1], token->value)) {
                        if (*bufferSize == 0) {
                            strncpy(shuttingYardString, (*buffer)[*bufferSize - 1],
                                    1024 - strlen((*buffer)[*bufferSize - 1]) - 1);
                            shuttingYardString[1024 - strlen((*buffer)[*bufferSize - 1])] = '\0';
                        } else {
                            strncat(shuttingYardString, (*buffer)[*bufferSize - 1],
                                    1024 - strlen((*buffer)[*bufferSize - 1]) - 1);
                        }
                        popBuffer(buffer, bufferSize);
                    } else {
                        break;
                    }
                }
                pushBuffer(buffer, bufferSize, token->value);
            }
        }
        // on passe au token suivant
        tokensToShuttingYardString(token->nextToken, buffer, bufferSize, shuttingYardString);
        return shuttingYardString; // on return la string
    }

    // on ajoute à la fin les opérateurs restants
    while (*bufferSize > 0) {
        strcat(shuttingYardString, (*buffer)[*bufferSize - 1]);
        popBuffer(buffer, bufferSize);
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

// Définit si l'operateur du buffer doit aller à la sortie
int isBufferOperatorPriority(char *bufferOperator, char *tokenOperator) {
    // si c'est égal à l'operateur entrant alors true
    if (strcmp(bufferOperator, tokenOperator) == 0) return 1;

    // On check la priorité, si buffer >= newOperator alors ok le l'operator du buffer sort
    if (checkOperatorPriority(bufferOperator) >= checkOperatorPriority(tokenOperator)) return 1;
    if (checkOperatorPriority(bufferOperator) < checkOperatorPriority(tokenOperator)) return 0;

    // Au cas où
    return 0;
}

int checkOperatorPriority(const char *operator) {
    switch (operator[0]) {
        // si on veux faire les += etc faire plutot un strcmp avec plusieurs if
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
        default:
            return 0;
    }
}
