#include <stdio.h>
#include <stdlib.h>
#include "../headers/token.h"
#include "../headers/parser.h"
#include "../headers/variable.h"

int main(void) {
    char input[255]; // Code de 255 caractères maximum
    //fgets(input, 255, stdin); // Entrée utilisateur
    Var * variable = NULL;
    //parser(input); // On effectue le code

    variable = addVariable("a=23.10", variable);
    //variable = addVariable("b=\"Bonjour\"", variable);
    //variable = addVariable("c=12.3", variable);
    //variable = addVariable("d=255.", variable);

    printVariables(variable);
    freeVariable(variable);
    return 0;
}
