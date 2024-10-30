//
// Created by Ylan on 23/10/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include "../headers/validator.h"

#include "../headers/lexer.h"

int isValidToParse(Token *TokenList) {
    if (TokenList == NULL) { // Si y'a rien, c'est pas valide
        return 0;
    }
    const Token *finalToken = checkTokenList(TokenList);
    if (finalToken == NULL) { // Si la chaine s'est terminée trop tôt (qu'on a consommé tous les tokens, c'est pas valide)
        return 0;
    }
    return matchType(finalToken->type, SEMICOLON); // Si on a bien un point virgule à la fin, c'est valide
}

Token *checkTokenList(Token *TokenList) {
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

Token* operation(Token * TokenList) {// permet de check si on a une opération
    Token* newToken = NULL; // newToken permet de ranger le token suivant si on appelle la fonction récursivement
    if (TokenList == NULL) { // check au cas où l'opération a mal été faite (ex : 1 + 1 mais pas de ;)
        return NULL;
    }
    Type actualChar = TokenList->type;
    if (isOperator(actualChar)) {
        newToken = next(TokenList);
        if (newToken == NULL) {// check au cas où l'opération a mal été faite encore (ex : 1 + )
            exit(123);
        }
        actualChar = newToken->type;
        if (!isNumber(actualChar)) { //TODO : si c'est une variable, on doit vérifier qu'elle a été déclarée
            exit(8745623);
        }
        newToken = next(newToken);
        newToken = operation(newToken); // Si c'est une longue opération, faut le prendre en compte
    } else {
        return TokenList;
    }
    return newToken;
}

int isNumber(Type actualChar) {
    return matchType(actualChar, NUMBER);
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
