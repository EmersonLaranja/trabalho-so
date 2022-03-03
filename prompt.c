#include "prompt.h"
#include <signal.h>
#include <fcntl.h>
#include <string.h>

#include <stdio.h>
#define DELIMITER_TOKEN ";"
#define MAX_COMMAND 5
#define MAX_ARGUMENTS 20

void redirect_command(char* file);

void print_prompt(void)
{
  printf("psh> ");
}

static void count_qnt_commands(char *line, int *qtd_comands)
{
  // contando qnt comandos
  for (int i = 0; i < strlen(line); i++)
  {
    if (line[i] == ';')
    {
      (*qtd_comands)++;
    }
  }
}

void print_commands(char **commands_array, int *qtd_commands)
{
  for (int i = 0; i < *qtd_commands; i++)
  {
    printf("%s\n", commands_array[i]);
  }
}

char **read_commands(int *qtd_commands)
{

  // ler a linha
  char *buffer_line = NULL;
  size_t buffer_line_size = 0;

  // retorna a quantidade de char
  int line = getline(&buffer_line, &buffer_line_size, stdin);
  // verificando se o comando é valido

  if (line == 1)
  {
    printf("Digite um comando valido!\n");
    return NULL;
  }
  // tem pelo menos 1 comando
  (*qtd_commands)++;

  // transformando conteudo em string para usar strlen
  buffer_line[strlen(buffer_line) - 1] = '\0';

  count_qnt_commands(buffer_line, qtd_commands);

  // alocando memoria para o array de comandos
  char **commands_array = (char **)malloc(sizeof(char *) * (*qtd_commands));

  char *command = strtok(buffer_line, DELIMITER_TOKEN);

  int i = 0;
  *qtd_commands = 0;
  while (command != NULL && *qtd_commands < MAX_COMMAND)
  {

    (*qtd_commands)++;
    commands_array[i] = strdup(command);
    command = strtok(NULL, DELIMITER_TOKEN);
    i++;
  }


  free(buffer_line);

  return commands_array;
}

static char **split_command_to_exec(char *command, char **command_splited,char delimiter)
{

  int count = 0;
  char *token;

  // counter arguments of command
  for (int i = 0; i < strlen(command); i++)
  {

    token = strtok(command, &delimiter);
    int flag = 0;
    //! ajustar free e max_arguments numero
    command_splited = (char **)malloc(sizeof(char *) * MAX_ARGUMENTS);
    while (token != NULL)
    {
      command_splited[count] = strdup(token);
      count++;
      token = strtok(NULL, &delimiter);
    }

    return command_splited;
  }
}

int psh_launch(char **commands_array, int qtd_commands, int pipe1[2], List* pid_list)
{
  pid_t pid, wpid;
  int status;
  char **array_parameters;

  if (qtd_commands == 1) // Não vacinados
  {
    pid = fork();
    if (pid == 0) //! filho - cada um tem que ter o seu grupo
    {
      //! filho
      if(strcmp(commands_array[0], "term") == 0){
        kill(getpid(), SIGUSR2);
      }

      //verificando se é um redirecionamento com '>' e fazendo os tratamentos
      if (strstr(commands_array[0], ">") != NULL)
      {
        array_parameters = split_command_to_exec(commands_array[0], array_parameters, '>'); 
        commands_array[0] = array_parameters[0];
        printf("NOME DO ARQUIVO:%s\n", array_parameters[1]);
        redirect_command(array_parameters[1]);
       // redirect_command(strstr(array_parameters[0], ">"));
      }
      signal(SIGINT, SIG_DFL);
      signal(SIGTSTP, SIG_DFL);
      signal(SIGQUIT, SIG_DFL);
    
      // Child process
      char delimiter=' ';
      array_parameters = split_command_to_exec(commands_array[0], array_parameters,delimiter);
      if (execvp(array_parameters[0], array_parameters) == -1)
      {
        perror("psh");
      }
      exit(EXIT_FAILURE);
    }

    else if (pid < 0)
    {
      // Error forking
      perror("psh");
    }

    else
    {
      // Parent process
      //! adicionando o pid do filho na lista
       insertList(pid_list, pid);

    }
  }
  else if (qtd_commands > 1)
  {
    //! fazer pipes
    //! criando um grupo de processos vacinados e executar os processos
    for (int i = 0; i < qtd_commands; i++)
    {
      pid = fork();
      int x = 0;

      if (pid == 0)
      {
        x=0;
        read(pipe1[0], &x, sizeof(x));
       // printf("Valor do pipe: %d\n", x);
        if (x == -1)
        {
          int fist = getpid();
          x = fist;
          printf("O id do primeiro ----%d\n", fist);

          write(pipe1[1], &fist, sizeof(fist));
          close(pipe1[0]);
          close(pipe1[1]);
        }
        else
        {
          write(pipe1[1], &x, sizeof(x));
          printf("Valor de x: %d\n", x);
          close(pipe1[0]);
          close(pipe1[1]);
        }

        // Child process
        char delimiter=' ';
        array_parameters = split_command_to_exec(commands_array[i], array_parameters,delimiter);

        signal(SIGINT, SIG_IGN); //! ignorando sinal de interrupção, vão continuar apos o exec
        signal(SIGTSTP, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);

        if (execvp(array_parameters[0], array_parameters) == -1)
        {
          perror("psh");
        }
        exit(EXIT_FAILURE);
      }
      else if(pid < 0)
      {
        perror("psh");
      }
      else
      {
        //! adicionando o pid da lista de vacinados
          insertList(pid_list, pid);
      }
    }
  }

  return 1;
}

void print_gandalf(void)
{
  printf("\n"
         "                        ,---.\n"
         "                        /    |\n"
         "                       /     |\n"
         "                      /      |\n"
         "                     /       |\n"
         "                ___,'        |\n"
         "              <  -'          :\n"
         "               `-.__..--'``-,_\\_\n"
         "                  |o/ ` :,.)_`>\n"
         "                  :/ `     ||/)\n"
         "                  (_.).__,-` |\\ \n"
         "                  /( `.``   `| :\n"
         "                  \'`-.)  `  ; ;\n"
         "                  | `       /-<\n"
         "                  |     `  /   `.\n"
         "  ,-_-..____     /|  `    :__..-'\\\n"
         " /,'-.__\\  ``-./ :`      ;       \\\n"
         " `\\ `\\  `\\  \\ :  (   `  /  ,   `. \\\n"
         "   \\` \\   \\   |  | `   :  :     .\\\\\n"
         "    \\ `\\_  ))  :  ;     |  |      ): :\n"
         "   (`-.-'\\ ||  |\\ \\   ` ;  ;       | |\n"
         "    \\-_   `;;._   ( `  /  /_       | |\n"
         "     `-.-.// ,'`-._\\__/_,'         ; |\n"
         "        \\:: :     /     `     ,   /  |\n"
         "         || |    (        ,' /   /   |\n"
         "         ||                ,'   /    |\n"
         " ________ Unfortunately all process died!________\n"
         " ___ Vaccination should be a social contract!____\n"
         " ____Cooperation was the morally right choice!___ \n");
}

void free_commands_array(char **commands_array, int qtd_commands)
{
  for (int i = 0; i < qtd_commands; i++)
  {
    free(commands_array[i]);
  }
  free(commands_array);
}

//function to write the output of a process in file
//function to print the output of exec in the file
void redirect_command(char* file){
  int fd=open(file,O_WRONLY|O_CREAT|O_TRUNC,0644);
  if(fd==-1){
    perror("psh");
    exit(EXIT_FAILURE);
  }
  if(dup2(fd,STDOUT_FILENO)==-1){
    perror("psh");
    exit(EXIT_FAILURE);
  }
}
