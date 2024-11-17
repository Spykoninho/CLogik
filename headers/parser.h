//
// Created by mathi on 04/10/2024.
//

#ifndef PARSER_H
#define PARSER_H
#include "token.h"
#include "AST.h"
#include "variable.h"

void parser(Token *token);

int calculShuttingYard(char *input);

void error(char *msg);

Token * nextToken(Token *input);

Token * checkCalcul(Token *input);

void checkParentheses(Token *input);

Token * checkPrint(Token *input);

Token* checkIf(Token *input);


// Analyse un nœud de type `while` et retourne le prochain token
Token *parseWhile(Token **currentToken);

// Analyse une instruction unique et retourne l'AST correspondant
ASTNode *parseStatement(Token **currentToken);

// Ajoute une instruction dans un bloc AST
ASTNode *addToBlock(ASTNode *block, ASTNode *statement);

// Évalue un AST complet ou partiel
double evaluateAST(ASTNode *node);

// Analyse un bloc de code entre accolades et retourne l'AST correspondant
ASTNode *parseBlock(Token **currentToken);

// Analyse une expression et retourne l'AST correspondant
ASTNode *parseExpression(Token **currentToken);

ASTNode *createVariableOrNumberNode(Token **currentToken);

void validateNodeType(NodeType type);



#endif //PARSER_H
