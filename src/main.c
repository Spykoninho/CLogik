#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/interpret.h"
#include "../headers/variable.h"

int main(void) {
    char input[100];
    Var *variables = NULL;
    printf("Entrez votre code : \n");
    do {
        fgets(input, 100, stdin);
        interpret(input, &variables);
    }while (strcmp(input, "q") != 0);

    freeVariable(variables);
    return 0;
}
