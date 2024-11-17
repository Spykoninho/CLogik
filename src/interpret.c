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
#include "../headers/parser.h"

void interpret(char *input) {
    Token *token = NULL;
    token = lexer(input, token);
    parser(token);
    if(token->type == LBRACE) {
        actualScope++;
        return;
    }
    if(token->type == RBRACE) {
        actualScope--;
        return;
    }
    while (token->nextToken != NULL) {
        if (token->type == PRINT) {
            parserPrint(token);
        } else if (token->type == IDENTIFIER) {
            const Token *tempToken = token->nextToken;
            if (tempToken->type == ASSIGN) {
                addVariable(token);
            }
        }else if(token->type == AST) {
            astEnabled = !astEnabled; // Change l'état d'affichage de l'AST
            printf("Affichage AST %s\n", astEnabled ? "active" : "desactive");
        }else if(token->type == LBRACE) {
            actualScope++;
        }else if(token->type == RBRACE) {
            actualScope--;
        }else if (token->type == KEYWORD) {
            if (strcmp(token->value,"while")== 0 ) {
                parseWhile(&token);
            }
            if (token == NULL) {
                break;
            }
        }

        if (token->nextToken != NULL) token = token->nextToken;
    }
    freeTokens(token);
}
