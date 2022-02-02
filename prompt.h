#if !defined(PROMPT_H)
#define PROMPT_H

void print_prompt(void);
char **read_commands(int *qtd_commands);
void print_commands(char **commands_array, int *qtd_commands);
void print_gandalf(void);
void free_commands_array(char **commands_array, int qtd_commands);
#endif // PROMPT_H
