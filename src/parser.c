//
// Created by mathi on 04/10/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include "../headers/token.h"
#include "../headers/parser.h"
#include "../headers/lexer.h"

void parser(char * input) {
    Token * token = NULL;
    token = lexer(input, token);

    if (token != NULL) {
        printToken(token);
        free(token);
    }
}
