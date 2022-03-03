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


void SIG_VAC(int signo){
  printf("Desista - Estou Vacinado!\n");
}

int main(int argc, char const *argv[])
{
  char **commands_array;
  int qtd_commands = 0;
  List* pid_list;
  pid_list=initList();
  
  system("clear");
  int pipe1[2];

  // tratando sinais para psh
  signal(SIGUSR1, SIG_VAC);
  //signal(SIGINT, SIG_VAC);
  //signal(SIGQUIT, SIG_VAC);
  signal(SIGTSTP, SIG_VAC);
  signal(SIGTERM, SIG_VAC);

  
  // criando pipe
  if (pipe(pipe1) == -1)
  {
    perror("pipe");
    exit(1);
  }

  int x = -1;
  write(pipe1[1], &x, sizeof(x));


 
  do
  {    
    print_prompt();

    // scanf("%*[^\n]");

    commands_array = read_commands(&qtd_commands);

    if (commands_array != NULL)
    {
      psh_launch(commands_array, qtd_commands, pipe1, pid_list);
    }

    int status;
    waitpid(-1, &status,WNOHANG);

      //! verificando se o processo filho morreu por causa do SIGUSR1
      if(WTERMSIG(status) == SIGUSR1){
        //printf("Processo filho morreu com code %d mata o resto\n", WTERMSIG(status));
        destroyList(pid_list);
        pid_list = initList();
        print_gandalf();
      }

      //! verificando se o processo filho morreu por causa do SIGUSR2
      if(WTERMSIG(status) == SIGUSR2){
        printf("Processo filho morreu com code %d mata o resto\n", WTERMSIG(status));
        destroyList(pid_list);
        break;
      }
    

  } while (TRUE);

  //free_commands_array(commands_array, qtd_commands);//! verificar depois

  return 0;
}

//! verificar depois sobre matar todos os pids de vaciandos e nao vacinados
//! dá para otimizar matando o grupo de vacinados ao inves de cada um separado
// ./loop;./loop; xcalc; xcalc; ping google.com


