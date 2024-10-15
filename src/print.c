#include "../headers/print.h"
#include "../headers/token.h"
#include <stdio.h>
#include <stdlib.h>

void parserPrint(Token *token) {
    int *responseInt = NULL;
    char *responseString = NULL;

    if (token == NULL || token->type != PRINT) {
        printf("Error: print function not found\n");
        printf("Token type : %d\n", token->value);
        return;
    }

    token = token->nextToken;
    if (token == NULL || token->type != LPAREN) {
        printf("Error: missing '(' after print\n");
        return;
    }

    token = token->nextToken;
    if (token == NULL || (token->type != IDENTIFIER && token->type != NUMBER)) {
        printf("Error: missing argument to print\n");
        return;
    }

    if (token->type == NUMBER) {
        responseInt = malloc(sizeof(int));
        *responseInt = atoi(token->value);
    }
    if (token->type == IDENTIFIER) responseString = token->value;

    token = token->nextToken;
    if (token == NULL || token->type != RPAREN) {
        printf("Error: missing ')' after print\n");
        return;
    }

    if (responseInt != NULL) printf("%d\n", *responseInt);
    else printf("%s\n", responseString);

    token = token->nextToken;
}