//
// Created by mathi on 07/10/2024.
//

#ifndef VARIABLE_H
#define VARIABLE_H
#include "token.h"
typedef enum {
    STRING,
    INT,
    DOUBLE,
    CHAR,
}VarType;

typedef struct Variable{
    VarType type;
    char * name;
    char * value;
    struct Variable * nextVar;
}Var;

Var * addVariable(char * input);

void freeVariable(Var * variable);

Var * getVariable(Var *var);

void printVariable(Var *var);

#endif //VARIABLE_H
