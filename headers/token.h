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
} Token;

Token *addToken(Token *, const Type, const char *);

void freeToken(Token *);

void printToken(Token *);

char *tokensToShuttingYardString(Token *, char ***, int *, StToken *);

int isBufferOperatorPriority(char * bufferOperator, char * tokenOperator);

int checkOperatorPriority(const char * operator);

#endif //TOKEN_H
