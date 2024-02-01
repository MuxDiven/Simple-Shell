#include "../include/main.h"
#include "../include/built-ins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LINE_SIZE 512

int main(int argc, char *argv[]) {

  char *originalPATH = getenv("PATH"); // save the current path
  chdir(getenv("HOME"));               // set home directory

  // load history
  // load alias

  char line[LINE_SIZE];

  printf("shimple shell- inishialished\n");
  for (;;) {

    // print prompt
    // get user input
    // parse & tokenize
    // while history condition thing
    // if it is built-in invoke the command
    // else execute command as an external process

    printf("₪ "); 
    if (get_line(line) == EOF) {
      printf("exit\n");
      break;
    }
    int numtok = 0;
    char **command = input_tok(line, &numtok);

    if (numtok == 0) {
      continue;
    }

    // built in commands
    if (strcmp(command[0], "exit") == 0) {
      break;
    } else if (strcmp(command[0], "cd") == 0) {
      int failure = cd(command, numtok); // runs cd command from built-ins.h
      continue;
    } else if (strcmp(command[0], "getpath") == 0) {
      int failure = getpath(command);
      continue;
    } else if (strcmp(command[0], "setpath") == 0) {
      int failure = setpath(command);
      continue;
    }

    pid_t pid = fork();

    if (pid < 0) {
      printf("Error spawning child process\n");
    } else if (pid == 0) {
      execvp(command[0], command);
      perror(command[0]);
      exit(1);
    } else {
      wait(NULL);
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
