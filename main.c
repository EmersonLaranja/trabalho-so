#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include "prompt.h"
#include <string.h>

#define TRUE 1
#define VACINADOS 2
#define NAO_VACINADOS 4

char msg[] = "Control - C pressed!\n";
void catch_ctrl_c(int signo)
{
  printf("Deu ruim - Sem control C");
  exit(1);
}

int main(int argc, char const *argv[])
{
  char **commands_array;
  int qtd_commands = 0;
  system("clear");

  struct sigaction act;
  sigset_t sigset;

/*   sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  sigaddset(&act.sa_mask, SIGTSTP);
  sigaddset(&act.sa_mask, SIGINT);  
  sigaddset(&act.sa_mask, SIGQUIT); 
   */

  if (sigprocmask(SIG_BLOCK, &act.sa_mask, NULL))
    perror("sigprocmask");

  // sigaction(SIGINT, &act, NULL);
  print_gandalf();

  //! trcho que funciona do código
  do
  {
    print_prompt();
    commands_array = read_commands(&qtd_commands);
    

    if(qtd_commands == 1){
      printf("Apenas um processo Não Vacinado\n");
      print_commands(commands_array, &qtd_commands);
    }
    else if(qtd_commands > 1){
      printf("%d processos Vacinados\n",qtd_commands);
      print_commands(commands_array, &qtd_commands);
    }

  } while (TRUE);

  free_commands_array(commands_array, qtd_commands);

  return 0;
}

/*
int pid = fork();
  // filho
  if (pid == 0)
  {
    printf("Sou o filho %d Process Group: %d\n", getpid(), getpgrp());
    // setpgid(0, getpid());//pega o id do processo que chamou e coloca como gpid
    setpgid(0, 0);
    printf("Sou o filho %d Process Group: %d\n", getpid(), getpgid(getpid()));
  }
  else
  {
    printf("Sou o pai %d  Process Group: %d %d\n", getpid(), getpgrp(), getpgid(getpid()));
    // setpgid(pid, getpid());
    // getpid nos segundo parametro muda o pgd do filho para o mesmo do pai
    printf("alterando filho\n");
    printf("testando\n");
    printf("Processo G do filho agr %d\n", getpgid(pid));
    // sleep(10000);
  }
*/

/*

 int pid = fork();
  int pid2 = fork();

  if (pid == 0)
  {
    printf("Sou o filho 1 e tenho pid%d\n", getpid());
    printf("Sou o filho 1 e tenho grupo de processos%d\n", getpgrp());
    int pid3 = fork();
    if (pid3 == 0)
    {
      printf("Sou o filho do filho 1 e tenho pid%d\n", getpid());
      printf(" Sou o filho do filho 1 e Meu grupo de processo é %d\n", getpgrp());
      setpgid(getppid(), getppid());
      printf("Sou o filho do filho 1 e Meu grupo de processo Alterado é %d\n", getpgrp());
    }
  }
  else if (pid2 == 0)
  {
    printf("Sou o filho 2 e tenho pid%d\n", getpid());
    printf("Sou o filho 2 e Meu grupo de processo é %d\n", getpgrp());
    int pid4 = fork();
    if (pid4 == 0)
    {
      printf("Sou o filho do filho 2 e tenho pid%d\n", getpid());
      printf("Sou o filho do filho 2 e Meu grupo de processo é %d\n", getpgrp());
      setpgid(getppid(), getpid());
      printf("Sou o filho do filho 2 e Meu grupo de processo Alterado é %d\n", getpgrp());
    }
  }
  else
  {
    printf("Sou o pai e tenho pid%d\n", getpid());
    wait(NULL);
  }

 */