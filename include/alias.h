#include "utils.h"

typedef struct alias {
  char *key;      // original command
  char **command; // tokens(potential flags attached to alias)
  struct alias *next;
} alias;

typedef struct alias **Aliases;

Aliases allocate_aliases(); // init
alias *new_alias(char *key, Tokens command);
int add_alias(Aliases aliases, char *key, Tokens args);
int save_aliases(Aliases aliases, char *filename);
int is_alias(Aliases aliases, char *key);
int load_alias();
int is_alias(Aliases aliases, char *key);
Tokens get_alias_command(Aliases aliases, char *key);
alias *rem_alias(Aliases aliases, char *key);
int rem_at(Aliases aliases, char *key);
void del_alias(alias *tail);
void show_aliases(Aliases aliases);
Aliases read_aliases(char *filepath);
int check_for_alias(Aliases aliases, Tokens tokens);
int alias_transform(Aliases, Tokens *tokens, int *num_tokens);
