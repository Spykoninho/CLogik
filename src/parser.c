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
    while (input->nextToken != NULL) {
        if(input->type == UNKNOWN) error("unknown token");

        if(input->type == IDENTIFIER) {
            nextToken(input);
            if(input->type != ASSIGN)
                if(!isOperator(input->type)) error("Assignation ou calcul manquant");
            nextToken(input);

            checkCalcul(input);

        }else if(input->type == FUNCTION) {

        }else if(input->type == NUMBER) {

        }else {
            error("Mauvais ordre");
        }
        input=input->nextToken;
    }
    if(strcmp(input->value, ";") != 0) error("; manquant");
}

void error(char *msg) {
    printf("%s\n", msg);
    exit(1);
}

void nextToken(Token *input) {
    input=input->nextToken;
    if(input == NULL) error("Ligne non finie correctement");
}

void checkCalcul(Token *input) {
    Token *temp=input;
    checkParentheses(temp);
    if(input->type != IDENTIFIER && input->type != NUMBER) error("Mauvaise assignation dans checkCalcul");
    nextToken(input);
    while (input->nextToken != NULL && isOperator(input->type)) {
        nextToken(input);
        if(input->type != IDENTIFIER && input->type != NUMBER) error("Mauvaise assignation dans checkCalcul 2");
        nextToken(input);
    }
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
        input=input->nextToken;
    }
    if(parentheseCheck != 0 || accoladeCheck != 0 || crochetCheck != 0) error("Mauvaise gestion des délimiteurs");
}