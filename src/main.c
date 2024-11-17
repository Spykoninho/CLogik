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
        if (strcmp(input, "\n") == 0) continue; // autorise les sauts de ligne
        if (userWantsToQuit(input)) break;
        interpret(input);
    } while (!userWantsToQuit(input));

    freeVariable(variables);
    return 0;
}

int userWantsToQuit(char *input) {
    char* ptr = strchr(input, '\n');
    if (ptr) {
        // if new line found replace with null character
        *ptr = '\0';
    }
    return strcmp(input, "q") == 0 || strcmp(input, "quit") == 0 || strcmp(input, "panic();") == 0;
}
