#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **copy_tokens(char **tokens, const int num_tokens) {
  char **copy = (char **)malloc(sizeof(char *) * num_tokens);
  for (int i = 0; i < num_tokens; i++) {
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

void free_tokens(char **tokens, const int num_tokens) {
  for (int i = 0; i < num_tokens; i++) {
    free(tokens[i]);
    tokens[i] = NULL;
  }
  free(tokens);
  tokens = NULL;
}

// logic works, should add token limit condition
char **input_tok(char *line, int *num_tok) {

  char *input = malloc(strlen(line) * sizeof(char));
  strcpy(input, line);

  int buffer = 16;
  char **tokens = (char **)malloc(buffer * sizeof(char *));
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
