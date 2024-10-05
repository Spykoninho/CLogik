//
// Created by mathi on 04/10/2024.
//

#ifndef TOKEN_H
#define TOKEN_H

typedef enum inputType {
    NUMBER, // 0 1 2 3...
    KEYWORD, // if...
    LPAREN, // (
    RPAREN, // )
    IDENTIFIER, // variable
    OPERATOR, // <, &&...
    LBRACE, // {
    RBRACE, // }
    SEMICOLON, // ;
    ASSIGN // =
} Type;

typedef struct token {
    Type type;
    char *value;
    struct token *nextToken;
} Token;

Token *addToken(Token *, const Type, const char *);

void freeToken(Token *);

void printToken(Token *);

char *tokensToShuttingYardString(Token *, char ***, int *, char *);

#endif //TOKEN_H
