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
                return;
            }
        }

        // Print une concaténation de string
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
            if (token == NULL || token->type != RPAREN) {
                printf("Error: missing ')' after print\n");
                return;
            }
            printf("%s\n", printedString);
            free(printedString);
            return;
        }else{
            // Traitement des calculs avec des nombres
            Token *tempTokenCalcul = token;
            while (tempTokenCalcul != NULL && tempTokenCalcul->type != RPAREN) {
                if (tempTokenCalcul->type == IDENTIFIER) {
                    if (isVarExists(variables, tempTokenCalcul->value)) {
                        const Var *var = getVariable(variables, tempTokenCalcul->value);
                        // On prend le token de la variable et on regarde si c'est un double
                        char *tempRealloc = realloc(tempTokenCalcul->value, sizeof(char) * (strlen(var->value) + 1));
                        if (tempRealloc == NULL) {
                            printf("Erreur de reallocation de memoire pour tempTokenCalcul->value\n");
                            freeVariable(variables);
                            freeTokens(token);
                            return;
                        }
                        // nouvelle taille pour la value
                        tempTokenCalcul->value = tempRealloc;
                        strcpy(tempTokenCalcul->value, var->value);
                        tempTokenCalcul->type = NUMBER;
                    } else {
                        printf("Erreur : variable '%s' inexistante\n", tempTokenCalcul->value);
                        freeVariable(variables);
                        return;
                    }
                }
                tempTokenCalcul = tempTokenCalcul->nextToken;
            }
            if (tempTokenCalcul == NULL || tempTokenCalcul->type != RPAREN) {
                printf("Error: missing ')' after print\n");
                return;
            }
            double result = calcul(token);
            printf("%g\n", result);
            return;
        }
    }
    if (token->type == IDENTIFIER) {
        if (isVarExists(variables, token->value)) {
            Var *var = getVariable(variables, token->value);
            token = token->nextToken;
            if (token == NULL || token->type != RPAREN) {
                printf("Error: missing ')' after print\n");
                return;
            }
            if(var->type == INT) {
                int result = strtol(var->value, NULL, 10);
                printf("%d\n", result);
            }else if(var->type == DOUBLE) {
                double result = strtod(var->value, NULL);
                printf("%g\n", result);
            }else {
                printf("%s\n", var->value);
            }
            return;
        } else {
            printf("Erreur : la variable n'existe pas\n");
            return;
        }
    }else if (token->type == TOKENSTRING){
        Token *tempToken = token->nextToken;
        if (tempToken == NULL || tempToken->type != RPAREN) {
            printf("Error: missing ')' after print\n");
            return;
        }
        printf("%s\n", token->value);
        return;
    }else if(token->type == NUMBER) {
        Token *tempToken = token->nextToken;
        if (tempToken == NULL || tempToken->type != RPAREN) {
            printf("Error: missing ')' after print\n");
            return;
        }
        printf("%s\n", token->value);
        return;
    }

    printf("Erreur : print non reconnu");
}
