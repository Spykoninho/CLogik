//
// Created by mathi on 01/10/2024.
//

#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Permet de comprendre les entrées
Token * lexer(char*string, Token * token) {
    while (*string != '\0') {
        switch (*string) {
            case '(':
                token = addToken(token, LPAREN, *string);
                break;
            case ')':
                token = addToken(token, RPAREN, *string);
                break;
            case '{':
                token = addToken(token, LBRACE, *string);
                break;
            case '}':
                token = addToken(token, RBRACE, *string);
                break;
            case ';':
                token = addToken(token, SEMICOLON, *string);
                break;
            case '=':
                token = addToken(token, ASSIGN, *string);
                break;
            case '<':
            case '>':
            case '&':
            case '|':
            case '+':
            case '-':
            case '/':
            case '*':
                token = addToken(token, OPERATOR, *string);
        }
        //char * input = strtok(*string, " ");
        //if(strlen(input) >= 2) { // pour réconnaitre les if, print, nombres à plusieurs digits...
            // verif si isDigit
            // stocker
            // avancer du strlen
        //}
        if(*string >= '0' && *string <= '9') {
            token = addToken(token, NUMBER, *string);
            string++;
            continue;
        }
        if(*string >= 'a' && *string <= 'z' || *string >= 'A' && *string <= 'Z') {
            token = addToken(token, IDENTIFIER, *string);
            string++;
            continue;
        }
        string++;
    }
    return token;
}

// Print un token à partir de la fin (pour les mettre dans l'ordre)
void printToken(Token * token) {
    if(token != NULL) {
        printToken(token->nextToken);
        printf("{type: \"%s\", value: \"%c\"},\n", getType(token->type) ,token->value);
    }
}

// Ajouter un token
Token * addToken(Token * token, const Type type, const char value) {
    Token * newToken = malloc(sizeof(Token));
    newToken->type = type;
    newToken->value = value;
    newToken->nextToken = token;
    return newToken;
}

// Permet de récupérer la string de l'enum pour print
char * getType(int type) {
    switch(type) {
        case NUMBER:
            return "NUMBER";
        case KEYWORD:
            return "KEYWORD";
        case LPAREN:
            return "LPAREN";
        case RPAREN:
            return "RPAREN";
        case IDENTIFIER:
            return "IDENTIFIER";
        case OPERATOR:
            return "OPERATOR";
        case LBRACE:
            return "LBRACE";
        case RBRACE:
            return "RBRACE";
        case SEMICOLON:
            return "SEMICOLON";
        case ASSIGN:
            return "ASSIGN";
        default:
            return "UNKNOWN";
    }
}

// Libère les tokens
void freeToken(Token * token) {
    if(token != NULL) {
        freeToken(token->nextToken);
        free(token);
    }
}