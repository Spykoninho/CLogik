//
// Created by Ylan on 23/10/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include "../headers/validator.h"

#include "../headers/lexer.h"

int isValidToParse(Token *TokenList, int *error) {
    if (TokenList == NULL) {
        // Si y'a rien, c'est pas valide
        *error = -2;
        return 0;
    }
    printTokens(TokenList);
    const Token *finalToken = handleStatements(TokenList, error); // On check la liste de tokens
    if (finalToken == NULL) {
        // Si la chaine s'est terminée trop tôt (qu'on a consommé tous les tokens, c'est pas valide)
        if (!*error) {
            *error = -1;
        }
        return 0;
    }
    return matchType(finalToken->type, SEMICOLON); // Si on a bien un point virgule à la fin, c'est valide
}

Token *handleStatements(Token *TokenList, int *error) {
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
    TokenList =next(TokenList);
    return handleStatementRest(TokenList, error);
}

Token* handleFirstToken(Token *firstToken, int *error) {
    Type tokenType = firstToken->type;
    if (!matchType(tokenType, LPAREN)) { //TODO : gérer les parenthèses
        int firstTokenIsVariable = isVariable(tokenType); //TODO : rajouter les autres cas ici s'il y en a
        int firstTokenIsFunction = matchType(tokenType, 999999); //FUNCTION
        int firstTokenIsNumber = isNumber(tokenType);
        int firstTokenIsKeyWord = matchType(tokenType, KEYWORD);
        if (!firstTokenIsVariable && !firstTokenIsFunction && !firstTokenIsNumber && !firstTokenIsKeyWord) {
            *error = 1;
        }
    } else {
        firstToken = next(firstToken);
        handleFirstToken(firstToken, error);
        if (*error) {
            return NULL;
        }
        firstToken = next(firstToken);
        if (!matchType(firstToken->type, RPAREN)) { //TODO : gérer les parenthèses
            *error = 56923;
        }
    }
    return firstToken;
}

Token *handleStatementRest(Token *TokenList, int *error) {
    //TODO : refactoriser le code pour la répétition
    Type tokenType = TokenList->type;
    int ok = matchType(tokenType, IDENTIFIER); //TODO : rajouter les autres cas ici
    if (!ok) {
        exit(852);
    }
    TokenList = next(TokenList); // Si c'est bon, on passe au token suivant
    tokenType = TokenList->type;
    ok = matchType(tokenType, ASSIGN);
    if (!ok) {
        exit(2);
    }
    TokenList = next(TokenList);// Si c'est bon, on passe au token suivant
    tokenType = TokenList->type;
    ok = isNumber(tokenType);
    if (!ok) {
        exit(3);
    }
    TokenList = next(TokenList);// Si c'est bon, on passe au token suivant
    TokenList= operation(TokenList);
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
            default:
                printf("Erreur : Erreur inconnue");
                break;
        }
    }
}
