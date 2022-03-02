#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include "prompt.h"
#include <string.h>
#include "list.h"

#define TRUE 1
#define VACINADOS 2
#define NAO_VACINADOS 4

 List* pid_list;

void SIG_VAC(int signo){
  printf("Desista - Estou Vacinado!\n");
}

void SIG_USR1(){
  printf("CEPA DO USR1 - TODOS MORREM MENOS EU!\n");
  destroyList(pid_list);
}

int main(int argc, char const *argv[])
{
  char **commands_array;
  int qtd_commands = 0;
  pid_list=initList();
  
  system("clear");
  int pipe1[2];

  // tratando sinais para psh
  signal(SIGUSR1, SIG_USR1);
  signal(SIGINT, SIG_VAC);
  signal(SIGQUIT, SIG_VAC);
  signal(SIGTSTP, SIG_VAC);
  signal(SIGTERM, SIG_VAC);

  


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
  


  //! variavel status 
 

  do
  {
    
    print_prompt();

    // scanf("%*[^\n]");

    commands_array = read_commands(&qtd_commands);

    // print_commands(commands_array, &qtd_commands);
    if (commands_array != NULL)
    {
      
      psh_launch(commands_array, qtd_commands, pipe1, pid_list);
      //printList(pid_list);
    }

    int status;
    waitpid(-1, &status,WUNTRACED);
    if(WIFSIGNALED(status) ||  WIFEXITED(status)){
      printf("Processo %d morreu\n", status);
     // printf("Processo %d terminou com status %d\n", WEXITSTATUS(status), status);      
      if(WTERMSIG(status) == SIGUSR1){
        printf("Processo filho morreu com code %d mata o resto\n", WTERMSIG(status));
        destroyList(pid_list);
      }
    }

  } while (TRUE);

  free_commands_array(commands_array, qtd_commands);

  return 0;
}
//! verificar pq adicionar varios do mesmo grupo está fucionando
//! verificar porque esta dando erro quando a lista é limpada depois e depois temtamos matar matar todo mundo