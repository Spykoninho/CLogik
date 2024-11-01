#include <stdio.h>
#include <stdlib.h>
#include "../headers/token.h"
#include "../headers/parser.h"
#include "../headers/variable.h"

extern Var *variable;

int main(void) {
    char input[255]; // Code de 255 caractères maximum
    //fgets(input, 255, stdin); // Entrée utilisateur
    parser("3.5+2"); // On effectue le code

    printf("1: %p\n", variable);
    printVariables(variable);

    variable = addVariable("z=3.5+2", variable);
    variable = addVariable("a=23", variable);
    variable = addVariable("b=\"Bonjour Monsieur.\"", variable);
    variable = addVariable("c=12.3", variable);
    variable = addVariable("d=a*2+a", variable);

    parser("print(a)");
    printf("3: %p\n", variable);
    printVariables(variable);
    freeVariable(variable);
    return 0;
}
