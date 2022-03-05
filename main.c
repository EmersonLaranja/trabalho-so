#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include "prompt.h"
#include <string.h>
#include "list.h"



int main(int argc, char const *argv[])
{
  char **commands_array;
  int qtd_commands = 0;
  List* pid_list;
  int pipe1[2];
  
  
  pid_list=initList();
  
  system("clear");


  block_signals();
  
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

    commands_array = read_commands(&qtd_commands);

    if (commands_array != NULL)
    {
      psh_launch(commands_array, qtd_commands, pipe1, pid_list);
    }

    int status;//* status para fazer autopsia nos filhos
    waitpid(-1, &status,WNOHANG);

      //* verificando se o processo filho morreu por causa do SIGUSR1
      //* começou a sepa do sigusr1, todos filhos morrem
      handle_SIGUSR1(status,pid_list);
     

      //* verificando se o processo filho morreu por causa do SIGUSR2
      //* identificando que o term foi digitado
      if(handle_SIGUSR2(status,pid_list))
        break;
      
    

  } while (TRUE);

  return 0;
}


/*
xcalc;
./loop       ; ./loop      ;
    xcalc; xcalc; ls;ps ;
;    ; ls;
echo oi;
pstree | grep a;
code .;

./loop;./loop;./loop;ls;
*/