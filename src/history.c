#include "../include/history.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

History allocate_history() {
  History h = (History)malloc(sizeof(struct h));
  h->previous_commands = (Tokens *)malloc(sizeof(Tokens) * HISTORY_SIZE);
  h->index = 0;
  h->counter = 0;
  h->size = HISTORY_SIZE;

  return h;
}

void add_history(History history,
                 Tokens tokens) { // adds function used to history struct
  if (history->previous_commands[history->index] == NULL) {
    history->previous_commands[history->index] = copy_tokens(tokens);
  } else {
    free_tokens(history->previous_commands[history->index]);
    history->previous_commands[history->index] = copy_tokens(tokens);
  }
  history->index++;
  history->counter++;
  if (history->index == history->size) {
    history->index = 0;
  }
}

Tokens get_history(History history, int index);

void show_history(History history) {
  int j = history->index;
  if (history->previous_commands[history->index + 1] != NULL) {
    for (int i = 0; i < 20; i++) {
      printf("%d %s\n", i + 1, tokens_to_string(history->previous_commands[j]));
      j++;
      if (j == 20) {
        j = 0;
      }
    }
  } else {
    for (int i = 0; i < history->index; i++) {
      printf("%d %s\n", i + 1, tokens_to_string(history->previous_commands[i]));
    }
  }
}

void save_history(
    History history); // save history from instance of struct into file

History load_history(History history); // load history from file into instance
                                       // of history struct
