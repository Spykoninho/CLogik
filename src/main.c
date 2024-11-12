#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/interpret.h"
#include "../headers/variable.h"

extern Var *variable;

void interactive_mode() {
    char input[100];
    printf("Mode interactif : Entrez votre code (tapez 'q' pour quitter) :\n");
    do {
        printf("> ");
        fgets(input, 100, stdin);
        interpret(input);
    } while (strcmp(input, "q\n") != 0);
}

void file_mode(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        interpret(line);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage : %s <fichier> ou --interactive\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "--interactive") == 0) {
        interactive_mode();
    } else {
        file_mode(argv[1]);
    }

    return 0;
}
