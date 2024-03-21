#include "../include/main.h"
#include "../include/alias.h"
#include "../include/built-ins.h"
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

  chdir(getenv("HOME")); // set home directory

  // // load history
  // // load alias

  char line[LINE_SIZE];

  // Aliases aliases = allocate_aliases();
  char *alias_filepath =
      malloc(strlen(getenv("HOME")) + strlen("/.aliases") + 1);
  alias_filepath = strcpy(alias_filepath, getenv("HOME"));
  alias_filepath = strcat(alias_filepath, "/.aliases");
  Aliases aliases = read_aliases(alias_filepath);

  char *history_filepath =
      malloc(strlen(getenv("HOME")) + strlen("/.hist_list") + 1);
  history_filepath = strcpy(history_filepath, getenv("HOME"));
  history_filepath = strcat(history_filepath, "/.hist_list");
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

    int add_to_history = 1;

    for (int i = 0; command[i] != NULL; i++) {
      if (is_history_invocation(command[i])) {
        add_to_history = 0;
      }
    }

    Tokens original_command = copy_tokens(command);

    if (alias_transform(aliases, history, &command, &numtok) == 1) {
      add_history(history, original_command);
      continue;
    }

    // aliases
    if (strcmp(command[0], "alias") == 0) {
      if (numtok == 1) {
        if (add_to_history) {
          add_history(history, original_command);
        }
        show_aliases(aliases);
        continue;
      } else if (numtok == 2) {
        printf("Error parsing alias command\n");
        continue;
      } else {
        char *key;
        key = strdup(command[1]);

        memmove(command, command + 2, (numtok - 2) * sizeof(char *));
        command[numtok - 2] = NULL;

        if (check_for_alias(aliases, key) == 0) {
          if (!add_alias(aliases, key, command)) {
            if (add_to_history) {
              add_history(history, original_command);
            }
            printf("Adding new alias\n");
          }
        } else {
          if (!add_alias(aliases, key, command)) {
            if (add_to_history) {
              add_history(history, original_command);
            }
            printf("Alias %s overriden\n", key);
          }
        }

        free(key);
        continue;
      }
    } else if (strcmp(command[0], "unalias") == 0) {
      if (numtok != 2) {
        printf("unalias: unexpected amount of parameters\nexpected 1 got %d\n",
               numtok - 1);
        if (add_to_history) {
          add_history(history, original_command);
        }
        continue;
      }
      int failure = rem_at(aliases, command[1]);
      if (failure == 1) {
        printf("Alias removed\n");
      } else {
        printf("Alias not found\n");
      }
      if (add_to_history) {
        add_history(history, original_command);
      }
      continue;
    }

    if (add_to_history) {
      add_history(history, original_command);
    }

    //
    // if (alias_transform(aliases, &command, &numtok) == 1) {
    //   printf("Cyclic alias detected: aborting\n");
    //   continue;
    // }
    //
    // // history commands
    // if (command[0][0] == '!') {
    //
    //   if (strlen(command[0]) > 1) {
    //
    //     if (command[0][1] == '!') { // user entered '!!'
    //       command = get_previous_history(history, &numtok);
    //     } else if (command[0][1] == '-') { // user entered '!-x'
    //       //
    //       int i, result = 0;
    //
    //       for (i = 2; command[0][i] != '\0'; i++) {
    //         result = result * 10 + (command[0][i] - '0');
    //       }
    //
    //       if (result >= history->count) {
    //         printf("history: No history invocation at index -%d\n", result);
    //         continue;
    //       }
    //
    //       command = get_minus_history(history, result, &numtok);
    //
    //     } else {
    //       int i, result = 0;
    //
    //       for (i = 1; command[0][i] != '\0'; i++) {
    //         result = result * 10 + (command[0][i] - '0');
    //       }
    //       if (result > history->count || result <= 0) {
    //         printf("history: No history invocation at index %d\n", result);
    //         continue;
    //       }
    //       command = get_history(history, result, &numtok);
    //     }
    //   }
    // }

    // built in commands
    if (strcmp(command[0], "exit") == 0) {
      break;
    } else if (strcmp(command[0], "cd") == 0) {
      if (cd(command, numtok) == 1) {
        printf("FAILED\n");
      }
      continue;
    } else if (strcmp(command[0], "getpath") == 0) {
      if (getpath(command, numtok) == 1) {
        printf("FAILED\n");
      }
      continue;
    } else if (strcmp(command[0], "setpath") == 0) {
      if (setpath(command, numtok) == 1) {
        printf("FAILED\n");
      }
      continue;
    } else if (strcmp(command[0], "history") == 0) {
      if (numtok != 1) {
        printf("history: Unexpected number of arguments\n");
        continue;
      }
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

  printf("%s\n", originalPATH);
  // save history
  save_history(history, history_filepath);
  free_history(&history);
  // save alias
  save_aliases(aliases, alias_filepath);
  free_aliases(aliases);
  // restore original path
  // exit
  free(history_filepath);
  free(alias_filepath);
  return 0;
}

// should probably abstract this to a different file with the tokenizer
// function
int get_line(char *line) {
  if (fgets(line, LINE_SIZE, stdin) == NULL)
    return EOF;
  // remove new line terminator
  line[strcspn(line, "\n")] = '\0';
  return 0;
}
