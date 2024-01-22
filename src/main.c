#include "../include/main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE 512

int main(int argc, char *argv[]) {

  // get home directory
  // set home directory
  // save the current path
  // load history
  // load alias
  //

  char line[LINE_SIZE];

  for (;;) {

    // print prompt
    // get user input
    // parse & tokenize
    // while history condition thing
    // if it is built-in invoke the command
    // else execute command as an external process

    printf("£ "); // ¥
    if (get_line(line) == EOF) {
      printf("exit\n");
      break;
    }

    // compare if command is exit
    if (strcmp(line, "exit") == 0) {
      break;
    }
  }

  // save history
  // save alias
  // restore original path
  // exit

  return 0;
}

// should probably abstract this to a different file with the tokenizer function
int get_line(char *line) {
  if (fgets(line, LINE_SIZE, stdin) == NULL)
    return EOF;
  // remove new line terminator
  line[strcspn(line, "\n")] = '\0';
  return 0;
}

// logic works, should add token limit condition
char **input_tok(char *input, int *num_tok) {
  int buffer = 16;
  char **tokens = (char **)malloc(buffer * sizeof(char *));

  if (!tokens) {
    printf("memory allocation failed\n");
    return NULL;
  }

  char *token = strtok(input, " ");
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
    token = strtok(NULL, " ");
  }
  return tokens;
}
