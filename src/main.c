#include "../include/main.h"
#include "../include/built-ins.h"
#include "../include/history.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define LINE_SIZE 512

int main(int argc, char *argv[]) {

  char *originalPATH = getenv("PATH"); // save the current path
  chdir(getenv("HOME"));               // set home directory

  // load history
  // load alias

  char line[LINE_SIZE];

  char *history_filepath =
      malloc(strlen(getenv("HOME")) + strlen("/.hist_list") + 1);
  history_filepath = strcpy(history_filepath, getenv("HOME"));
  history_filepath = strcat(history_filepath, "/hist_list");
  History history = load_history(history_filepath);

  printf("shimple shell- inishialished\n");
  for (;;) {

    // print prompt
    // get user input
    // parse & tokenize
    // while history condition thing
    // while alias condition thing
    // if it is built-in invoke the command
    // else execute command as an external process

    printf("₪ ");
    if (get_line(line) == EOF) {
      printf("exit\n");
      break;
    }
    int numtok = 0;
    Tokens command = input_tok(line, &numtok);

    if (numtok == 0) {
      continue;
    }

    // history commands
    if (command[0][0] == '!') {
      printf("History command invoked\n");
      if (strlen(command[0]) > 1) {
        if (command[0][1] == '-') {
          printf("User entered !-?\n");
          // user entered !-
        } else if (command[0][1] == '!') {
          printf("User entered !!\n");
          // user entered !!
        }
        Tokens command = get_history(history, command[0][1]);
      }
    }

    add_history(history, command);

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
      int failure = setpath(command, numtok);
      continue;
    } else if (strcmp(command[0], "history") == 0) {
      show_history(history);
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

    free_tokens(command);
  }

  save_history(history, history_filepath);

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
