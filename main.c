#include <stdio.h>
#include <string.h>

#define LINE_SIZE 512

int get_line(char *);

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

    printf("¥ ");
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
