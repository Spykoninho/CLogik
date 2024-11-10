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
} VarType;

typedef struct Variable {
    VarType type;
    char *name;
    char *value;
    struct Variable *nextVar;
} Var;

extern Var* variables;

Var *addVariable(Token *input);

void freeVariable(Var *variable);

Var *getVariable(Var *var, char *searchedVar);

void printVariables(Var *var);

char *getVarType(int type);

int getListLength(Token *token);

int isVarExists(Var *var, const char *searchedVar);

int isTokenDouble(char *value);
#endif //VARIABLE_H
