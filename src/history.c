#include "../include/history.h"
#include <stdlib.h>

history *allocate_history() {
  history *h = (history *)malloc(sizeof(history));
  h->previous_commands = (char ***)malloc(sizeof(char **) * HISTORY_SIZE);
  h->index = 0;
  h->size = HISTORY_SIZE;

  return h;
}
