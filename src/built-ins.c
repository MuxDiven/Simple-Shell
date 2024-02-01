#include "../include/built-ins.h"
#include <stdio.h>
#include <stdlib.h>

int cd(char **args) {
  printf("CD COMMAND USED\n");
  return 0;
}
int getpath(char **args) {
  printf("%s\n", getenv("PATH"));
  return 0;
}
int setpath(char **args) {
  printf("SETPATH COMMAND USED\n");
  return 0;
}
