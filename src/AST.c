//
// Created by thier on 12/11/2024.
//


#include "../headers/AST.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

ASTNode *createOperationNode(char operator, ASTNode *left, ASTNode *right) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_TYPE_OPERATION;
    node->operation.operator = operator;
    node->operation.left = left;
    node->operation.right = right;
    return node;
}

ASTNode *createNumberNode(double value) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_TYPE_NUMBER;
    node->number = value;
    return node;
}

ASTNode *createVariableNode(const char *name) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_TYPE_VARIABLE;
    node->variableName = strdup(name);
    return node;
}


void printAST(ASTNode *node) {
    if (node == NULL) return;

    switch (node->type) {
        case NODE_TYPE_NUMBER:
            printf("%lf", node->number);
            break;
        case NODE_TYPE_OPERATION:
            printf("(");
            printAST(node->operation.left);
            printf(" %c ", node->operation.operator);
            printAST(node->operation.right);
            printf(")");
            break;
    }
}

void freeAST(ASTNode *node) {
    if (node == NULL) return;
    if (node->type == NODE_TYPE_OPERATION) {
        freeAST(node->operation.left);
        freeAST(node->operation.right);
    }
    free(node);
}


void generateNodeLabel(char *buffer, int *counter) {
    sprintf(buffer, "N%d", (*counter)++);
}

// Fonction principale pour afficher l'AST au format Graphviz
void printASTGraphviz(ASTNode *node, int *counter) {
    if (node == NULL) return;

    char currentLabel[10];
    generateNodeLabel(currentLabel, counter); // Crée un label unique pour le nœud actuel

    // Affiche le nœud actuel en fonction de son type
    switch (node->type) {
        case NODE_TYPE_NUMBER:
            printf("%s [label=\"%lf\"];\n", currentLabel, node->number);
            break;
        case NODE_TYPE_VARIABLE:
            printf("%s [label=\"%s\"];\n", currentLabel, node->variableName);
            break;
        case NODE_TYPE_OPERATION: {
            printf("%s [label=\"%c\"];\n", currentLabel, node->operation.operator);

            char leftLabel[10], rightLabel[10];
            generateNodeLabel(leftLabel, counter);
            generateNodeLabel(rightLabel, counter);

            // Appelle récursivement la fonction pour afficher les sous-nœuds
            printASTGraphviz(node->operation.left, counter);
            printASTGraphviz(node->operation.right, counter);

            // Affiche les connexions entre le nœud actuel et ses sous-nœuds
            printf("%s --> %s;\n", currentLabel, leftLabel);
            printf("%s --> %s;\n", currentLabel, rightLabel);
            break;
        }
        case NODE_TYPE_IF:
            printf("%s [label=\"if\"];\n", currentLabel);
            printASTGraphviz(node->controlFlow.condition, counter);
            printf("%s --> %s;\n", currentLabel, "Condition");

            if (node->controlFlow.body) {
                printf("%s --> %s;\n", currentLabel, "Then");
                printASTGraphviz(node->controlFlow.body, counter);
            }
            if (node->controlFlow.elseBody) {
                printf("%s --> %s;\n", currentLabel, "Else");
                printASTGraphviz(node->controlFlow.elseBody, counter);
            }
            break;

        case NODE_TYPE_WHILE:
            printf("%s [label=\"while\"];\n", currentLabel);
            printASTGraphviz(node->controlFlow.condition, counter);
            printf("%s --> %s;\n", currentLabel, "Condition");
            if (node->controlFlow.body) {
                printf("%s --> %s;\n", currentLabel, "Body");
                printASTGraphviz(node->controlFlow.body, counter);
            }
            break;

        case NODE_TYPE_PRINT:
            printf("%s [label=\"print\"];\n", currentLabel);
            printASTGraphviz(node->print.expression, counter);
            printf("%s --> %s;\n", currentLabel, "Expression");
            break;
    }
}

// Fonction principale pour afficher l'AST
void displayASTGraph(ASTNode *root) {
    int counter = 0; // Compteur de labels de nœuds pour garantir l'unicité
    printf("graph TD\n"); // Préambule pour le format Graphviz
    printASTGraphviz(root, &counter);
}
