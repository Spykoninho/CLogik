#include "../headers/print.h"
#include "../headers/token.h"
#include "../headers/variable.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Check if the print parenthesis is valid
 * 
 * @param token 
 * @return int 
 */
int checkValidPrint(Token** token) {
    int count = 0;
    while ((*token)->nextToken != NULL) {
        if ((*token)->type == LPAREN) {
            count++;
        } else if ((*token)->type == RPAREN) {
            count--;
        }

        *token = (*token)->nextToken;
    }
    
    return count == 0;
}

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
    if (token == NULL || (token->type != IDENTIFIER && token->type != NUMBER)) {
        printf("Error: missing argument to print\n");
        return;
    }

    if (token->type == IDENTIFIER) {
        Var *var = getVariable(variable, token->value);
        
        if (var == NULL) {
            printf("Error: variable %s not found\n", token->value);
            return;
        }

        if (var->type == INT) {
            responseString = malloc(sizeof(char) * 255);
            sprintf(responseString, "%s", var->value);
        } else if (var->type == DOUBLE) {
            responseDouble = malloc(sizeof(double));
            *responseDouble = atof(var->value);
        } else {
            responseString = malloc(sizeof(char) * 255);
            sprintf(responseString, "%s", var->value);
        }
    } else if (token->type == NUMBER) {
        responseDouble = malloc(sizeof(double));
        *responseDouble = calcul(token);
        printf("Result Print: %g\n", *responseDouble);
    }

    token = token->nextToken;

    // Check if the print parenthesis is valid
    if (!checkValidPrint(&token)) {
        printf("Error: invalid print due to parenthesis\n");
        return;
    }

    if (token == NULL || token->type != RPAREN) {
        printf("Error: missing ')' after print\n");
        return;
    }
    printf("Token value : %s\n", token->value);

    // printf("Result Print: %g\n", result);
}
