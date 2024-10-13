//
// Created by mathi on 04/10/2024.
//

#ifndef TOKEN_H
#define TOKEN_H
#include "parser.h"

typedef struct token {
    Type type;
    char *value;
    struct token *nextToken;
    struct token *previousToken;
} Token;

Token *addToken(Token *, const Type, const char *);

void freeTokens(Token *);

void freeToken(Token *);

void printTokens(Token * tokens);

void printToken(Token * token);

Token * tokensToShuttingYardLinkedList(Token *);

int isBufferOperatorPriority(Type bufferType, Type operatorType);

int checkOperatorPriority(Type type);

int isOperator(Type type);

double calcul(Token * stToken);

Token *popBufferToken(Token *head);

Token *addBufferToken(Token *head, const Type type, const char *value);

#endif //TOKEN_H
