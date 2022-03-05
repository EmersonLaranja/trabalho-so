#if !defined(PROMPT_H)
#define PROMPT_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "list.h"

#define TRUE 1
#define VACINADOS 2
#define NAO_VACINADOS 4

void print_prompt(void);
void block_signals(void);
 void handle_SIGUSR1(int status,List* pid_list);

 int handle_SIGUSR2(int status,List* pid_list);

void SIG_VAC(int signo);
char **read_commands(int *qtd_commands);
void print_commands(char **commands_array, int *qtd_commands);
void print_gandalf(void);
int psh_launch(char **commands_array, int qtd_commands, int pipe1[2], List* pid_list);
void launch_all_commands(char **commands_array, int qtd_commands);
void free_commands_array(char **commands_array, int qtd_commands);
#endif // PROMPT_H
