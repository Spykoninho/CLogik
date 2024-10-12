//
// Created by mathi on 04/10/2024.
//

#ifndef PARSER_H
#define PARSER_H

typedef enum inputType {
    NUMBER, // 0 1 2 3...
    KEYWORD, // if...
    LPAREN, // (
    RPAREN, // )
    IDENTIFIER, // variable
    PLUS,             // +
    MINUS,            // -
    MULT,             // *
    DIV,              // /
    MOD,              // % (à implémenter avec les conditions)
    POW,
    GREATER,          // > (à implémenter avec les conditions)
    LESS,             // < (à implémenter avec les conditions)
    GREATEREQ,        // >= (à implémenter avec les conditions)
    LESSEQ,           // <= (à implémenter avec les conditions)
    EQUAL,            // == (à implémenter avec les conditions)
    NOT,              // ! (à implémenter avec les conditions)
    LBRACE, // {
    RBRACE, // }
    SEMICOLON, // ;
    ASSIGN, // =
    UNKNOWN = -1      // quand le lexer sait pas ou que c'est pas encore implémenté
} Type;

typedef struct ShuttingYardToken {
    char * value;
    Type type;
    struct ShuttingYardToken * nextStToken;
    struct ShuttingYardToken * previousStToken;
}StToken;

typedef struct StBuffer {
    char * value;
    Type type;
    struct StBuffer * nextStBuffer;
}StBuffer;

void parser(char *input);

void pushBuffer(char ***buffer, int *bufferSize, char *value);

void popBuffer(char ***buffer, int *bufferSize);

void freeBuffer(char **buffer, int bufferSize);

void printBuffer(char **buffer, int bufferSize);

int calculShuttingYard(char *input);

StToken * addStToken(StToken* head, Type type, char * value);

// print pour debug
void printStToken(StToken * stToken);
// Free buffer et sttoken
void freeStToken(StToken * stToken);
// get ??? (jsp à quoi ça sert)
// addbuffer
void addBuffer(StBuffer ** buffer, Type type, char * value);

#endif //PARSER_H
