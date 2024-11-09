//
// Created by mathi on 09/11/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/interpret.h"
#include "../headers/lexer.h"
#include "../headers/token.h"
#include "../headers/variable.h"

void interpret(char * input, Var ** variables) {
    Token *token = NULL;
    token = lexer(input, token);
    while (token->nextToken != NULL) {
        if(strcmp(getType(token->type), "FUNCTION") == 0) {
            if(strcmp(token->value, "print") == 0) { // call print func
                while (token->nextToken != NULL) { // FIXME : remplacer par la vraie fonction print
                    printf("%s", token->nextToken->value);
                    token=token->nextToken;
                }
                printf("\n");
            }
        }else if(strcmp(getType(token->type), "IDENTIFIER") == 0) {
            const Token * tempToken = token->nextToken;
            if(strcmp(getType(tempToken->type), "ASSIGN") == 0) {
                *variables = addVariable(token, *variables);
                printVariables(*variables);
            }
        }

        if(token->nextToken != NULL) token=token->nextToken;
    }
}
