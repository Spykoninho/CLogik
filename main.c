#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

int main(void) {
    Token * token = NULL;
    token = lexer("x = 10 + y", token);
    if (token != NULL) {
        printToken(token);
        free(token);
    }
    printToken(token);
    return 0;
}

