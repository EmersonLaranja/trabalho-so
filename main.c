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


  block_signals();//bloqueia os sinais na psh
  
  // criando pipe
  if (pipe(pipe1) == -1)
  {
    perror("pipe");
    exit(1);
  }

  int x = -1; // variavel para verificar o primeiro processo que acessa o pipe 
  write(pipe1[1], &x, sizeof(x));//escreve no pipe o valor -1


  do
  {    
    print_prompt();

    commands_array = read_commands(&qtd_commands);

    if (commands_array != NULL)
    {
      psh_launch(commands_array, qtd_commands, pipe1, pid_list); //executa os comandos
    }

    int status;//* status para fazer autopsia nos filhos
    waitpid(-1, &status,WNOHANG); 



      //* verificando se o processo filho morreu por causa do SIGUSR1
      //* começou a sepa do sigusr1, todos filhos morrem
      if(handle_SIGUSR1(status,pid_list)){
         status = 0;
      }
      
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
echo oi;

./loop;./loop;./loop;ls;
*/