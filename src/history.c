#include "../include/history.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>

History allocate_history() {
  History h = (History)malloc(sizeof(History));
  h->previous_commands = (Tokens *)malloc(sizeof(Tokens) * HISTORY_SIZE);
  h->index = 0;
  h->size = HISTORY_SIZE;

  return h;
}

void add_history(History history, Tokens tokens,
                 int num_tok) { // adds function used to history struct
  if (history->index % history->size == 0) {
    history->index -= history->size;
  }
  if (history->previous_commands[history->index] == NULL) {
    history->previous_commands[history->index] = copy_tokens(tokens, num_tok);
    history->index++;
  } else {
    free_tokens(tokens, num_tok); // i dont think this works but ill be back
    history->previous_commands[history->index] = copy_tokens(tokens, num_tok);
    history->index++;
  }
}

Tokens get_history(History history, int index);

void show_history(History history) {
  printf("History command invoked\n");
  for (int i = 0; i < history->index; i++) {
    printf("%d %s\n", i, history->previous_commands[i][0]);
  }
}

void save_history(
    History history); // save history from instance of struct into file

History load_history(
    History history); // load history from file into instance of history struct
