//
// Created by mathi on 01/10/2024.
//

#ifndef LEXER_H
#define LEXER_H

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
}Type;

typedef struct token {
    Type type;
    char value;
    struct token *nextToken;
}Token;

Token * lexer(char*, Token*);
char * getType(int);
Token * addToken(Token *, Type, char);
void printToken(Token *);
void freeToken(Token *);
#endif //LEXER_H
