//
// Created by mathi on 09/11/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/interpret.h"
#include "../headers/lexer.h"
#include "../headers/print.h"
#include "../headers/token.h"
#include "../headers/variable.h"

void interpret(char *input) {
    Token *token = NULL;
    token = lexer(input, token);
    while (token->nextToken != NULL) {
        if (strcmp(getType(token->type), "PRINT") == 0) {
            parserPrint(token);
        } else if (strcmp(getType(token->type), "IDENTIFIER") == 0) {
            const Token *tempToken = token->nextToken;
            if (strcmp(getType(tempToken->type), "ASSIGN") == 0) {
                addVariable(token);
                printVariables(variables);
            }
        }

        if (token->nextToken != NULL) token = token->nextToken;
    }
    freeTokens(token);
}
