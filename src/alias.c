#include "../include/alias.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// aliases a = {{"cd", NULL}, {"getpath", NULL}, {"setpath", NULL}, {NULL,
// NULL}};
//
alias *new_alias(char *key, Tokens command, int num_tok) {
  alias *a = (alias *)malloc(sizeof(alias));
  a->key = malloc(sizeof(char) * (strlen(key) + 1));
  strcpy(a->key, key);
  a->command = copy_tokens(command, num_tok);
  a->next = NULL;
  return a;
}

void del_alias(alias *tail) {
  free(tail);
  tail = NULL;
}

void allocate_aliases() {
  Aliases aliases = malloc(sizeof(Aliases));
  *aliases = NULL;
}

int add_alias(Aliases aliases, char *key, Tokens args, int num_tok) {
  alias *temp, *head;
  temp = new_alias(key, args, num_tok);

  if (!(*aliases)) {
    *aliases = temp;
    return 0;
  } else {
    head = *aliases;
    while (head->next) {
      if (strcmp(head->key, key) == 0) {
        head->command = args;
        return 0;
      }
      head = head->next;
    }
    head->next = temp;
    return 0;
  }
  return 1;
}

Tokens get_command(Aliases aliases, char *key) {
  alias *head = *aliases;
  for (head; head != NULL; head = head->next) {
    if (strcmp(head->key, key) == 0) {
      return head->command;
    }
  }
  return NULL;
}
