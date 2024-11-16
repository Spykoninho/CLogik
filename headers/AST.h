//
// Created by thier on 12/11/2024.
//

#ifndef AST_H
#define AST_H


typedef enum {
    NODE_TYPE_IF = 1,
    NODE_TYPE_WHILE,
    NODE_TYPE_PRINT,
    NODE_TYPE_OPERATION,
    NODE_TYPE_NUMBER,
    NODE_TYPE_VARIABLE
} NodeType;



typedef struct ASTNode {
    NodeType type;
    union {
        struct {  //calcul
            struct ASTNode *left;
            struct ASTNode *right;
            char operator;
        } operation;

        struct {  //if while
            struct ASTNode *condition;
            struct ASTNode *body;
            struct ASTNode *elseBody;
        } controlFlow;

        struct {  // print
            struct ASTNode *expression;
        } print;

        double number;
        char *variableName;
    };
} ASTNode;


ASTNode *createOperationNode(char operator, ASTNode *left, ASTNode *right);

ASTNode *createNumberNode(double value);

ASTNode *createVariableNode(const char *name);

void printAST(ASTNode *node);

void freeAST(ASTNode *node);

void generateNodeLabel(char *buffer, int *counter);

void printASTGraphviz(ASTNode *node, int *counter);

void displayASTGraph(ASTNode *root);

#endif //AST_H
