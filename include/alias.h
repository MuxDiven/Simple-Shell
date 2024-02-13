typedef struct alias {
  char *key; // original command
  char **comand;
} alias;

typedef struct alias *aliases;

aliases allocate_aliases();
int save_alias();
int load_alias();
