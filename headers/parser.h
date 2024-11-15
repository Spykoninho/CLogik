//
// Created by mathi on 04/10/2024.
//

#ifndef PARSER_H
#define PARSER_H
#include "token.h"

void parser(Token *token);

int calculShuttingYard(char *input);

void error(char *msg);

Token * nextToken(Token *input);

Token * checkCalcul(Token *input);

void checkParentheses(Token *input);

Token * checkPrint(Token *input);
#endif //PARSER_H
