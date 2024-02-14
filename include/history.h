#include "../include/utils.h"

#define HISTORY_SIZE 20

struct h {
  Tokens *previous_commands;
  int index;
  int size;
};

typedef struct h *History;

History allocate_history();

void add_history(History history, Tokens tokens, int num_tok);

Tokens get_history(History history, int index);

void show_history(History history);

void save_history(History history);

History load_history(History history);
