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

void printStToken(StToken *token) {
    printf("Printing stToken\n");
    StToken *temp = token;
    if (temp == NULL) {
        printf("stToken is empty\n");
        return;
    }
    do {
        printf("{type: \"%s\", value: \"%s\"},\n", getType(temp->type), temp->value);
        temp = temp->nextStToken;
    } while (temp != NULL);
}

// Convertit la liste de token en liste chainée shuttingyard
void tokensToShuttingYardLinkedList(const Token *token, char ***buffer, int *bufferSize, StToken **stToken) {
    if (token == NULL) {
        return;
    }
    StToken *stTokenPile = NULL;
    do {
        if (isOperator(token->type)) {
            if (stTokenPile == NULL) {
                stTokenPile = addStToken(stTokenPile, token->type, token->value);
            } else {
                while (stTokenPile != NULL && isBufferOperatorPriority(stTokenPile->type, token->type)) {
                    *stToken = addStToken(*stToken, stTokenPile->type, stTokenPile->value);
                    stTokenPile = stTokenPile->nextStToken;
                }
                stTokenPile = addStToken(stTokenPile, token->type, token->value);
            }
        } else if (token->type == LPAREN) {
            stTokenPile = addStToken(stTokenPile, token->type, token->value);
        } else if (token->type == RPAREN) {
            while (stTokenPile != NULL && stTokenPile->type != LPAREN) {
                *stToken = addStToken(*stToken, stTokenPile->type, stTokenPile->value);
                StToken *oldstTokenPile = stTokenPile;
                stTokenPile = stTokenPile->previousStToken;
                free(oldstTokenPile);
            }
        } else {
            StToken *newStToken = addStToken(*stToken, token->type, token->value);
            *stToken = newStToken;
        }
        token = token->nextToken;
    } while (token != NULL);
    StToken *lastStTokenPileElement = stTokenPile;
    while (stTokenPile != NULL) {
        lastStTokenPileElement = stTokenPile;
        stTokenPile = stTokenPile->nextStToken;
    }

    while (lastStTokenPileElement != NULL) {
        *stToken = addStToken(*stToken, lastStTokenPileElement->type, lastStTokenPileElement->value);
        lastStTokenPileElement = lastStTokenPileElement->previousStToken;
    }
    printStToken(*stToken);
    calcul(*stToken);
}

void calcul(StToken *stToken) {
    StToken *stTokenPile = NULL;
    StToken *actualStToken = stToken;
    while (actualStToken != NULL) {
        if (isOperator(actualStToken->type)) // si on a un opérateur, on fait le calcul
        {
            char *op1 = actualStToken->previousStToken->previousStToken->value;
            // on prend les deux derniers éléments de la stTokenPile
            char *op2 = actualStToken->previousStToken->value;
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
            actualStToken->previousStToken->previousStToken->value = malloc(sizeof(char) * 100);
            itoa(result, actualStToken->previousStToken->previousStToken->value, 10);
            // on met le résultat à la place du premier opérande. itoa permet de convertir un int en char *, c'est l'inverse de itoa que j'ai changé en strtol (lol)
            StToken *oldStTokenPile = stTokenPile;
            stTokenPile = stTokenPile->previousStToken;
            free(oldStTokenPile);
        } else {
            // si c'est pas un opérateur, on met l'élément dans la stTokenPile, du coup on a toujours les deux derniers éléments de la stTokenPile qui seront les opérandes
            if (stTokenPile == NULL) {
                stTokenPile = actualStToken;
            } else {
                stTokenPile->nextStToken = actualStToken;
                actualStToken->previousStToken = stTokenPile;
                stTokenPile = actualStToken;
            }
            printf("Pruntin stTokenPile\n");
            printStToken(stTokenPile);
        }
        actualStToken = actualStToken->nextStToken;
    }
    printf("--------------------------------------------------------------\n");
    printf("Result : %s\n", stTokenPile->value); // on affiche le résultat
    printf("--------------------------------------------------------------\n");
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

void printoneStToken(StToken *actualStToken) {
    printf("Printing one stToken\n");
    printf("{type: \"%s\", value: \"%s\"},\n", getType(actualStToken->type), actualStToken->value);
}
