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

void parser(Token *input) {
    printTokens(input);
    while (input->nextToken != NULL) {
        if(input->type == UNKNOWN) error("unknown token");

        if(input->type == IDENTIFIER) {
            input = nextToken(input);
            if(input->type != ASSIGN)
                if(!isOperator(input->type)) error("Assignation ou calcul manquant");

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
        }else if(input->type == NUMBER) {
            // verif quand on met un calcul random
        }else if(input->type == SEMICOLON) {
            if(input->nextToken != NULL) error("Mauvais ordre");
        }else {
            printf("error at : %s\n", input->value);
            error("Mauvais ordre 2 :");
        }
        input=nextToken(input);
    }
    if(strcmp(input->value, ";") != 0) error("; manquant");
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

        if(strcmp(input->value, "}") == 0) accoladeCheck++;
        if(strcmp(input->value, "{") == 0) {
            accoladeCheck--;
            error("Mauvaise gestion de accolade");
        }
        input = nextToken(input);
    }
    if(strcmp(input->value, ";") != 0) error("; manquant");
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