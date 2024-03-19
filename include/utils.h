typedef char **Tokens;

char **input_tok(char *input, int *num_tokens);
char **copy_tokens(Tokens tokens);
void free_tokens(Tokens tokens);
char *tokens_to_string(Tokens tokens);
int count_tokens(Tokens tokens);
