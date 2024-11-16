//
// Created by mathi on 04/10/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/token.h"
#include "../headers/lexer.h"
#include "../headers/parser.h"
#include "../headers/AST.h"


// Ajouter un token
Token *addToken(Token *head, const Type type, const char *value) {
    // Crée le token
    Token *newToken = malloc(sizeof(Token));
    if (newToken == NULL) {
        printf("Erreur d'allocation mémoire pour le Token %s", value);
        freeTokens(head);
        exit(0);
    }

    // Met les valeurs
    newToken->type = type;
    newToken->value = malloc(sizeof(char) * (strlen(value) + 1));
    strcpy(newToken->value, value);
    newToken->nextToken = NULL; // met NULL car on va le mettre en dernier
    newToken->previousToken = NULL; // met NULL car on va le mettre en dernier

    // si c'est le 1er de la liste on le return tout de suite
    if (head == NULL) {
        freeTokens(head);
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
        freeTokens(head);
        exit(0);
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
Token *tokensToShuttingYardLinkedList(Token *tokens) {
    if (tokens == NULL) return NULL;
    Token *buffer = NULL;
    Token *output = NULL;
    while (tokens != NULL) {
        // Si la valeur est un nombre on le met directement dans l'output
        if (tokens->type == NUMBER) {
            output = addToken(output, tokens->type, tokens->value);
        }

        if (tokens->type == LPAREN) {
            buffer = addBufferToken(buffer, tokens->type, tokens->value);
        }

        if (tokens->type == RPAREN) {
            while (buffer != NULL && strcmp(buffer->value, "(") != 0) {
                output = addToken(output, buffer->type, buffer->value);
                buffer = popBufferToken(buffer);
            }
            if (buffer != NULL && strcmp(buffer->value, "(") == 0) {
                buffer = popBufferToken(buffer);
            }
        }

        // si c'est un opérateur on le met dans le buffer
        if (isOperator(tokens->type)) {
            while (buffer != NULL &&
                   isBufferOperatorPriority(buffer->type, tokens->type)) {
                output = addToken(output, buffer->type, buffer->value);
                buffer = popBufferToken(buffer);
            }
            buffer = addBufferToken(buffer, tokens->type, tokens->value);
        }
        // on passe au token suivant
        tokens = tokens->nextToken;
    }
    // on ajoute à la fin les opérateurs restants
    while (buffer != NULL) {
        output = addToken(output, buffer->type, buffer->value);
        buffer = popBufferToken(buffer);
    }
    freeTokens(buffer);
    freeTokens(tokens);
    return output;
}

double calcul(Token *token) {
    Token *stToken = tokensToShuttingYardLinkedList(token);
    Token *stTokenPile = NULL;
    Token *actualStToken = stToken;
    double result = 0;

    ASTNode *astStack[100];
    int astStackTop = -1;

    while (actualStToken != NULL) {
        if (isOperator(actualStToken->type)) {
            // On récupère les deux derniers opérandes de la pile pour le calcul
            Token *op2Token = stTokenPile;  // Le dernier opérande
            if (stTokenPile == NULL || stTokenPile->previousToken == NULL) {
                printf("Erreur, token manquant\n");
                exit(0);
            }
            stTokenPile = stTokenPile->previousToken;
            Token *op1Token = stTokenPile;  // L'avant-dernier opérande
            stTokenPile = stTokenPile->previousToken;

            // Convertir les opérandes en double pour le calcul
            char *error1, *error2;
            double operator1 = strtod(op1Token->value, &error1);
            double operator2 = strtod(op2Token->value, &error2);
            if (op1Token->value == error1 || op2Token->value == error2) {
                printf("Erreur de conversion en double.\n");
                exit(0);
            }

            // Effectuer le calcul en fonction de l'opérateur
            switch (actualStToken->type) {
                case PLUS:
                    result = operator1 + operator2;
                    break;
                case MINUS:
                    result = operator1 - operator2;
                    break;
                case MULT:
                    result = operator1 * operator2;
                    break;
                case DIV:
                    if (operator2 == 0) {
                        printf("Division par zéro interdite\n");
                        freeTokens(token);
                        exit(0);
                    }
                    result = operator1 / operator2;
                    break;
                case MOD:
                    if (operator2 == 0) {
                        printf("Modulo par zéro interdit.\n");
                        freeTokens(token);
                        exit(0);
                    }
                    result = (long)operator1 % (long)operator2;
                    break;
                default:
                    printf("Mauvais opérateur\n");
                    return 0;
            }

            // Crée un nœud d'AST pour l'opération
            char operatorChar = actualStToken->type == PLUS ? '+' :
                                actualStToken->type == MINUS ? '-' :
                                actualStToken->type == MULT ? '*' :
                                actualStToken->type == DIV ? '/' : '%';
            ASTNode *rightNode = astStack[astStackTop--];
            ASTNode *leftNode = astStack[astStackTop--];
            ASTNode *operationNode = createOperationNode(operatorChar, leftNode, rightNode);
            astStack[++astStackTop] = operationNode;

            // Mettre le résultat dans la pile
            char *resultValue = malloc(sizeof(char) * 100);
            sprintf(resultValue, "%lf", result);
            Token *resultToken = malloc(sizeof(Token));
            resultToken->type = NUMBER;
            resultToken->value = resultValue;
            resultToken->previousToken = stTokenPile;
            stTokenPile = resultToken;

            // Libérer les opérandes après le calcul
            freeToken(op1Token);
            freeToken(op2Token);
        } else {
            // Si c'est un nombre, crée un nœud d'AST pour ce nombre
            double value = strtod(actualStToken->value, NULL);
            ASTNode *numberNode = createNumberNode(value);
            astStack[++astStackTop] = numberNode;

            // Empiler l'élément dans la pile de tokens comme avant
            actualStToken->previousToken = stTokenPile;
            stTokenPile = actualStToken;
        }
        actualStToken = actualStToken->nextToken;
    }

    // La racine de l'AST est le dernier noeud de la pile AST
    ASTNode *astRoot = astStack[astStackTop];

    // Afficher l'AST après le calcul
    printf("AST de l'expression :\n");
    displayASTGraph(astRoot);

    // Libération de mémoire
    freeAST(astRoot);
    //freeTokens(stToken);

    return result;
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
    else {
        printf("Erreur, token NULL");
        freeTokens(token);
        exit(0);
    }
}

// enleve le dernier ajouté
Token *popBufferToken(Token *head) {
    if (head == NULL) {
        printf("Erreur, token NULL");
        freeTokens(head);
        exit(0);
    }
    Token *temp = head->nextToken;
    freeToken(head);
    return temp;
}
