#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Tokens copy_tokens(Tokens tokens) {

  int buffer = 4;
  Tokens copy = (Tokens)malloc(buffer * sizeof(char *));

  if (!copy) {
    printf("memory allocation failed\n");
    return NULL;
  }

  for (int i = 0; tokens[i] != NULL; i++) {
    if (i >= buffer) {
      buffer <<= 1;
      copy = realloc(copy, buffer * sizeof(char *));
      if (!copy) {
        free(copy);
        printf("memory allocation failed\n");
        return NULL;
      }
    }
    copy[i] = malloc(sizeof(tokens[i]));
    if (!copy[i]) {
      free(copy);
      printf("memory allocation failed\n");
      return NULL;
    }
    strcpy(copy[i], tokens[i]);
  }
  return copy;
}

void free_tokens(Tokens tokens) {
  free(tokens);
  tokens = NULL;
}

// logic works, should add token limit condition
Tokens input_tok(char *input, int *num_tok) {

  int buffer = 16;
  Tokens tokens = (char **)malloc(buffer * sizeof(char *));
  const char delim[] = " \t\n|><&;";

  if (!tokens) {
    printf("memory allocation failed\n");
    return NULL;
  }

  char *token = strtok(input, delim);
  *num_tok = 0;

  while (token != NULL) {
    if (*num_tok >= buffer) {
      buffer <<= 1;
      tokens = realloc(tokens, buffer * sizeof(char *));
      if (!tokens) {
        free(tokens);
        printf("memory allocation failed\n");
        return NULL;
      }
    }
    tokens[*num_tok] = token;
    (*num_tok)++;
    token = strtok(NULL, delim);
  }
  return tokens;
}

void replace_with_alias(char *token_to_replace, Tokens replace_with) {

  Tokens copy_of_tokens = copy_tokens(replace_with);

  free(token_to_replace);

  token_to_replace = replace_with[0];
}
