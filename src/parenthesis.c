
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX 100000

// Structure de la pile
typedef struct {
  char items[MAX];
  int top;
} Stack;

//Initialisation de la pile
void initStack(Stack *stack) {
  stack->top = -1;
}

//Fonction pour vérifier si la pile est vide
bool isEmpty(Stack *stack) {
  return stack->top == -1;
}

//Fonction pour ajouter un élément dans la pile
void push(Stack *stack, char value) {
  if (stack->top < MAX - 1) {
    stack->items[++stack->top] = value;
  } else {
    printf("Erreur : pile pleine.\n");
  }
}

//Fonction pour retirer un élément de la pile
char pop(Stack *stack) {
  if (!isEmpty(stack)) {
    return stack->items[stack->top--];
  } else {
    printf("Erreur : pile vide.\n");
    return '\0';
  }
}

//Fonction pour vérifier si les parenthèses sont appariées
bool areMatching(char open, char close) {
  return (open == '(' && close == ')') || (open == '{' && close == '}') || (open == '[' && close == ']');
}

//Fonction pour vérifier si les parenthèses sont bien imbriquées
bool areParenthesesBalanced(const char *expression) {
  Stack stack;
  initStack(&stack);

  for (int i = 0; i < strlen(expression); i++) {
    char ch = expression[i];
    //Si c'est une parenthèse ouvrante, on la pousse dans la pile
    if (ch == '(' || ch == '{' || ch == '[') {
      push(&stack, ch);
    } //Si c'est une parenthèse fermante, on vérifie le sommet de la pile
    else if (ch == ')' || ch == '}' || ch == ']') {
      if (isEmpty(&stack) || !areMatching(pop(&stack), ch)) {
        return false;
        //Déséquilibre trouvé
      }
    }
  }

  //Si la pile est vide à la fin, les parenthèses sont bien imbriquées
  return isEmpty(&stack);
}

int main() {
  const char *expression = "((()))[()]()";

  if (areParenthesesBalanced(expression)) {
    printf("Les parenthèses sont bien imbriquées.\n");
  } else {
    printf("Les parenthèses ne sont pas bien imbriquées.\n");
  }

  return 0;
}
