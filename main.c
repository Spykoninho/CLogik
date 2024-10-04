#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

int main(void) {
    Token * token = NULL;
    token = lexer("if(a>b){print(a);}", token);
    if (token != NULL) {
        printToken(token);
        free(token);
    }
    return 0;
}

