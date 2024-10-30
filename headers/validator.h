//
// Created by Ylan on 23/10/2024.
//

#ifndef VALIDATOR_H
#define VALIDATOR_H
#include "token.h"

int isValidToParse(Token* TokenList);
int matchType(Type actualType, Type expectedType);
Token* next(Token* TokenList);
int isNumber(Type actualChar);
Token* checkTokenList(Token* TokenList);
Token* operation(Token * TokenList);
#endif //VALIDATOR_H
