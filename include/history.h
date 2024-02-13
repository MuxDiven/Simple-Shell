#include "../include/utils.h"

#define HISTORY_SIZE 20

typedef struct h {
  Tokens *previous_commands;
  int index;
  int size;
} history;

history *allocate_history();

history load_history();

void add_history(Tokens tokens);

void save_history();

Tokens get_history(int index);
