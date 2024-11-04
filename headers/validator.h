//
// Created by Ylan on 23/10/2024.
//

#ifndef VALIDATOR_H
#define VALIDATOR_H
#include "token.h"

Token *handleStatements(Token *TokenList, int * error);
Token *handleStatement(Token *TokenList, int * error);
Token*  handleFirstToken(Token *firstToken, int * error);
Token *handleStatementRest(Token *TokenList, int * error);
int isValidToParse(Token *TokenList, int* error);
int matchType(Type actualType, Type expectedType);
Token* next(Token* TokenList);
int isNumber(Type actualChar);
int isVariable(Type actualChar);
Token* operation(Token * TokenList, int* error);
void printReturn(int ok, int error);
#endif //VALIDATOR_H
