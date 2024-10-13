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
        printf("Erreur d'allocation mémoire pour le Token %s", value);
        return NULL;
    }

    // Met les valeurs
    newToken->type = type;
    newToken->value = malloc(sizeof(char) * (strlen(value) + 1));
    strcpy(newToken->value, value);
    newToken->nextToken = NULL; // met NULL car on va le mettre en dernier
    newToken->previousToken = NULL; // met NULL car on va le mettre en dernier

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
    newToken->previousToken = temp;
    // on return le même que celui d'entrée vu qu'on ne modifie que le dernier de la liste
    return head;
}

// Ici on veut garder l'ordre naturel de la list puisque le dernier ajouté doit sortir en premier
Token *addBufferToken(Token *head, const Type type, const char *value) {
    // Crée le token
    Token *newToken = malloc(sizeof(Token));
    if (newToken == NULL) {
        printf("Erreur d'allocation mémoire pour le Token %s", value);
        return NULL;
    }

    // Met les valeurs
    newToken->type = type;
    newToken->value = malloc(sizeof(char) * (strlen(value) + 1));
    strcpy(newToken->value, value);
    newToken->nextToken = head;
    newToken->previousToken = NULL; // met NULL car on va le mettre en dernier

    return newToken;
}


// Print les tokens dans l'ordre (déjà trié lors de l'ajout)
void printTokens(Token *tokens) {
    while (tokens != NULL) {
        printToken(tokens);
        tokens = tokens->nextToken;
    }
}

// Convertit la liste de token en liste chainée shuttingyard
Token *tokensToShuttingYardLinkedList(Token *token) {
    if (token == NULL) return NULL;
    Token *buffer = NULL;
    Token *output = NULL;
    while (token != NULL) {
        // Si la valeur est un nombre on le met directement dans l'output
        if (strcmp(getType(token->type), "NUMBER") == 0) {
            output = addToken(output, token->type, token->value);
        }

        if (strcmp(getType(token->type), "LPAREN") == 0) {
            buffer = addBufferToken(buffer, token->type, token->value);
        }

        if (strcmp(getType(token->type), "RPAREN") == 0) {
            while (buffer != NULL && strcmp(buffer->value, "(") != 0) {
                output = addToken(output, buffer->type, buffer->value);
                buffer = popBufferToken(buffer);
            }
            if (buffer != NULL && strcmp(buffer->value, "(") == 0) {
                buffer = popBufferToken(buffer);
            }
        }

        // si c'est un opérateur on le met dans le buffer
        if (isOperator(token->type)) {
            while (buffer != NULL &&
                   isBufferOperatorPriority(buffer->type, token->type)) {
                output = addToken(output, buffer->type, buffer->value);
                buffer = popBufferToken(buffer);
            }
            buffer = addBufferToken(buffer, token->type, token->value);
        }
        // on passe au token suivant
        token = token->nextToken;
    }
    // on ajoute à la fin les opérateurs restants
    while (buffer != NULL) {
        output = addToken(output, buffer->type, buffer->value);
        buffer = popBufferToken(buffer);
    }
    printTokens(output);
    calcul(output);
    return output;
}

void calcul(Token *stToken) {
    Token *stTokenPile = NULL;
    Token *actualStToken = stToken;
    while (actualStToken != NULL) {
        if (isOperator(actualStToken->type)) // si on a un opérateur, on fait le calcul
        {
            char *op1 = actualStToken->previousToken->previousToken->value;
            // on prend les deux derniers éléments de la stTokenPile
            char *op2 = actualStToken->previousToken->value;
            int result = 0;
            char *error1;
            long operator1 = strtol(op1, &error1, 10);
            if (op1 == error1) {
                printf("Error op 1");
            }
            char *error2;
            long operator2 = strtol(op2, &error2, 10);
            if (op2 == error2) {
                printf("Error op 2");
            }
            switch (actualStToken->type) // on fait le calcul, si on a les deux opérandes
            {
                // dans la stTokenPile on aura un truc qui ressemblera à ça : [1,4,+, 2, *] => (1 + 4) * 2
                // op1 = 4, op2 = 1, actualStToken = +
                case PLUS:
                    result = operator1 + operator2;
                    if (operator1 != 0 && operator2 != 0 && result - operator1 != operator2) {
                        printf("Overflow detected. The result might not be accurate.\n");
                    }
                    break;
                case MINUS:
                    result = operator1 - operator2;
                    break;
                case MULT:
                    result = operator1 * operator2;
                    if (operator1 != 0 && operator2 != 0 && result / operator1 != operator2) {
                        printf("Overflow detected. The result might not be accurate.\n");
                    }
                    break;
                case DIV:
                    if (operator2 == 0) {
                        printf("Division by zero\n");
                    }
                    result = operator1 / operator2;
                    break;
                case MOD:
                    if (operator2 == 0) {
                        printf("Modulo by zero\n");
                    }
                    result = operator1 % operator2;
                    break;
                default:
                    break;
            }
            actualStToken->previousToken->previousToken->value = malloc(sizeof(char) * 100);
            itoa(result, actualStToken->previousToken->previousToken->value, 10);
            // on met le résultat à la place du premier opérande. itoa permet de convertir un int en char *, c'est l'inverse de itoa que j'ai changé en strtol (lol)
            Token *oldStTokenPile = stTokenPile;
            stTokenPile = stTokenPile->previousToken;
            free(oldStTokenPile);
        } else {
            // si c'est pas un opérateur, on met l'élément dans la stTokenPile, du coup on a toujours les deux derniers éléments de la stTokenPile qui seront les opérandes
            if (stTokenPile == NULL) {
                stTokenPile = actualStToken;
            } else {
                stTokenPile->nextToken = actualStToken;
                actualStToken->previousToken = stTokenPile;
                stTokenPile = actualStToken;
            }
        }
        actualStToken = actualStToken->nextToken;
    }
    printf("--------------------------------------------------------------\n");
    printf("Result : %s\n", stTokenPile->value); // on affiche le résultat
    printf("--------------------------------------------------------------\n");
}

// Libère les tokens
void freeTokens(Token *token) {
    if (token != NULL) {
        free(token->value);
        freeTokens(token->nextToken);
        free(token);
    }
}

// Libère le token
void freeToken(Token *token) {
    if (token != NULL) {
        free(token->value);
        free(token);
    }
}

// Définit si l'operateur du buffer doit aller à la sortie
int isBufferOperatorPriority(Type bufferOperator, Type tokenOperator) {
    // si c'est égal à l'operateur entrant alors true
    if (bufferOperator == tokenOperator) return 1;

    // On check la priorité, si buffer >= newOperator alors ok le l'operator du buffer sort
    if (checkOperatorPriority(bufferOperator) >= checkOperatorPriority(tokenOperator)) return 1;
    if (checkOperatorPriority(bufferOperator) < checkOperatorPriority(tokenOperator)) return 0;

    // Au cas où
    return 0;
}

int checkOperatorPriority(Type type) {
    switch (type) {
        // si on veux faire les += etc faire plutot un strcmp avec plusieurs if (rien compris)
        case PLUS:
        case MINUS:
            return 1;
        case MULT:
        case DIV:
            return 2;
        case POW:
            return 3;
        default:
            return 0;
    }
}

int isOperator(Type type) {
    switch (type) {
        case PLUS:
        case MINUS:
        case MULT:
        case DIV:
        case MOD:
        case GREATER:
        case LESS:
        case GREATEREQ:
        case LESSEQ:
        case EQUAL:
        case ASSIGN:
        case NOT:
            return 1;
        default:
            return 0;
    }
}

void printToken(Token *token) {
    if (token != NULL) printf("{type: \"%s\", value: \"%s\"},\n", getType(token->type), token->value);
    else printf("Erreur, token NULL");
}

// enleve le dernier ajouté
Token *popBufferToken(Token *head) {
    if (head == NULL) {
        printf("Erreur, token NULL");
        return NULL;
    }
    Token *temp = head->nextToken;
    freeToken(head);
    return temp;
}
