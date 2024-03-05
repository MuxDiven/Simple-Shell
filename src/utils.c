#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Tokens copy_tokens(Tokens tokens) {

  int buffer = 4;
  Tokens copy = (Tokens)malloc((buffer * sizeof(char *)) + 1);

  if (!copy) {
    printf("memory allocation failed\n");
    return NULL;
  }

  int i = 0;
  for (; tokens[i] != NULL; i++) {
    if (i >= buffer) {
      buffer <<= 1;
      copy = (Tokens)realloc(copy, (buffer * sizeof(char *)) + 1);
      if (!copy) {
        free(copy);
        printf("memory allocation failed\n");
        return NULL;
      }
    }
    copy[i] = strdup(tokens[i]);
    // copy[i] = (char *)malloc((strlen(tokens[i]) + 1) * sizeof(char));
    // if (!copy[i]) {
    //   free(copy);
    //   printf("memory allocation failed\n");
    //   return NULL;
    // }
    // strcpy(copy[i], tokens[i]);
  }
  copy[i] = NULL;
  return copy;
}

void free_tokens(Tokens tokens) {
  for (int i = 0; tokens[i] != NULL; i++) {
    free(tokens[i]);
    tokens[i] = NULL;
  }
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
    tokens[(*num_tok)++] = strdup(token);
    // tokens[*num_tok] = token;
    // (*num_tok)++;
    token = strtok(NULL, delim);
  }
  return tokens;
}

char *tokens_to_string(Tokens tokens) {
  char *line = (char *)malloc(512 * sizeof(char));
  for (int i = 0; tokens[i] != NULL; i++) {
    strcat(i == 0 ? line : strcat(line, " "), tokens[i]);
  }
  return line;
}
