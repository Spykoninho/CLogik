#include <stdio.h>
#include <stdlib.h>
#include "../headers/token.h"
#include "../headers/parser.h"

int main(void) {
    char input[255];
    fgets(input, 255, stdin);
    parser(input);

    return 0;
}

