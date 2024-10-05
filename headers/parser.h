//
// Created by mathi on 04/10/2024.
//

#ifndef PARSER_H
#define PARSER_H

void parser(char *input);

void pushBuffer(char ***buffer, int *bufferSize, char *value);

void popBuffer(char ***buffer, int *bufferSize);

void freeBuffer(char **buffer, int bufferSize);

void printBuffer(char **buffer, int bufferSize);
#endif //PARSER_H
