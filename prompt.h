#if !defined(PROMPT_H)
#define PROMPT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void print_prompt(void);
char **read_commands(int *qtd_commands);
void print_commands(char **commands_array, int *qtd_commands);
void print_gandalf(void);
int psh_launch(char **commands_array, int qtd_commands, int *first_pid);
void launch_all_commands(char **commands_array, int qtd_commands, int *first_pid);
void free_commands_array(char **commands_array, int qtd_commands);
#endif // PROMPT_H
