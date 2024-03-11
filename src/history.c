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
  } else {
    int j = history->index + index -
            1; // there is an error when this is over 20 but it
    for (int i = 0; i < 20;
         i++) { // messes up the rest of this code so ill do it later
      j++;
      if (j == 20) {
        j = 0;
      }
    }
    return history->previous_commands[j];
  }
  return 0;
}

Tokens get_previous_history(History history) {
  return history->previous_commands[history->index - 1];
}

Tokens get_minus_history(History history,
                         int index) { // need to make it work past 20 hist_index
  if (history->previous_commands[history->index + 1] == NULL) {
    int new_index = history->index - index;
    return get_history(history, new_index);
  } else {
    printf("Are we getting here");
    fflush(stdout);
    int j = 0;
    if (history->index + index - 1 > 20) {
      // something but we are fucked here
      // error is when history index + index > 20 after the first 20 are
      // completed
    } else {
      int j = history->index + index - 1;
    }
    for (int i = 0; i < 20; i++) {
      j++;
      if (j == 20) {
        j = 0;
      }
    }
    int new_index = j - index;
    printf("This is the new index -> %d\n This is the j -> %d\n This is the "
           "index -> %d\n This is the history->index -> %d",
           new_index, j, index, history->index);
    fflush(stdout);
    return get_history(history, new_index);
  }
  return 0;
}

void show_history(History history) {
  int j = history->index;
  if (history->previous_commands[history->index + 1] != NULL) {
    for (int i = 0; i < 20; i++) {
      printf("%d ", i + 1);
      for (int k = 0; history->previous_commands[j][k] != NULL; k++) {
        printf("%s ", history->previous_commands[j][k]);
      }
      printf("\n");
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

      for (int k = 0; h->previous_commands[i][k] != NULL; k++) {
        fprintf(fptr, "%s ", h->previous_commands[i][k]);
      }
      fprintf(fptr, "\n");
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
