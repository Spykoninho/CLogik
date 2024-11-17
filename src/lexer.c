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
            case '.':
                token = addToken(token, DOT, stringValue);
                break;
            case '=':
                if (*(string + 1) == '=') {
                    token = addToken(token, EQUAL, "==");
                    string++;
                } else {
                    token = addToken(token, ASSIGN, stringValue);
                }
                break;
            case '>':
                if (*(string + 1) == '=') {
                    token = addToken(token, GREATEREQ, ">=");
                    string++;
                } else {
                    token = addToken(token, GREATER, stringValue);
                }
                break;
            case '<':
                if (*(string + 1) == '=') {
                    token = addToken(token, LESSEQ, "<=");
                    string++;
                } else {
                    token = addToken(token, LESS, stringValue);
                }
                break;
            case '!':
                if (*(string + 1) == '=') {
                    token = addToken(token, NOTEQUAL, "!=");
                    string++;
                } else {
                    token = addToken(token, NOT, stringValue);
                }
        }
        if (*string >= '0' && *string <= '9') {
            char numberString[255] = "";
            while (*string >= '0' && *string <= '9' || *string == '.') {
                char NumberToString[2] = {*string, '\0'};
                strcat(numberString, NumberToString);
                *string++;
            }
            token = addToken(token, NUMBER, numberString);
            continue;
        }

        if (*string == '"') {
            *string++;
            char longString[255] = "";
            while (*string != '"' && *string != '\0') {
                char charToString[2] = {*string, '\0'};
                strcat(longString, charToString);
                string++;
            }
            if (*string == '\0') {
                printf("Error lexer : il manque une \"");
                exit(1);
            }
            *string++;
            token = addToken(token, TOKENSTRING, longString);
            continue;
        }

        if (*string >= 'a' && *string <= 'z' || *string >= 'A' && *string <= 'Z') {
            char longString[255] = "";
            while (*string >= 'a' && *string <= 'z' || *string >= 'A' && *string <= 'Z' || *string >= '0' && *string <=
                   '9') {
                char charToString[2] = {*string, '\0'};
                strcat(longString, charToString);
                *string++;
            }
            if (strcmp(longString, "AST") == 0) {
                token = addToken(token, AST, longString);
            }else if (strcmp(longString, "print") == 0) {
                token = addToken(token, PRINT, longString);
            } else if (strcmp(longString, "while") == 0) {
                token = addToken(token, KEYWORD, longString); // Ajoute le mot-clé "while"
            } else if (strcmp(longString, "if") == 0) {
                token = addToken(token, IF, longString);
                continue;
            } else if (strcmp(longString, "else") == 0) {
                token = addToken(token, ELSE, longString);
                continue;
            } else if (strcmp(longString, "true") == 0) {
                token = addToken(token, TRUE, longString);
                continue;
            } else if (strcmp(longString, "false") == 0) {
                token = addToken(token, FALSE, longString);
                continue;
            } else {
                token = addToken(token, IDENTIFIER, longString); // Traite comme un identifiant normal
            }
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
        case TOKENSTRING:
            return "TOKENSTRING";
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
        case NOTEQUAL:
            return "NOTEQUAL";
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
        case DOT:
            return "DOT";
        case PRINT:
            return "PRINT";
        default:
            return "UNKNOWN";
    }
}
