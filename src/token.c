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
    StToken *lastStTokenPileElement = stTokenPile;
    while (stTokenPile != NULL) {
        lastStTokenPileElement = stTokenPile;
        stTokenPile = stTokenPile->nextStToken;
    }

    while (lastStTokenPileElement != NULL) {
        *stToken = addStToken(*stToken, lastStTokenPileElement->type, lastStTokenPileElement->value);
        lastStTokenPileElement = lastStTokenPileElement->previousStToken;
    }

    token = token->nextToken;
}

while
(token
!=
NULL
);

printStToken( *stToken);

}

// Si c'est une parenthèse ouvrante on la met dans le buffer
/*
if (strcmp(getType(token->type), "LPAREN") == 0) { // on le met dans le buffer
   pushBuffer(buffer, bufferSize, token->value);
}

if (strcmp(getType(token->type), "RPAREN") == 0) {
   while (strcmp((*buffer)[*bufferSize - 1], "(") != 0) {
       if (*bufferSize == 0) {
           printf("ERREUR, IL MANQUE UNE PARENTHESE");
           exit(1);
       }
       // Retrouver la valeur et le type du buffer, voir pour faire un buffer en ligne chainée
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
               // add buffer
               popBuffer(buffer, bufferSize);
           } else {
               break;
           }
       }
       pushBuffer(buffer, bufferSize, token->value);
   }
}
// on passe au token suivant
//tokensToShuttingYardString(token->nextToken, buffer, bufferSize, stToken);
//return stToken;
}

// on ajoute à la fin les opérateurs restants
while (*bufferSize > 0) {
addStToken(stToken, NUMBER, (*buffer)[*bufferSize - 1]); // a revoir le buffer
popBuffer(buffer, bufferSize);
}
}

// Libère les tokens
void freeToken(Token *token) {
if (token != NULL) {
free(token->value);
freeToken(token->nextToken);
free(token);
}
}
*/
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
