//
// Created by mathi on 04/10/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/token.h"
#include "../headers/parser.h"
#include "../headers/lexer.h"

#define BUFFER_SIZE 1024
#define SHUTTINGYARDSTRING_SIZE 1024

// Comprend le code pour l'éxecuter ensuite
void parser(char *input) {
    // crée la suite de token en fonction de l'input
    Token *inputToken = NULL;
    inputToken = lexer(input, inputToken);
    // Si il y a bien des instructions
    if (inputToken != NULL) {
        // on copie le resultat de la fonction qui permet de l'avoir dans notre string
        double result = calcul(inputToken);
        printf("Result : %lf\n", result);

        // On free tout
        freeToken(inputToken);
    }
}