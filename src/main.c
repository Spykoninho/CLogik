#include <stdio.h>
#include <stdlib.h>

#include "../headers/lexer.h"
#include "../headers/token.h"
#include "../headers/parser.h"
#include "../headers/variable.h"

int main(void) {
    /*
    char input[255]; // Code de 255 caractères maximum
    fgets(input, 255, stdin); // Entrée utilisateur
    Token *token = NULL;
    token = lexer(input, token);
    printTokens(token);
    */
    //parser("3.5+2"); // On effectue le code

    Var *variable = NULL;
    variable = addVariable("z=3.5+2", variable);
    variable = addVariable("a=23-z", variable);
    variable = addVariable("c=a/12.3", variable);
    variable = addVariable("d=2.2+c", variable);
    variable = addVariable("b=\"Monsieur.\"", variable);
    variable = addVariable("A=\"Bonjour \" + b", variable);
    variable = addVariable("A = A + \"Dupont\"", variable);

    printVariables(variable);
    freeVariable(variable);

    return 0;
}
