#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include "prompt.h"
#include <string.h>
#define TRUE 1
#define VACINADOS 2
#define NAO_VACINADOS 4

void SIG_VAC(int signo)
{
  printf("Desista - Estou Vacinado!\n");
}

int main(int argc, char const *argv[])
{
  char **commands_array;
  int qtd_commands = 0;

  system("clear");
  int pipe1[2];

  // tratando sinais para psh
  signal(SIGINT, SIG_VAC);
  signal(SIGQUIT, SIG_VAC);
  signal(SIGTSTP, SIG_VAC);

  if (pipe(pipe1) == -1)
  {
    perror("pipe");
    exit(1);
  }

  int x = -1;
  // close(pipe1[0]);
  write(pipe1[1], &x, sizeof(x));
  // close(pipe1[1]);

  //! trecho que funciona do código
  do
  {

    print_prompt();

    // scanf("%*[^\n]");

    commands_array = read_commands(&qtd_commands);

    // print_commands(commands_array, &qtd_commands);
    if (commands_array != NULL)
    {
      psh_launch(commands_array, qtd_commands, pipe1);
    }

  } while (TRUE);

  free_commands_array(commands_array, qtd_commands);

  return 0;
}
