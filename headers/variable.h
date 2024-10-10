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
    CALCUL
}VarType;

typedef struct Variable{
    VarType type;
    char * name;
    char * value;
    struct Variable * nextVar;
}Var;

Var * addVariable(char * input, Var *headVar);

void freeVariable(Var * variable);

Var * getVariable(Var *var, char *searchedVar);

void printVariables(Var *var);

char * getVarType(int type);

#endif //VARIABLE_H
