//
// Created by mathi on 04/10/2024.
//

#ifndef PARSER_H
#define PARSER_H

typedef enum inputType {
    NUMBER, // 0 1 2 3...
    TOKENSTRING,
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
    QUOTES, // "
    DOT, // .
    UNKNOWN = -1      // quand le lexer sait pas ou que c'est pas encore implémenté
} Type;

void parser(char *input);

int calculShuttingYard(char *input);

#endif //PARSER_H
