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
  h->first = 0;

  return h;
}

Tokens parse_history_input(History history, char *input) {

  if (input[0] == '!') {

    if (strlen(input) > 1) {

      if (input[1] == '!') { // user entered '!!'
        return get_previous_history(history);
      } else if (input[1] == '-') { // user entered '!-x'
        //
        int i, result = 0;

        for (i = 2; input[i] != '\0'; i++) {
          result = result * 10 + (input[i] - '0');
        }

        if (result >= history->count) {
          printf("history: No history invocation at index -%d\n", result);
          return NULL;
        }

        return get_minus_history(history, result);

      } else {
        int i, result = 0;

        for (i = 1; input[i] != '\0'; i++) {
          result = result * 10 + (input[i] - '0');
        }
        if (result > history->count || result <= 0) {
          printf("history: No history invocation at index %d\n", result);
          return NULL;
        }
        return get_history(history, result);
      }
    }
  }
  return NULL;
}

int is_history_invocation(char *input) {
  if (strlen(input) > 0) {
    if (input[0] == '!')
      return 1;
  }
  return 0;
}

void add_history(History history, Tokens tokens) {
  if (history->size != history->count) {
    history->previous_commands[history->index] = copy_tokens(tokens);
  } else {
    free_tokens(history->previous_commands[history->index]);
    history->previous_commands[history->index] = copy_tokens(tokens);
  }
  history->index++;
  history->count++; // gave history count a use :)
  if (history->count == 21) {
    history->count = 20;
    history->first++;
    if (history->first == 20) {
      history->first = 0;
    }
  }
  if (history->index == history->size) {
    history->index = 0;
  }
}

Tokens get_history(History history, int index) {
  if (history->previous_commands[history->index + 1] == NULL) {
    Tokens tokens = copy_tokens(history->previous_commands[index - 1]);
    return tokens;
  } else {
    int j = 0;
    j = history->first + index - 1;
    if (j > 19) {
      j -= 20;
    }
    if (history->previous_commands[j] != 0) {
      Tokens tokens = copy_tokens(history->previous_commands[j]);
      return tokens;
    }
  }
  return 0;
}

Tokens get_previous_history(History history) {
  if (history->index == 0) {
    Tokens tokens = copy_tokens(history->previous_commands[19]);
    return tokens;
  } else {
    Tokens tokens = copy_tokens(history->previous_commands[history->index - 1]);
    return tokens;
  }
}

Tokens get_minus_history(History history, int index) {
  if (history->previous_commands[history->index + 1] == NULL) {
    int new_index = history->index - index;
    return get_history(history, new_index);
  } else {
    int j = 0;
    j = 20 - index;
    return get_history(history, j);
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
