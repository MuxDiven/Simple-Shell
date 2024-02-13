
/* char **command = input_tok(line, &numtok); */

typedef struct h {
  char **previous_commands[20];
  int index;
  int size;
} history;

history *allocate_history();
