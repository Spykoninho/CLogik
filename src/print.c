#include "../headers/print.h"
#include "../headers/token.h"
#include "../headers/variable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/lexer.h"

/**
 * @brief Print the result of the expression
 * 
 * @param token
 */
void parserPrint(Token *token) {
    // Check syntaxe print(args)
    if (token == NULL || token->type != PRINT) {
        printf("Error: print function not found\n");
        printf("Token type : %s\n", token ? getType(token->type) : "ERROR");
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

    int isCalcul = 0;
    int isString = 0;
    Token *temp = token;
    while (temp != NULL) {
        if (temp->type == STRING) isString++;
        if (isOperator(temp->type)) isCalcul = 1;
        temp = temp->nextToken;
    }
    freeTokens(temp);

    if (isCalcul) {
        int isVarString = 0;
        if (token->type == IDENTIFIER) {
            if (isVarExists(variables, token->value)) {
                Var *checkVar = getVariable(variables, token->value);
                if (checkVar->type == STRING) isVarString = 1;
            } else {
                printf("Erreur : La variable n'existe pas\n");
                exit(1);
            }
        }

        if (token->type == TOKENSTRING || isVarString) {
            char *printedString = malloc(255 * sizeof(char));
            if (printedString == NULL) {
                printf("Erreur d'allocation de memoire\n");
                return;
            }
            printedString[0] = '\0';

            if (isVarString) {
                Var *var = getVariable(variables, token->value);
                strncat(printedString, var->value, 255 - strlen(printedString));
            } else {
                strncat(printedString, token->value, 255 - strlen(printedString));
            }
            token = token->nextToken;

            // Traitement des concaténations
            while (token != NULL && token->type == PLUS) {
                token = token->nextToken; // Sauter le '+'

                if (token->type == TOKENSTRING) {
                    strncat(printedString, token->value, 255 - strlen(printedString));
                } else if (token->type == IDENTIFIER) {
                    Var *var = getVariable(variables, token->value);
                    if (var->type != STRING) {
                        printf("Erreur : la concatenation doit etre avec une chaine de caracteres\n");
                        return;
                    }
                    strncat(printedString, var->value, 255 - strlen(printedString));
                } else {
                    printf("Erreur : concatenation invalide\n");
                    return;
                }
                token = token->nextToken;
            }
            printf("%s\n", printedString);
            free(printedString);
            return;
        }
    }
    if (token == NULL || token->type != RPAREN) {
        printf("Error: missing ')' after print\n");
        return;
    }
    printf("Result Print: %g\n", calcul(token));
}
