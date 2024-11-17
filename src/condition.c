#include "../headers/condition.h"

int evaluateIfCondition(Token *token) {
    if (token->type == TRUE)  return 1;
    if (token->type == FALSE) return 0;
}

void parseBlockIf(Token *token) {
    while (token != NULL && token->type != RBRACE) {
        if (token->type == PRINT) {
            parserPrint(token);
        }
        token = token->nextToken;
    }
    if (token == NULL || token->type != RBRACE) {
        printf("Erreur il manque }\n");
    }
}

void parserIf(Token *token) {
    if (token == NULL || token->type != IF) {
        printf("Erreur le namespace if n'a pas été trouvé\n");
        printf("Token type: %s\n", getType(token->type));
        return;
    }

    token = token->nextToken;
    if (token == NULL || token->type != LPAREN) {
        printf("Erreur parenthese ouvrante\n");
        return;
    }

    token = token->nextToken;
    if (token == NULL) {
        printf("Erreur pas d'argument au niveau du if\n");
        return;
    }

    int conditionResult = evaluateIfCondition(token);

    if (conditionResult) {
        token = token->nextToken;
        if (token == NULL || token->nextToken->type != LBRACE) {
            printf("%s\n", token->value);
            printf("Erreur il manque { apres le if\n");
            return;
        }

        parseBlockIf(token->nextToken);
    } else {
        token = token->nextToken;
        if (token != NULL && token->type == ELSE) {
            token = token->nextToken;
            if (token == NULL || token->type != LBRACE) {
                printf("Erreur il manque { apres le else\n");
                return;
            }
            parseBlockIf(token->nextToken);
        }
    }
}