#include <stdio.h>
#include <stdlib.h>
#include "../headers/token.h"
#include "../headers/parser.h"

int main(void) {
    char input[255]; // Code de 255 caractères maximum
    fgets(input, 255, stdin); // Entrée utilisateur
    parser(input); // On effectue le code

    return 0;
}
