typedef char **Tokens;

char **input_tok(char *input, int *num_tokens);
char **copy_tokens(Tokens tokens, const int num_tokens);
void free_tokens(Tokens tokens, const int num_tokens);
