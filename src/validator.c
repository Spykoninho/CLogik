//
// Created by Ylan on 23/10/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include "../headers/validator.h"

#include <string.h>

#include "../headers/lexer.h"

int isValidToParse(Token *TokenList, int *error) {
    if (TokenList == NULL) {
        // Si y'a rien, c'est pas valide
        *error = -2;
        return 0;
    }
    printTokens(TokenList);
    // const Token *finalToken = handleStatements(TokenList, error); // On check la liste de tokens
    const Token *finalToken = startHandling(TokenList, error); // On check la liste de tokens
    if (finalToken == NULL) {
        // Si la chaine s'est terminée trop tôt (qu'on a consommé tous les tokens, c'est pas valide)
        if (!*error) {
            *error = -1;
        }
        return 0;
    }
    return matchType(finalToken->type, SEMICOLON); // Si on a bien un point virgule à la fin, c'est valide
}

/*Token *handleStatements(Token *TokenList, int *error) {
    Token *newToken = handleStatement(TokenList, error); // On check le premier statement
    if (newToken == NULL) {
        return NULL;
    }
    if (matchType(newToken->type, SEMICOLON)) {
        // la première statement est ok
        if (newToken->nextToken == NULL) {
            return newToken;
        }
        return handleStatements(newToken->nextToken, error); // Si on a un point virgule, on check le prochain statement
    }
}

Token *handleStatement(Token *TokenList, int *error) {
    TokenList = handleFirstToken(TokenList, error);
    if (*error || TokenList == NULL) {
        return NULL;
    }
    TokenList = next(TokenList);
    return handleStatementRest(TokenList, error);
}*/

Token * startHandling(Token*  token, int* error) {

    Token *finalToken = handleExpression(token, error);
    if (*error) {
        return NULL;
    }
    int ok = matchType(finalToken->type, SEMICOLON);
    if (!ok) {
        *error = 7;
    }
    if (!*error && finalToken->nextToken != NULL) {
        return startHandling(finalToken->nextToken, error);
    }
    if (*error) {
        return NULL;
    }
    return finalToken;

}

Token *handleElement(Token *token, int *error) {
    if (strcmp(token->value, "(") == 0) {
        matchType(token->type, LPAREN);
        Token *newToken = next(token);
        newToken = handleExpression(newToken, error);
        if (*error) {
            return NULL;
        }
        int ok= matchType(newToken->type, RPAREN);
        if (!ok) {
             *error =6;
        }
    } else {
        Type tokenType = token->type;
        int firstTokenIsVariable = isVariable(tokenType);
        int firstTokenIsNumber = isNumber(tokenType);
        if (!firstTokenIsVariable && !firstTokenIsNumber && !nextTokenIsParenthesis(token)) {
            *error = 1;
        }
    }
    return next(token);
}
int nextTokenIsParenthesis(Token* token) {
    return matchType(token->nextToken->type, RPAREN);
}
Token *handleExpression(Token *firstToken, int *error) {
    Token* token = handleTerm(firstToken, error);
    if (*error) {
        return NULL;
    }
    token = handleRestTerm(token, error);
    return token;
}

Token* handleTerm(Token* token, int* error) {
    Token* newToken = handleElement(token, error);
    if (*error) {
        return NULL;
    }
    newToken = handleRestTerm(newToken, error);
    return newToken;
}

Token* handleRestTerm(Token* token, int* error) {
    Type tokenType = token->type;
    int isOp = isOperator(tokenType);
    if (isOp) {
        Token *newToken = next(token);
        newToken= handleElement(newToken, error);
        if (*error) {
            return NULL;
        }
        newToken = handleRestTerm(newToken, error);
        token = newToken;
    }
    return token;
}

Token *handleStatementRest(Token *TokenList, int *error) {
    //TODO : refactoriser le code pour la répétition
    Type tokenType = TokenList->type;
    int isAssignation = matchType(tokenType, ASSIGN);
    if (isAssignation) {
        TokenList = next(TokenList); // Si c'est bon, on passe au token suivant
        tokenType = TokenList->type;
        int assigningNum = isNumber(tokenType);
        int tokenIsVariable = matchType(tokenType, IDENTIFIER);
        if (!assigningNum && !tokenIsVariable) {
            *error = 2;
            return NULL;
        }
        int varExists = 1; //TODO : check si la variable a été déclarée (isVarExists)
        if (!varExists) {
            *error = 3;
            return NULL;
        }
        TokenList = next(TokenList); // Si c'est bon, on passe au token suivant
    }
    TokenList = operation(TokenList, error);
    return TokenList;
}

Token *operation(Token *TokenList, int *error) {
    // permet de check si on a une opération
    Token *newToken = NULL; // newToken permet de ranger le token suivant si on appelle la fonction récursivement
    if (TokenList == NULL) {
        // check au cas où l'opération a mal été faite (ex : 1 + 1 mais pas de ;)
        return NULL;
    }
    Type actualChar = TokenList->type;
    if (isOperator(actualChar)) {
        newToken = next(TokenList);
        if (newToken == NULL) {
            // check au cas où l'opération a mal été faite encore (ex : 1 + )
            *error = 4;
            return NULL;
        }
        actualChar = newToken->type;
        if (!isNumber(actualChar) && !isVariable(actualChar)) {
            //TODO : si c'est une variable, on doit vérifier qu'elle a été déclarée
            *error = 5;

            return NULL;
        }
        newToken = next(newToken);
        newToken = operation(newToken, error); // Si c'est une longue opération, faut le prendre en compte
    } else {
        return TokenList;
    }
    return newToken;
}

int isNumber(Type actualChar) {
    return matchType(actualChar, NUMBER);
}

int isVariable(Type actualChar) {
    return matchType(actualChar, IDENTIFIER);
}

int matchType(Type actualChar, Type expectedChar) {
    if (actualChar == expectedChar) {
        return 1;
    }
    return 0;
}

Token *next(Token *TokenList) {
    return TokenList->nextToken;
}

void printReturn(int ok, int error) {
    if (ok) {
        printf("ok pour parse");
    } else {
        printf("pas ok pour parse, erreur %d\n", error);
        switch (error) {
            case -2:
                printf("Erreur : Chaine vide");
                break;
            case -1:
                printf("Erreur : Chaine malformee");
                break;
            case 1:
                printf("Erreur : Attendu une variable, un nombre ou un mot cle");
                break;
            case 2:
                printf("Erreur : Attendu une assignation (manque valeur a droite de la variable)");
                break;
            case 3:
                printf("Erreur : Variable non declaree");
                break;
            case 4:
                printf("Erreur : Attendu une valeur apres l'operateur");
                break;
            case 5:
                printf("Erreur : Caractère inconnu apres un operateur");
                break;
            case 6:
                printf("Erreur : Parenthese fermante non détectée");
                break;
            default:
                printf("Erreur : Erreur inconnue");
                break;
        }
    }
}


