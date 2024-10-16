#include "../headers/print.h"
#include "../headers/token.h"
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

    double result = calcul(token);

    // Check if the print parenthesis is valid
    if (!checkValidPrint(&token)) {
        printf("Error: invalid print due to parenthesis\n");
        return;
    }

    if (token == NULL || token->type != RPAREN) {
        printf("Error: missing ')' after print\n");
        return;
    }

    printf("Result Print: %g\n", result);
}
