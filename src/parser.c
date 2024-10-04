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

void parser(char * input) {
    Token * token = NULL;
    token = lexer(input, token);
    if (token != NULL) {
        char buffer[BUFFER_SIZE];
        char * shuttingYardString = malloc(sizeof(char)*1024);
        tokensToShuttingYardString(token, buffer, BUFFER_SIZE, shuttingYardString);
        strcpy(shuttingYardString, tokensToShuttingYardString(token, buffer, BUFFER_SIZE, shuttingYardString));
        printf("%s\n", shuttingYardString);

        free(shuttingYardString);
        free(token);
    }
}