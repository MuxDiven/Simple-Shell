#include "../include/history.h"
#include <stdlib.h>

history *allocate_history() {
  history *h = (history *)malloc(sizeof(history));
  return h;
}
