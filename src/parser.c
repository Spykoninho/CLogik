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
    if(input->type == RBRACE || input->type == LBRACE) {
        if(input->nextToken != NULL) error("Erreur, syntaxe incorrecte");
        return;
    }
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
                return;
                // input = parseWhile(&input);
                if (input == NULL) {
                    return;
                }
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
    ASTNode *condition = parseExpression(currentToken); // Parse condition

    if ((*currentToken)->type != RPAREN) {
        printf("Erreur : parenthèse droite manquante après la condition\n");
        exit(1);
    }

    *currentToken = (*currentToken)->nextToken;

    ASTNode *body = parseBlock(currentToken); // Parse body

    ASTNode *whileNode = malloc(sizeof(ASTNode)); // Allocation memoire
    if (whileNode == NULL) { // Verification de nullite
        printf("Erreur : allocation memoire echouee pour le noeud while\n");
        exit(1);
    }

    whileNode->type = NODE_TYPE_WHILE;
    validateNodeType(whileNode->type); // Valider que le type est correct
    whileNode->controlFlow.condition = condition;
    whileNode->controlFlow.body = body;

    if (condition == NULL) {
        printf("Erreur : condition du noeud WHILE non definie\n");
        exit(1);
    }
    if (body == NULL) {
        printf("Erreur : corps du noeud WHILE non defini\n");
        exit(1);
    }

    // evaluer immediatement le noeud while
    evaluateAST(whileNode);

    freeAST(whileNode); // Liberer la memoire après evaluation

    return *currentToken;
}








ASTNode *parseExpression(Token **currentToken) {
    ASTNode *left = createVariableOrNumberNode(currentToken);

    if (isOperator((*currentToken)->type)) {
        char operator = (*currentToken)->value[0];
        *currentToken = (*currentToken)->nextToken;

        ASTNode *right = parseExpression(currentToken);

        ASTNode *operationNode = malloc(sizeof(ASTNode));
        if (operationNode == NULL) {
            printf("Erreur : allocation memoire echouee pour le noeud OPERATION\n");
            exit(1);
        }

        operationNode->type = NODE_TYPE_OPERATION;
        operationNode->operation.operator = operator;
        operationNode->operation.left = left;
        operationNode->operation.right = right;

        if (left == NULL) {
            printf("Erreur : sous-expression gauche invalide\n");
            exit(1);
        }
        if (isOperator((*currentToken)->type)) {
            if (right == NULL) {
                printf("Erreur : sous-expression droite invalide\n");
                exit(1);
            }
        }

        return operationNode;
    }

    return left;
}



void validateNodeType(NodeType type) {
    // Ajoutez tous les types definis dans l'enum NodeType
    switch (type) {
        case NODE_TYPE_NUMBER:
        case NODE_TYPE_VARIABLE:
        case NODE_TYPE_OPERATION:
        case NODE_TYPE_IF:
        case NODE_TYPE_WHILE:
        case NODE_TYPE_PRINT:
        case NODE_TYPE_ASSIGN:
            return; // Type valide
        default:
            printf("Erreur : type de noeud invalide (%d) detecte\n", type);
        exit(1);
    }
}





ASTNode *parseBlock(Token **currentToken) {
    if ((*currentToken)->type != LBRACE) {
        error("Accolade gauche manquante pour le bloc");
    }

    actualScope++;
    *currentToken = (*currentToken)->nextToken; // Passe l'accolade gauche

    ASTNode *block = NULL; // Liste des instructions dans le bloc
    while ((*currentToken)->type != RBRACE) {
        if (*currentToken == NULL) {
            error("Accolade droite manquante pour le bloc");
        }

        ASTNode *statement = parseStatement(currentToken); // Analyse une instruction
        block = addToBlock(block, statement); // Ajoute l'instruction au bloc

        if ((*currentToken)->type == SEMICOLON) {
            *currentToken = (*currentToken)->nextToken; // Passe le point-virgule
        }
    }
    actualScope--;
    *currentToken = (*currentToken)->nextToken; // Passe l'accolade droite
    return block;
}





ASTNode *parseStatement(Token **currentToken) {
    if (*currentToken == NULL) {
        printf("Erreur : token NULL dans parseStatement\n");
        exit(1);
    }

    // Gestion des identifiants (ex : x = 5)


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
        if (!assignNode) {
            printf("Erreur : allocation mémoire échouée pour le nœud ASSIGN\n");
            exit(1);
        }
        assignNode->type = NODE_TYPE_ASSIGN;
        assignNode->variableName = strdup(varToken->value);
        assignNode->operation.right = valueNode;

        return assignNode;
    }


    // Gestion des instructions `while`
    if ((*currentToken)->type == KEYWORD && strcmp((*currentToken)->value, "while") == 0) {
        return parseWhile(currentToken);
    }

    // Gestion des instructions `print`
    if ((*currentToken)->type == PRINT) {
        *currentToken = (*currentToken)->nextToken; // Passe le mot-cle `print`
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
        validateNodeType(statement->type); // Validation du type
        return statement;
    }

    ASTNode *current = block;
    while (current->next != NULL) { // Parcourt jusqu'au dernier nœud
        current = current->next;
    }
    current->next = statement; // Ajoute l'instruction à la fin du bloc

    validateNodeType(statement->type); // Validation du type
    return block;
}




double evaluateAST(ASTNode *node) {
    if (node == NULL) {
        printf("Erreur critique : noeud AST NULL detecte dans evaluateAST\n");
        exit(1);
    }

    switch (node->type) {

         case NODE_TYPE_ASSIGN: {
            // Évalue l'expression sur le côté droit
            double value = 0.0;
            char *stringValue = NULL;

            if (node->operation.right->type == NODE_TYPE_STRING) {
                stringValue = node->operation.right->variableName;
            } else {
                value = evaluateAST(node->operation.right);
            }

            // Vérifie si la variable existe déjà
             Var * var = malloc(sizeof(Var));
            if (!isVarExists(variables, node->variableName)) {
                // Si la variable n'existe pas, crée une nouvelle
                if (!var) {
                    printf("Erreur : allocation mémoire échouée pour la variable\n");
                    exit(1);
                }
                var->scope = actualScope;
                var->name = strdup(node->variableName);
                var->nextVar = variables;
                variables = var; // Ajoute à la liste des variables

            }else {
                var = getVariable(variables, node->variableName);
            }
            // Met à jour le type et la valeur de la variable
            if (stringValue) {
                var->type = STRING;
                var->value = strdup(stringValue);
            } else {
                if ((double)((int)value) == value) {
                    var->type = INT;
                    var->value = malloc(32); // Alloue la mémoire pour la chaîne
                    if (!var->value) {
                        printf("Erreur : allocation mémoire échouée pour la valeur\n");
                        exit(1);
                    }
                    sprintf(var->value, "%d", (int)value);
                } else {
                    var->type = DOUBLE;
                    var->value = malloc(32); // Alloue la mémoire pour la chaîne
                    if (!var->value) {
                        printf("Erreur : allocation mémoire échouée pour la valeur\n");
                        exit(1);
                    }
                    sprintf(var->value, "%g", value);
                }
            }

            return value; // Retourne la valeur assignée (utile pour les chaînes si nécessaire)
        }


        case NODE_TYPE_STRING:
            return 0; // Si vous n'avez pas besoin d'un retour numérique


        case NODE_TYPE_NUMBER:
            return node->number;

        case NODE_TYPE_VARIABLE: {
            Var *var = getVariable(variables, node->variableName);
            if (!var) {
                printf("Erreur : variable '%s' non définie dans le contexte\n", node->variableName);
                exit(1);
            }
            if (var->type == STRING) {
                return 0; // Les chaînes ne sont pas évaluées comme des nombres
            } else {
                return atof(var->value);
            }
        }

        case NODE_TYPE_OPERATION:
            if (node->operation.left == NULL || node->operation.right == NULL) {
                printf("Erreur : noeud OPERATION a des sous-noeuds NULL\n");
                exit(1);
            }
            double leftValue = evaluateAST(node->operation.left);
            double rightValue = evaluateAST(node->operation.right);
            // Ajoute un switch pour l'operateur
            switch (node->operation.operator) {
                case '+': return leftValue + rightValue;
                case '-': return leftValue - rightValue;
                case '*': return leftValue * rightValue;
                case '/':
                    if (rightValue == 0) {
                        printf("Erreur : division par zero\n");
                        exit(1);
                    }
                    return leftValue / rightValue;
                case '>': return leftValue > rightValue;
                case '<': return leftValue < rightValue;
                case '=': return leftValue == rightValue;
                default:
                    printf("Erreur : operateur inconnu '%c'\n", node->operation.operator);
                    exit(1);
            }

        case NODE_TYPE_PRINT:
            if (node->print.expression->type == NODE_TYPE_STRING) {
                printf("%s\n", node->print.expression->variableName);
            } else if (node->print.expression->type == NODE_TYPE_VARIABLE) {
                Var *var = getVariable(variables, node->print.expression->variableName);
                if (var->type == STRING) {
                    printf("%s\n", var->value);
                } else {
                    printf("%g\n", atof(var->value));
                }
            } else {
                double value = evaluateAST(node->print.expression);
                printf("%g\n", value);
            }
        return 0;


        case NODE_TYPE_WHILE: {
            int iteration = 0;
            actualScope++;
            while (evaluateAST(node->controlFlow.condition)) {
                iteration++;

                // Parcourt et évalue tous les nœuds du corps
                ASTNode *current = node->controlFlow.body;
                while (current) {
                    evaluateAST(current); // Évalue chaque instruction
                    current = current->next; // Passe au nœud suivant dans le bloc
                }
            }
            actualScope--;
            printf("Neoud WHILE : boucle terminee\n");
            return 0;
        }



        default:
            printf("Erreur critique : type de neoud inconnu (%d)\n", node->type);
            exit(1);
    }
}




ASTNode *createVariableOrNumberNode(Token **currentToken) {
    if (*currentToken == NULL) {
        printf("Erreur : Token NULL dans createVariableOrNumberNode\n");
        exit(1);
    }

    ASTNode *node = malloc(sizeof(ASTNode));
    if (node == NULL) {
        printf("Erreur : allocation memoire echouee pour le noeud AST\n");
        exit(1);
    }

    if ((*currentToken)->type == IDENTIFIER) {
        node->type = NODE_TYPE_VARIABLE;
        node->variableName = strdup((*currentToken)->value);
    } else if((*currentToken)->type == TOKENSTRING){
            node->type = NODE_TYPE_STRING;
            node->variableName = strdup((*currentToken)->value);
        }
    else if ((*currentToken)->type == NUMBER) {
        node->type = NODE_TYPE_NUMBER;
        node->number = atof((*currentToken)->value);
    } else if ((*currentToken)->type == LPAREN) {
        *currentToken = (*currentToken)->nextToken; // Passe la parenthèse gauche
        node = parseExpression(currentToken);       // Analyse l'expression entre parenthèses
        if ((*currentToken)->type != RPAREN) {
            printf("Erreur : parenthèse droite manquante\n");
            exit(1);
        }
    } else {
        printf("Erreur : type de token inattendu dans createVariableOrNumberNode\n");
        printf("Type de token : %s, Valeur : %s\n", getType((*currentToken)->type), (*currentToken)->value);
        exit(1);
    }

    *currentToken = (*currentToken)->nextToken; // Passe au prochain token
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
        if(input->type != IDENTIFIER && input->type != NUMBER && !isOperator(input->type) && input->type!=TOKENSTRING) error("Mauvais type de donnee dans le calcul");
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
        if(input->type != IDENTIFIER && input->type != NUMBER && !isOperator(input->type) && input->type!=TOKENSTRING) error("Mauvais type de donnee dans le calcul");
        input = nextToken(input);
        modulo2++;
    }
    return input;
}