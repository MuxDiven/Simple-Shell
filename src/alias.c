#include "../include/alias.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALIAS_LIM 10

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

Aliases allocate_aliases() {
  Aliases aliases = malloc(sizeof(Aliases));
  *aliases = NULL;
  return aliases;
}

int add_alias(Aliases aliases, char *key, Tokens args) {
  alias *temp, *head;
  temp = new_alias(key, args);

  if (!(*aliases)) {
    *aliases = temp;
    return 0;
  } else {
    head = *aliases;
    int size = 1;
    while (head->next) {
      if (strcmp(head->key, key) == 0) { // if alias exists override
        free_tokens(head->command);
        head->command = copy_tokens(args);
        return 0;
      }
      head = head->next;
      size++;
    }
    // check override condition on last item in the list
    if (strcmp(head->key, key) == 0) {
      free_tokens(head->command);
      head->command = copy_tokens(args);
      return 0;
    }
    if (size >= ALIAS_LIM) {
      printf("alias: list full, remove an alias\n");
      return 1;
    }
    // push to end of list
    head->next = temp;
    return 0;
  }
  return 1;
}

Tokens get_alias_command(Aliases aliases, char *key) {
  for (alias *head = *aliases; head != NULL; head = head->next) {
    if (strcmp(head->key, key) == 0) {
      return head->command;
    }
  }
  return NULL;
}

int check_for_alias(Aliases aliases, char *key) {
  for (alias *head = *aliases; head != NULL; head = head->next) {
    if (strcmp(head->key, key) == 0) {
      return 1;
    }
  }
  return 0;
}

alias *rem_alias(Aliases aliases) {
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
  if (!aliases) {
    printf("there are no aliases\n");
    return 0;
  }
  if (*aliases) {
    if (strcmp((*aliases)->key, key) == 0) {
      rem_alias(aliases);
      return 1;
    }
    alias *head = *aliases;
    while (head->next) {
      if (strcmp(head->next->key, key) == 0) {
        alias *after2Head = head->next->next;
        del_alias(head->next);
        head->next = after2Head;
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

Aliases read_aliases(char *filepath) {
  FILE *fptr = fopen(filepath, "r");
  Aliases a = allocate_aliases();
  if (!fptr) // file doesn't exist, return empty alias list
    return a;

  int c = 1;
  int buffer = 16;

  while (c > 0) {
    char *push = malloc(buffer * sizeof(char));
    int i = 0;

    while ((c = fgetc(fptr)) != '\n' && c > 0) {
      if (i == buffer) {
        buffer <<= 1; // double buffer
        push = realloc(push, buffer);
      }
      push[i++] = c;
    }

    // tokenise

    if (c > 0) {
      push[i++] = '\0';
      int len = 0;
      Tokens push_tok = input_tok(push, &len);
      // get first key
      char *key = malloc(sizeof(char *) * (strlen(push_tok[0]) + 1));
      strcpy(key, push_tok[0]);
      // memmove down
      // double check truncating
      memmove(push_tok, &push_tok[1], (sizeof(char *) * (len - 1)));
      push_tok[len - 1] = NULL;
      add_alias(a, key, push_tok);
    }
  }

  fclose(fptr);
  return a;
}

int save_aliases(Aliases aliases, char *filename) {
  FILE *fptr = fopen(filename, "w");
  if (fptr == NULL) // file does not exist
    return 0;

  alias *head = *aliases;

  while (head) {
    fprintf(fptr, "%s", head->key);

    int j = 0;
    while (head->command[j] != NULL) {
      fprintf(fptr, " %s", strdup(head->command[j]));
      j++;
    }
    fprintf(fptr, "\n");
    head = head->next;
  }
  fclose(fptr);
  return 1;
}

void show_aliases(Aliases aliases) {

  if (*aliases == NULL) {
    printf("No aliases set yet\n");
    return;
  }

  for (alias *head = *aliases; head != NULL; head = head->next) {
    printf("Name:%s   Command:", head->key);
    for (int i = 0; head->command[i] != NULL; i++) {
      printf(" %s", head->command[i]);
    }
    printf("\n");
  }
}

void clear_aliases(Aliases aliases) {
  alias *temp;
  alias *head = *aliases;
  while (head != NULL) {
    temp = head->next;
    free_alias_node(head);
    head = temp;
  }
  *aliases = NULL;
}

void free_aliases(Aliases aliases) {
  clear_aliases(aliases);
  free(*aliases);
}

int command_transform(Aliases aliases, History history, Tokens *tokens,
                      int *num_tokens) {

  int expansion_scalar =
      0; // this is the updated after a token transformation has been done
  AT_List at_list = allocate_at_list();

  for (int i = 0; i < (*num_tokens);) {
    int j = 0;

    // if we are performing the transform on the first command double check for
    // the special case that we follow a chain to get to alias or unalias if we
    // do then stop all transformation
    if (i == 0) {
      if (strcmp((*tokens)[i], "alias") == 0 ||
          strcmp((*tokens)[i], "unalias") == 0) {
        free_at_list(at_list);
        return 0;
      }
    }

    Tokens to_insert = NULL;

    // check if the token is a history invocation
    int is_history = is_history_invocation((*tokens)[i + expansion_scalar]);

    // if it is retrieve it
    if (is_history) {
      to_insert = parse_history_input(history, (*tokens)[i + expansion_scalar]);
      // if we fail to get it return an error
      if (to_insert == NULL) {
        return 1;
      }
    } else {
      // if it is not a history command check if it is an alias
      to_insert = get_alias_command(aliases, (*tokens)[i + expansion_scalar]);
    }

    // if there is a valid replacement
    if (to_insert) {
      int num_alias_tokens = 0;
      // iterate through the command to insert
      while (to_insert[num_alias_tokens] != NULL) {
        // make sure that the command that is being inserted was not the key to
        // what was being inserted prevents cycles a -> a b
        if (strcmp(to_insert[num_alias_tokens], (*tokens)[i + j]) == 0) {
          free_at_list(at_list);
          printf("cycle detected: aborting\n");
          return 1;
        }
        num_alias_tokens++;
      }

      // check we haven't already seen the transformation again
      // if we have return error for cycle
      if (contains_at_node(at_list, (*tokens)[i + j]) == 1) {
        free_at_list(at_list);
        printf("cycle detected: aborting\n");
        return 1;
      }
      // add the transform to a linked list to be able to detect later cycles
      add_at_node(at_list, (*tokens)[i + j]);

      // increase the number of tokens to deal with the command to insert
      *num_tokens += (num_alias_tokens - 1);

      // we need to resize the original to have engouh space
      *tokens =
          (Tokens)realloc((*tokens), ((*num_tokens) * sizeof(char *)) + 1);

      // then memmove it allong num_alias_tokens
      memmove(
          *tokens + (i + j + expansion_scalar) + (num_alias_tokens - 1),
          *tokens + (i + j + expansion_scalar),
          (*num_tokens - (num_alias_tokens - 1) - (i + j + expansion_scalar)) *
              sizeof(char *));

      // then copy in everything
      for (int k = 0; to_insert[k] != NULL; k++) {
        (*tokens)[i + j + k + expansion_scalar] = strdup(to_insert[k]);
      }

      j += num_alias_tokens - 1;
    } else {
      i++;
      expansion_scalar += j;
      // finished the expansion on a specfic index therefore clear the list of
      // transforms
      clear_at_list(at_list);
    }
  }

  // set null at the end of the list to be extra safe
  (*tokens)[(*num_tokens)] = NULL;
  free_at_list(at_list);

  return 0;
}

AT_List allocate_at_list() {
  AT_List list = malloc(sizeof(AT_List));
  *list = NULL;
  return list;
}

at_node *new_at_node(char *key) {
  at_node *node = (at_node *)malloc(sizeof(at_node));
  node->key = malloc(sizeof(char) * (strlen(key) + 1));
  strcpy(node->key, key);
  node->next = NULL;
  return node;
}

int add_at_node(AT_List at_list, char *key) {
  at_node *temp, *head;
  temp = new_at_node(key);

  if (!(*at_list)) {
    *at_list = temp;
    return 0;
  }

  head = *at_list;
  while (head->next) {
    head = head->next;
  }
  head->next = temp;
  return 0;
}

int contains_at_node(AT_List at_list, char *key) {
  for (at_node *head = *at_list; head != NULL; head = head->next) {
    if (strcmp(head->key, key) == 0) {
      return 1;
    }
  }
  return 0;
}

void free_alias_node(alias *node) {
  free(node->key);
  free_tokens(node->command);
}

void free_at_node(at_node *node) {
  free(node->key);
  free(node);
  node = NULL;
}

void clear_at_list(AT_List at_list) {
  at_node *temp;
  at_node *head = *at_list;
  while (head != NULL) {
    temp = head->next;
    free_at_node(head);
    head = temp;
  }
  *at_list = NULL;
}

void free_at_list(AT_List at_list) {
  clear_at_list(at_list);
  free(*at_list);
}
