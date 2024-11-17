// variablesList.c
#include "../headers/variable.h"
#include <stdio.h>

Var *variables = NULL; // Définition de la variable globale

int astEnabled = 0; // 0 pour désactivé, 1 pour activé

int actualScope = 0;
