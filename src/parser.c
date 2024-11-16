//
// Created by mathi on 04/10/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/token.h"
#include "../headers/parser.h"
#include "../headers/lexer.h"
#include "../headers/print.h"
#include "../headers/condition.h"

Token* checkBlock(Token* input) {
    while (input != NULL && input->type != RBRACE) {
        input = nextToken(input);
    }
    return input;
}

void parser(Token *input) {
    while (input->nextToken != NULL) {
        if(input->type == UNKNOWN) error("unknown token");

        if(input->type == IDENTIFIER) {
            input = nextToken(input);
            if(input->type != ASSIGN)
                error("Assignation manquante.");

            input = nextToken(input);
            if(input->type == TOKENSTRING) {
                if(isOperator(input->nextToken->type)) {
                    input = checkCalcul(input);
                    break;
                }
            }else {
                input = checkCalcul(input);
                break;
            }
            input = nextToken(input);
            break;
        }
        if(input->type == PRINT) {
            input = nextToken(input);
            if(input->type != LPAREN) error("Parentheses gauche manquante");
            input = checkPrint(input);
            if(input->type != RPAREN) error("Parentheses droite manquante");
            input = nextToken(input);
            break;
        }

        if(input->type == IF) {
            input = nextToken(input);
            if(input->type != LPAREN) error("Parenthese gauche manquante pour le 'if'");

            input = checkIf(input);
            if(input->type != LBRACE) error("Accolade gauche manquante après la condition du 'if'");

            input = checkBlock(input);

            if(input->type != RBRACE) error("Accolade droite manquante après le bloc du 'if'");

            input = nextToken(input);
            break;
        }

        error("Entree non prise en charge");
    }
    if(strcmp(input->value, ";") != 0 && strcmp(input->value, "}") != 0) error("; manquant");
}

void error(char *msg) {
    printf("%s\n", msg);
    exit(1);
}

Token * nextToken(Token *input) {
    input=input->nextToken;
    if(input == NULL) error("Ligne non finie correctement");
    return input;
}

Token * checkCalcul(Token *input) {
    checkParentheses(input);
    int modulo2 = 0;
    while (input->nextToken != NULL) {
        if(input->type == LPAREN || input->type == RPAREN) {
            input=nextToken(input);
            continue;
        }
        if(modulo2 % 2 == 0 && isOperator(input->type)) error("Operateur dans le mauvais ordre");
        if(modulo2 % 2 == 1 && (input->type == IDENTIFIER || input->type == NUMBER || input->type==TOKENSTRING)) error("Operandes dans le mauvais ordre");
        if(input->type != IDENTIFIER && input->type != NUMBER && !isOperator(input->type) && input->type!=TOKENSTRING) error("Mauvais type de donnée dans le calcul");
        input = nextToken(input);
        modulo2++;
    }
    return input;
}

void checkParentheses(Token *input) {
    int parentheseCheck = 0;
    int crochetCheck = 0;
    int accoladeCheck = 0;
    while(input->nextToken != NULL) {
        if(strcmp(input->value, "(") == 0) parentheseCheck++;
        if(strcmp(input->value, ")") == 0) {
            parentheseCheck--;
            if(parentheseCheck<0) error("Mauvaise gestion de parenthèse");
        }

        if(strcmp(input->value, "[") == 0) crochetCheck++;
        if(strcmp(input->value, "]") == 0) {
            crochetCheck--;
            error("Mauvaise gestion de crochet");
        }

        input = nextToken(input);
    }
    if(strcmp(input->value, ";") != 0 && strcmp(input->value, "}") != 0) error("; manquant");
    if(parentheseCheck != 0 || accoladeCheck != 0 || crochetCheck != 0) error("Mauvaise gestion des delimiteurs");
}

Token * checkPrint(Token *input) {
    checkParentheses(input);
    int modulo2 = 0;
    while (input->nextToken != NULL) {
        if(input->nextToken->type == SEMICOLON && input->type == RPAREN) {
            return input;
        }
        if(input->type == LPAREN || input->type == RPAREN) {
            input=nextToken(input);
            continue;
        }
        if(modulo2 % 2 == 0 && isOperator(input->type)) error("Operateur dans le mauvais ordre");
        if(modulo2 % 2 == 1 && (input->type == IDENTIFIER || input->type == NUMBER || input->type==TOKENSTRING)) error("Operandes dans le mauvais ordre");
        if(input->type != IDENTIFIER && input->type != NUMBER && !isOperator(input->type) && input->type!=TOKENSTRING) error("Mauvais type de donnée dans le calcul");
        input = nextToken(input);
        modulo2++;
    }
    return input;
}

int isValidateValue(Type type) {
    return type == IDENTIFIER || type == NUMBER || type == FALSE || type == TRUE || type == TOKENSTRING;
}

Token* checkIf(Token* input) {
    checkParentheses(input);  
    int pair = 0;
    while (input != NULL) {
        if (input->type == LBRACE || input->type == RBRACE) {
            return input;
        }
        if (input->type == LPAREN || input->type == RPAREN) {
            input = nextToken(input); 
            continue;
        }

         if (pair % 2 == 0 && isOperator(input->type)) {
            error("Opérateur dans le mauvais ordre dans la condition");
        }
        if (pair % 2 == 1 && (input->type == IDENTIFIER || input->type == NUMBER || input->type == TOKENSTRING)) {
            error("Opérande dans le mauvais ordre dans la condition");
        }

        if (!isValidateValue(input->type) && !isOperator(input->type)) {
            error("Mauvais type de donnée dans la condition du if");
        }

        input = nextToken(input);
        pair++;
    }
    return input;
}
