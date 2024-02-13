#include "utils.h"

typedef struct alias {
  char *key;      // original command
  char **command; // tokens(potential flags attached to alias)
  struct alias *next;
} alias;

typedef struct alias **Aliases;

void allocate_aliases(); // init
alias *new_alias(char *key, Tokens command, int num_tok);
int add_alias(Aliases aliases, char *key, Tokens args, int num_tok);
int save_alias(Aliases aliases, char *key, char *filename);
int is_alias(Aliases aliases, char *key);
int load_alias();
Tokens get_command(Aliases aliases, char *key);
alias rem_alias(Aliases aliases, char *key);
void del_alias(Aliases aliases, char *key);
