#include "../headers/condition.h"
#include "<stdlib.h>"

void parserCondition(Token *token) {
  if (token == NULL || token->type != IF) {
        printf("Error: condition not found\n");
        printf("Token type : %s\n", token ? getType(token->type) : "ERROR");
        return;
    }

    token = token->nextToken;
    if (token == NULL || token->type != LPAREN) {
        printf("Error: missing '(' after condition\n");
        return;
    }

    token = token->nextToken;
    if (token == NULL) {
        printf("Error: missing argument to the condition\n");
        return;
    }

    
}