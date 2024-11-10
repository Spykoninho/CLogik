#include "../headers/print.h"
#include "../headers/token.h"
#include "../headers/variable.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Print the result of the expression
 * 
 * @param token
 */
void parserPrint(Token *token) {
    double *responseDouble = NULL;
    char *responseString = NULL;
    extern Var *variable;

    if (token == NULL || token->type != PRINT) {
        printf("Error: print function not found\n");
        printf("Token type : %d\n", token ? token->type : -1);
        return;
    }

    token = token->nextToken;
    if (token == NULL || token->type != LPAREN) {
        printf("Error: missing '(' after print\n");
        return;
    }

    token = token->nextToken;
    if (token == NULL) {
        printf("Error: missing argument to print\n");
        return;
    }

    if(token->type == IDENTIFIER) {
        Var *var = getVariable(variable, token->value);

        if (var == NULL) {
            printf("Error: variable %s not found\n", token->value);
            return;
        }

        printf("Result Print: %s\n", var->value);
    } else {
        responseDouble = malloc(sizeof(double));
        *responseDouble = calcul(token);
        printf("Result Print: %g\n", *responseDouble);
    }
}
