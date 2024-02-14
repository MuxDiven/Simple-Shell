#include "../include/built-ins.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int cd(Tokens args, int num_tokens) {
  if (num_tokens == 1) { // checks if cd has any arguments
    if (chdir(getenv("HOME")) ==
        -1) { // changes working directory to home directory
      perror(getenv("HOME"));
      return -1;
    }
    return 0;
  }
  if (chdir(args[1]) == -1) { // changes working directory to directory given
    perror(args[1]);
    return -1;
  }
  return 0;
}
int getpath(Tokens args) {
  printf("%s\n", getenv("PATH"));
  return 0;
}
int setpath(Tokens args, int num_tokens) {
  if (num_tokens == 1) {
    printf("Error: No path provided\n");
    return 1;
  }
  setenv("PATH", args[1], 1);
  return 0;
}
