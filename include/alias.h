#include "history.h"

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
alias *rem_alias(Aliases aliases);
int rem_at(Aliases aliases, char *key);
void del_alias(alias *tail);
void show_aliases(Aliases aliases);
Aliases read_aliases(char *filepath);
int check_for_alias(Aliases aliases, char *key);
void free_alias_node(alias *node);
void clear_aliases(Aliases aliases);
void free_aliases(Aliases aliases);

// alias transform strcut
typedef struct at_node {
  char *key;
  struct at_node *next;
} at_node;

int command_transform(Aliases, History h, Tokens *tokens, int *num_tokens);
typedef struct at_node **AT_List;
AT_List allocate_at_list();
at_node *new_at_node(char *key);
int add_at_node(AT_List at_list, char *key);
int contains_at_node(AT_List at_list, char *key);
void free_at_node(at_node *node);
void clear_at_list(AT_List at_list);
void free_at_list(AT_List at_list);
