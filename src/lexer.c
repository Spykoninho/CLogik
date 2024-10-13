//
// Created by mathi on 01/10/2024.
//

#include "../headers/lexer.h"
#include "../headers/token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Permet de comprendre les entrées
Token *lexer(char *string, Token *token) {
    while (*string != '\0') {
        char stringValue[2] = {*string, '\0'};
        switch (*string) {
            case '(':
                token = addToken(token, LPAREN, stringValue);
                break;
            case ')':
                token = addToken(token, RPAREN, stringValue);
                break;
            case '{':
                token = addToken(token, LBRACE, stringValue);
                break;
            case '}':
                token = addToken(token, RBRACE, stringValue);
                break;
            case ';':
                token = addToken(token, SEMICOLON, stringValue);
                break;
            case '+':
                token = addToken(token, PLUS, stringValue);
                break;
            case '-':
                token = addToken(token, MINUS, stringValue);
                break;
            case '/':
                token = addToken(token, DIV, stringValue);
                break;
            case '*':
                token = addToken(token, MULT, stringValue);
                break;
            case '^':
                token = addToken(token, POW, stringValue);
                break;
            case '"':
                token = addToken(token, QUOTES, stringValue);
                break;
            case '.':
                token = addToken(token, DOT, stringValue);
                break;
            case '=':
                token = addToken(token, ASSIGN, stringValue);
            break;
        }
        // rajouter les < > & | = ! % >= <= == !=
        if (*string >= '0' && *string <= '9') {
            char numberString[255] = "";
            while (*string >= '0' && *string <= '9') {
                char NumberToString[2] = {*string, '\0'};
                strcat(numberString, NumberToString);
                *string++;
            }
            token = addToken(token, NUMBER, numberString);
            continue;
        }

        if (*string >= 'a' && *string <= 'z' || *string >= 'A' && *string <= 'Z') {
            char longString[255] = "";
            while (*string >= 'a' && *string <= 'z' || *string >= 'A' && *string <= 'Z') {
                char charToString[2] = {*string, '\0'};
                strcat(longString, charToString);
                *string++;
            }
            token = addToken(token, IDENTIFIER, longString);
            continue;
        }
        string++;
    }
    return token;
}

// Permet de récupérer la string de l'enum pour print
char *getType(int type) {
    switch (type) {
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
        case PLUS:
            return "PLUS";
        case MINUS:
            return "MINUS";
        case NOT:
            return "NOT";
        case MULT:
            return "MULT";
        case DIV:
            return "DIV";
        case MOD:
            return "MOD";
        case POW:
            return "POW";
        case GREATER:
            return "GREATER";
        case LESS:
            return "LESS";
        case GREATEREQ:
            return "GREATEREQ";
        case LESSEQ:
            return "LESSEQ";
        case EQUAL:
            return "EQUAL";
        case LBRACE:
            return "LBRACE";
        case RBRACE:
            return "RBRACE";
        case SEMICOLON:
            return "SEMICOLON";
        case ASSIGN:
            return "ASSIGN";
        case QUOTES:
            return "QUOTES";
        case DOT:
            return "DOT";
        default:
            return "UNKNOWN";
    }
}
