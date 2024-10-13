#include <stdio.h>
#include <stdlib.h>
#include "../headers/token.h"
#include "../headers/parser.h"
#include "../headers/variable.h"

int main(void) {
    char input[255]; // Code de 255 caractères maximum
    //fgets(input, 255, stdin); // Entrée utilisateur
    Var * variable = NULL;
    parser("7 + 2 * (3 + 1)"); // On effectue le code

    /*
    variable = addVariable("a=23", variable);
    variable = addVariable("b=\"Bonjour Monsieur.\"", variable);
    variable = addVariable("c=12.3", variable);
    variable = addVariable("d=a", variable);
    */
    //printVariables(variable);
   // freeVariable(variable);
    return 0;
}
