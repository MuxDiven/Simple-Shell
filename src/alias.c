#include "../include/alias.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// aliases a = {{"cd", NULL}, {"getpath", NULL}, {"setpath", NULL}, {NULL,
// NULL}};
//
//
alias *new_alias(char *key, Tokens command) {
  alias *a = (alias *)malloc(sizeof(alias));
  a->key = malloc(sizeof(char) * (strlen(key) + 1));
  strcpy(a->key, key);
  a->command = copy_tokens(command);
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

int add_alias(Aliases aliases, char *key, Tokens args) {
  alias *temp, *head;
  temp = new_alias(key, args);

  if (!(*aliases)) {
    *aliases = temp;
    return 0;
  } else {
    head = *aliases;
    while (head->next) {
      if (strcmp(head->key, key) == 0) {
        head->command = copy_tokens(args);
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

alias *rem_alias(Aliases aliases, char *key) {
  if (*aliases == NULL) {
    alias *afterHead = (*aliases)->next;
    del_alias(*aliases);
    *aliases = afterHead;
  }

  alias *afterHead = (*aliases)->next;
  alias *a = *aliases;
  del_alias(*aliases);
  *aliases = afterHead;
  return a;
}

int rem_at(Aliases aliases, char *key) {
  if (*aliases) {
    alias *head = *aliases;
    while (head->next) {
      if (strcmp(head->next->key, key) == 0) {
        alias *afterHead = head->next->next;
        del_alias(head->next);
        head->next = afterHead;
        return 1;
      }
      head = head->next;
    }
    if (strcmp(head->key, key) == 0) {
      del_alias(head);
      return 1;
    }
  }
  return 0;
}
