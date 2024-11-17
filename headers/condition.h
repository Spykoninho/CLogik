#ifndef CONDITION_H
#define CONDITION_H

#include "token.h"
#include "lexer.h"
#include "print.h"
#include "variable.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int evaluateIfCondition(Token *token);
void parseBlockIf(Token *token);
void parserIf(Token *token);

#endif