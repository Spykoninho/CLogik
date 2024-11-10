//
// Created by mathi on 04/10/2024.
//

#ifndef PARSER_H
#define PARSER_H

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
    UNKNOWN = -1 // quand le lexer sait pas ou que c'est pas encore implémenté
} Type;

void parser(char *input);

int calculShuttingYard(char *input);

#endif //PARSER_H
