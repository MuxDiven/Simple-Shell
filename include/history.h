#include "../include/utils.h"

#define HISTORY_SIZE 20

struct h {
  Tokens *previous_commands;
  int index;
  int size;
  int count;
  int first;
};

typedef struct h *History;

History allocate_history();

Tokens parse_history_input(History history, char *input);

int is_history_invocation(char *input);

void add_history(History history, Tokens tokens);

Tokens get_history(History history, int index);

Tokens get_previous_history(History history);

Tokens get_minus_history(History history, int index);

void show_history(History history);

void free_history(History *history);

int save_history(History history, char *filepath);

History load_history(char *filepath);
