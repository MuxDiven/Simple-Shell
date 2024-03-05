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

Tokens get_alias_command(Aliases aliases, char *key) {
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
  if (!fptr)
    return NULL;

  Aliases a = allocate_aliases();
  int c = 1;
  int buffer = 16;

  while (c > 0) {
    char *push = malloc(buffer * sizeof(char));
    int i = 0;

    while ((c = fgetc(fptr)) != '\n' && c > 0) {
      if (i == buffer) {
        buffer <<= 1;
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
      memmove(push_tok[0], push_tok[1], (sizeof(char *) * (len - 1)));
      add_alias(a, key, push_tok);
    }
  }

  fclose(fptr);
  return a;
}

int save_aliases(Aliases aliases, char *key, char *filename) {
  FILE *fptr = fopen(filename, "w");
  if (fptr == NULL)
    return 0;

  alias *head = *aliases;

  while (head) {
    fprintf(fptr, "%s", head->key);

    int j = 0;
    while (head->command[j] != NULL) {
      fprintf(fptr, " %s", head->command[j]);
    }
    fprintf(fptr, "\n");
    head = head->next;
  }
  fclose(fptr);
  return 1;
}

int alias_transform(Aliases aliases, Tokens *tokens, int *num_tokens) {

  int j = 0;
  int original_num = *num_tokens;

  for (int i = 0; i < original_num; i++) {
    Tokens alias_command = get_alias_command(aliases, (*tokens)[i + j]);

    if (alias_command) {
      int num_alias_tokens = 0;
      while (alias_command[num_alias_tokens] != NULL) {
        num_alias_tokens++;
      }
      *num_tokens += (num_alias_tokens - 1);

      // we need to resize the original to have engouh space
      *tokens =
          (Tokens)realloc((*tokens), ((*num_tokens) * sizeof(char *)) + 1);

      // then memmve it allong num_alias_tokens
      memmove(*tokens + (i + j) + (num_alias_tokens - 1), *tokens + (i + j),
              (*num_tokens - (num_alias_tokens - 1) - (i + j)) *
                  sizeof(char *));

      // then cpy in everything
      for (int k = 0; alias_command[k] != NULL; k++) {
        (*tokens)[i + j + k] = (char *)malloc(sizeof(alias_command[k]));
        strcpy((*tokens)[i + j + k], alias_command[k]);
      }

      j += num_alias_tokens - 1;
    }
  }

  return 0;
}

void show_aliases(Aliases aliases) {
	alias *head = *aliases;
  for (head; head != NULL; head = head->next) {
    printf("Name:%s   Command:%s\n", head->key, tokens_to_string(head->command));
  }
}

int check_for_alias(Aliases aliases, Tokens tokens){
	alias *head = *aliases;
	for (head; head != NULL; head = head->next) {
		if (head->key == tokens[1]){
			printf("FOUND");
			return 1;
		}
	}
	return 0;
}
