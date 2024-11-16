//
// Created by mathi on 04/10/2024.
//

#ifndef TOKEN_H
#define TOKEN_H

typedef enum inputType {
    FUNCTION,
    NUMBER, // 0 1 2 3...
    TOKENSTRING,
    KEYWORD, // if...
    LPAREN, // (
    RPAREN, // )
    IDENTIFIER, // variable
    PLUS, // +
    MINUS, // -
    MULT, // *
    DIV, // /
    MOD, // % (à implémenter avec les conditions)
    POW,
    GREATER, // >
    LESS, // <
    GREATEREQ, // >=
    LESSEQ, // <=
    EQUAL, // ==
    NOT, // !
    NOTEQUAL, // !=
    LBRACE, // {
    RBRACE, // }
    SEMICOLON, // ;
    ASSIGN, // =
    DOT, // .
    PRINT, // print
    IF, // if
    ELSE, // else
    TRUE, // true
    FALSE, // false
    UNKNOWN = -1 // quand le lexer sait pas ou que c'est pas encore implémenté
} Type;

typedef struct token {
    Type type;
    char *value;
    struct token *nextToken;
    struct token *previousToken;
} Token;

Token *addToken(Token *, const Type, const char *);

void freeTokens(Token *);

void freeToken(Token *);

void printTokens(Token *tokens);

void printToken(Token *token);

Token *tokensToShuttingYardLinkedList(Token *);

int isBufferOperatorPriority(Type bufferType, Type operatorType);

int checkOperatorPriority(Type type);

int isOperator(Type type);

double calcul(Token *stToken);

Token *popBufferToken(Token *head);

Token *addBufferToken(Token *head, const Type type, const char *value);

#endif //TOKEN_H
