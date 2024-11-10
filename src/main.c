#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/interpret.h"
#include "../headers/variable.h"

extern Var *variable;

int main(void) {
    char input[100];
    printf("Entrez votre code : \n");
    do {
        fgets(input, 100, stdin);
        interpret(input);
    } while (strcmp(input, "q") != 0);

    freeVariable(variables);
    return 0;
}
