//
// Created by mathi on 04/10/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/token.h"
#include "../headers/parser.h"
#include "../headers/lexer.h"
#include "../headers/print.h"
#include "../headers/AST.h"

void parser(Token *input) {
    while (input->nextToken != NULL) {
        if(input->type == UNKNOWN) error("unknown token");

        if(input->type == IDENTIFIER) {
            input = nextToken(input);
            if(input->type != ASSIGN)
                error("Assignation manquante.");

            input = nextToken(input);
            if(input->type == TOKENSTRING) {
                if(isOperator(input->nextToken->type)) {
                    input = checkCalcul(input);
                    break;
                }
            }else {
                input = checkCalcul(input);
                break;
            }
            input = nextToken(input);
            break;
        }
        if(input->type == PRINT) {
            input = nextToken(input);
            if(input->type != LPAREN) error("Parentheses gauche manquante");
            input = checkPrint(input);
            if(input->type != RPAREN) error("Parentheses droite manquante");
            input = nextToken(input);
            break;
        }
        if (input->type == KEYWORD) {
            if (strcmp(input->value, "while") == 0) {
                ASTNode *whileNode = parseWhile(&input);
                evaluateAST(whileNode);
                freeAST(whileNode); // Libère la mémoire
                continue; // Passe à la prochaine instruction
            }
        }

        if(input->type == AST) {
            input = nextToken(input);
            continue;
        }

        printf("%s\n", input->value);
        error("Entree non prise en charge");
    }
    if(strcmp(input->value, ";") != 0) error("; manquant");
}

Token *parseWhile(Token **currentToken) {
    if ((*currentToken)->type != KEYWORD || strcmp((*currentToken)->value, "while") != 0) {
        printf("Erreur : 'while' attendu\n");
        exit(1);
    }

    *currentToken = (*currentToken)->nextToken;

    if ((*currentToken)->type != LPAREN) {
        printf("Erreur : parenthèse gauche manquante après 'while'\n");
        exit(1);
    }

    *currentToken = (*currentToken)->nextToken;
    Token *startToken = *currentToken; // Début de la condition

    while ((*currentToken)->type != RPAREN) {
        if (*currentToken == NULL) {
            printf("Erreur : parenthèse droite manquante\n");
            exit(1);
        }
        *currentToken = (*currentToken)->nextToken;
    }

    *currentToken = (*currentToken)->nextToken; // Passe la parenthèse droite

    if ((*currentToken)->type != LBRACE) {
        printf("Erreur : accolade gauche manquante pour le corps du 'while'\n");
        exit(1);
    }

    *currentToken = (*currentToken)->nextToken; // Passe l'accolade gauche

    while ((*currentToken)->type != RBRACE) {
        if (*currentToken == NULL) {
            printf("Erreur : accolade droite manquante\n");
            exit(1);
        }
        *currentToken = (*currentToken)->nextToken;
    }

    *currentToken = (*currentToken)->nextToken; // Passe l'accolade droite

    return startToken; // Retourne le premier token de la condition
}



ASTNode *parseExpression(Token **currentToken) {
    ASTNode *left = createVariableOrNumberNode(currentToken);

    if (isOperator((*currentToken)->type)) {
        char operator = (*currentToken)->value[0];
        *currentToken = (*currentToken)->nextToken;
        ASTNode *right = parseExpression(currentToken);

        ASTNode *operationNode = malloc(sizeof(ASTNode));
        if (operationNode == NULL) {
            printf("Erreur : allocation mémoire échouée pour le nœud d'opération\n");
            exit(1);
        }

        operationNode->type = NODE_TYPE_OPERATION; // Affecte explicitement le type
        operationNode->operation.operator = operator;
        operationNode->operation.left = left;
        operationNode->operation.right = right;

        return operationNode;
    }

    return left;
}


void validateNodeType(NodeType type) {
    if (type < NODE_TYPE_IF || type > NODE_TYPE_VARIABLE) {
        printf("Erreur : type de nœud invalide (%d) détecté lors de la création\n", type);
        exit(1);
    }
}




ASTNode *parseBlock(Token **currentToken) {
    if ((*currentToken)->type != LBRACE) {
        error("Accolade gauche manquante pour le bloc");
    }

    *currentToken = (*currentToken)->nextToken; // Passe l'accolade gauche

    ASTNode *block = NULL; // Liste des instructions dans le bloc
    while ((*currentToken)->type != RBRACE) {
        ASTNode *statement = parseStatement(currentToken); // Analyse une instruction
        block = addToBlock(block, statement); // Ajoute l'instruction au bloc
        *currentToken = (*currentToken)->nextToken;
    }

    return block;
}


ASTNode *parseStatement(Token **currentToken) {
    if (*currentToken == NULL) {
        printf("Erreur : token NULL dans parseStatement\n");
        exit(1);
    }

    if ((*currentToken)->type == IDENTIFIER) {
        Token *varToken = *currentToken;
        *currentToken = (*currentToken)->nextToken;

        if ((*currentToken)->type != ASSIGN) {
            printf("Erreur : '=' attendu après la variable\n");
            exit(1);
        }

        *currentToken = (*currentToken)->nextToken;

        // Analyse l'expression après le '='
        ASTNode *valueNode = parseExpression(currentToken);

        // Crée un nœud pour l'assignation
        ASTNode *assignNode = malloc(sizeof(ASTNode));
        assignNode->type = NODE_TYPE_VARIABLE;
        assignNode->variableName = strdup(varToken->value);
        assignNode->operation.left = valueNode;

        return assignNode;
    }


    if ((*currentToken)->type == KEYWORD) {
        if (strcmp((*currentToken)->value, "while") == 0) {
            return parseWhile(currentToken);
        }
    }

    if ((*currentToken)->type == PRINT) {
        // Gestion d'un `print`
        *currentToken = (*currentToken)->nextToken; // Passe le mot-clé `print`
        ASTNode *expression = parseExpression(currentToken);
        ASTNode *printNode = malloc(sizeof(ASTNode));
        printNode->type = NODE_TYPE_PRINT;
        printNode->print.expression = expression;

        return printNode;
    }

    printf("Erreur : instruction non reconnue dans parseStatement\n");
    exit(1);
}


ASTNode *addToBlock(ASTNode *block, ASTNode *statement) {
    if (block == NULL) {
        // Si le bloc est vide, initialise-le avec la première instruction
        return statement;
    }

    // Si le bloc existe déjà, ajoute l'instruction à la fin
    ASTNode *current = block;
    while (current->operation.right != NULL) {
        current = current->operation.right;
    }
    current->operation.right = statement;
    return block;
}


double evaluateAST(ASTNode *node) {
    if (node == NULL) {
        printf("Erreur : nœud AST NULL\n");
        return 0;
    }

    switch (node->type) {
        case NODE_TYPE_NUMBER:
            return node->number;

        case NODE_TYPE_VARIABLE: {
            // Gestion des variables avec assignation
            if (node->operation.left) { // C'est une assignation (x = ...)
                double value = evaluateAST(node->operation.left);
                Var *var = getVariable(variables, node->variableName);
                if (var) {
                    // Met à jour la valeur existante
                    free(var->value);
                    var->value = malloc(32); // Alloue suffisamment de mémoire
                    sprintf(var->value, "%g", value);
                } else {
                    // Crée une nouvelle variable
                    var = malloc(sizeof(Var));
                    var->type = DOUBLE; // Suppose que c'est un double
                    var->name = strdup(node->variableName);
                    var->value = malloc(32);
                    sprintf(var->value, "%g", value);
                    var->nextVar = variables;
                    variables = var;
                }
                return value;
            }

            // Si ce n'est pas une assignation, retourne la valeur de la variable
            Var *var = getVariable(variables, node->variableName);
            if (!var) {
                printf("Erreur : variable '%s' non définie\n", node->variableName);
                exit(1);
            }
            return atof(var->value);
        }

        case NODE_TYPE_OPERATION: {
            double leftValue = evaluateAST(node->operation.left);
            double rightValue = evaluateAST(node->operation.right);
            switch (node->operation.operator) {
                case '+': return leftValue + rightValue;
                case '-': return leftValue - rightValue;
                case '*': return leftValue * rightValue;
                case '/':
                    if (rightValue == 0) {
                        printf("Erreur : division par zéro\n");
                        return 0;
                    }
                return leftValue / rightValue;
                case '>': return leftValue > rightValue; // Support de '>'
                case '<': return leftValue < rightValue;
                case '=': return leftValue == rightValue;
                case '!': return leftValue != rightValue;
                default:
                    printf("Erreur : opérateur inconnu '%c'\n", node->operation.operator);
                return 0;
            }
        }

        case NODE_TYPE_IF:
            if (evaluateAST(node->controlFlow.condition)) {
                return evaluateAST(node->controlFlow.body);
            } else if (node->controlFlow.elseBody) {
                return evaluateAST(node->controlFlow.elseBody);
            }
            return 0;

        case NODE_TYPE_WHILE: {
            double result = 0;
            while (evaluateAST(node->controlFlow.condition)) {
                result = evaluateAST(node->controlFlow.body);
            }
            return result;
        }

        case NODE_TYPE_PRINT: {
            double value = evaluateAST(node->print.expression);
            printf("Impression : %g\n", value);
            return value;
        }

        default:
            printf("Erreur : type de nœud inconnu\n");
            printf("Type de nœud : %d\n", node->type); // Ajoute ceci pour afficher le type du nœud
            exit(1);

    }
}


ASTNode *createVariableOrNumberNode(Token **currentToken) {
    if (*currentToken == NULL) {
        printf("Erreur : Token NULL dans createVariableOrNumberNode\n");
        exit(1);
    }

    ASTNode *node = NULL;

    if ((*currentToken)->type == IDENTIFIER) {
        // Crée un nœud pour une variable
        node = malloc(sizeof(ASTNode));
        if (node == NULL) {
            printf("Erreur d'allocation mémoire pour le nœud AST\n");
            exit(1);
        }
        node->type = NODE_TYPE_VARIABLE;
        node->variableName = strdup((*currentToken)->value);

    } else if ((*currentToken)->type == NUMBER) {
        // Crée un nœud pour un nombre
        node = malloc(sizeof(ASTNode));
        if (node == NULL) {
            printf("Erreur d'allocation mémoire pour le nœud AST\n");
            exit(1);
        }
        node->type = NODE_TYPE_NUMBER;
        node->number = atof((*currentToken)->value);

    } else if ((*currentToken)->type == LPAREN) {
        // Gère une expression entre parenthèses
        *currentToken = (*currentToken)->nextToken; // Passe la parenthèse gauche
        node = parseExpression(currentToken);       // Analyse l'expression dans les parenthèses

        if ((*currentToken)->type != RPAREN) {
            printf("Erreur : parenthèse droite manquante dans l'expression\n");
            exit(1);
        }
    } else {
        printf("Erreur : type de token inattendu dans createVariableOrNumberNode\n");
        printf("Type de token : %s, Valeur : %s\n", getType((*currentToken)->type), (*currentToken)->value);
        exit(1);
    }

    // Passe au prochain token
    *currentToken = (*currentToken)->nextToken;

    return node;
}

void error(char *msg) {
    printf("%s\n", msg);
    exit(1);
}

Token * nextToken(Token *input) {
    input=input->nextToken;
    if(input == NULL) error("Ligne non finie correctement");
    return input;
}

Token * checkCalcul(Token *input) {
    checkParentheses(input);
    int modulo2 = 0;
    while (input->nextToken != NULL) {
        if(input->type == LPAREN || input->type == RPAREN) {
            input=nextToken(input);
            continue;
        }
        if(modulo2 % 2 == 0 && isOperator(input->type)) error("Operateur dans le mauvais ordre");
        if(modulo2 % 2 == 1 && (input->type == IDENTIFIER || input->type == NUMBER || input->type==TOKENSTRING)) error("Operandes dans le mauvais ordre");
        if(input->type != IDENTIFIER && input->type != NUMBER && !isOperator(input->type) && input->type!=TOKENSTRING) error("Mauvais type de donnée dans le calcul");
        input = nextToken(input);
        modulo2++;
    }
    return input;
}

void checkParentheses(Token *input) {
    int parentheseCheck = 0;
    int crochetCheck = 0;
    int accoladeCheck = 0;
    while(input->nextToken != NULL) {
        if(strcmp(input->value, "(") == 0) parentheseCheck++;
        if(strcmp(input->value, ")") == 0) {
            parentheseCheck--;
            if(parentheseCheck<0) error("Mauvaise gestion de parenthèse");
        }

        if(strcmp(input->value, "[") == 0) crochetCheck++;
        if(strcmp(input->value, "]") == 0) {
            crochetCheck--;
            error("Mauvaise gestion de crochet");
        }

        if(strcmp(input->value, "}") == 0) accoladeCheck++;
        if(strcmp(input->value, "{") == 0) {
            accoladeCheck--;
            error("Mauvaise gestion de accolade");
        }
        input = nextToken(input);
    }
    if(strcmp(input->value, ";") != 0) error("; manquant");
    if(parentheseCheck != 0 || accoladeCheck != 0 || crochetCheck != 0) error("Mauvaise gestion des delimiteurs");
}

Token * checkPrint(Token *input) {
    checkParentheses(input);
    int modulo2 = 0;
    while (input->nextToken != NULL) {
        if(input->nextToken->type == SEMICOLON && input->type == RPAREN) {
            return input;
        }
        if(input->type == LPAREN || input->type == RPAREN) {
            input=nextToken(input);
            continue;
        }
        if(modulo2 % 2 == 0 && isOperator(input->type)) error("Operateur dans le mauvais ordre");
        if(modulo2 % 2 == 1 && (input->type == IDENTIFIER || input->type == NUMBER || input->type==TOKENSTRING)) error("Operandes dans le mauvais ordre");
        if(input->type != IDENTIFIER && input->type != NUMBER && !isOperator(input->type) && input->type!=TOKENSTRING) error("Mauvais type de donnée dans le calcul");
        input = nextToken(input);
        modulo2++;
    }
    return input;
}