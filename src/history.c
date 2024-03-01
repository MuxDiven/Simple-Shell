#include "../include/history.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

History allocate_history() {
  History h = (History)malloc(sizeof(struct h));
  h->previous_commands = (Tokens *)malloc(sizeof(Tokens) * HISTORY_SIZE);
  h->index = 0;
  h->size = HISTORY_SIZE;
  h->count = 0;

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
  history->count++;
  if (history->index == history->size) {
    history->index = 0;
  }
}

Tokens get_history(History history, int index) {
  if (history->previous_commands[history->index + 1] == NULL) {
    return history->previous_commands[index - 1];
    printf("Returning from %d the value -> %s and so on\n", index - 1,
           history->previous_commands[index - 1][0]);
  } else {
    for (int i = 0; i < history->index; i++) {
      return history->previous_commands[history->index];
    }
  }
  return 0;
}

void show_history(History history) {
  int j = history->index;
  if (history->previous_commands[history->index + 1] != NULL) {
    for (int i = 0; i < 20; i++) {
      printf("%d ", i + 1);
      for (int k = 0; history->previous_commands[j][k] != NULL; k++) {
        printf("%s", history->previous_commands[j][k]);
      }
      printf("\n");
      // char *line = tokens_to_string(history->previous_commands[j]);
      // printf("%d %s\n", i + 1, line);
      // free(line);
      j++;
      if (j == 20) {
        j = 0;
      }
    }
  } else {
    for (int i = 0; i < history->index; i++) {
      printf("%d ", i + 1);
      for (int k = 0; history->previous_commands[i][k] != NULL; k++) {
        printf("%s ", history->previous_commands[i][k]);
      }
      printf("\n");
      // char *line = tokens_to_string(history->previous_commands[i]);
      // printf("%d %s\n", i + 1, line);
      // free(line);
    }
  }
}

void free_history(History *history) {

  for (int i = 0; (*history)->previous_commands[i] != NULL; i++) {
    free_tokens((*history)->previous_commands[i]);
    (*history)->previous_commands[i] = NULL;
  }

  free(*history);
  *history = NULL;
}

// of history struct
int save_history(History h, char *filepath) {
  FILE *fptr = fopen(filepath, "w");
  if (!fptr) {
    return 0;
  }

  for (int i = h->index, j = 0; j < 20; i = (i + 1) % h->size, j++) {
    if (h->previous_commands[i] != NULL) {
      fprintf(fptr, "%s\n", tokens_to_string(h->previous_commands[i]));
    }
  }
  fclose(fptr);
  return 1;

} // save history from instance of struct into file
//
// load history from file into instance of history struct
History load_history(char *filepath) {

  History h = allocate_history();

  FILE *fptr = fopen(filepath, "r");

  if (!fptr) {
    return h;
  }

  int c = 1;
  int b_size = 16;

  while (c > 0) {

    char *buffer = malloc(sizeof(char) * b_size);
    int i = 0;

    while ((c = fgetc(fptr)) != '\n' && c > 0) {
      if (i == b_size) {
        b_size += b_size;
        buffer = realloc(buffer, b_size);
      }
      buffer[i++] = c;
    }
    if (c > 0) {
      buffer[i++] = 0;
      int num_tokens = 0;
      Tokens tokens = input_tok(buffer, &num_tokens);
      add_history(h, tokens);
      free_tokens(tokens);
    }
    free(buffer);
  }

  fclose(fptr);
  return h;
}
