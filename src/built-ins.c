#include "../include/built-ins.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int cd(char **args, int ntokens) {
  if(ntokens == 1){ //checks if cd has any arguments
    if(chdir(getenv("HOME")) == -1){ //changes working directory to home directory
    perror(getenv("HOME")); 
    return -1;
    }
	return 0;
  }
  if(chdir(args[1]) == -1){ //changes working directory to directory given
  perror(args[1]);
  return -1;
  }
  return 0;
}
int getpath(char **args) {
  printf("%s\n", getenv("PATH"));
  return 0;
}
int setpath(char **args) {
  setenv("PATH",args[1],1);
  return 0;
}
