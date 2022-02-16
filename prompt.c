#include "prompt.h"

#define DELIMITER_TOKEN ";"
#define MAX_COMMAND 5
#define MAX_ARGUMENTS 20

int teste = 0;

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

  if (line == -1)
  {

    printf("\n");
    return NULL;
  }

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

  /*
  ?ficar de olho
  recomecando leitura para armazenar comandos
  rewind(stdin);*/

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

  // coma

  free(buffer_line);

  return commands_array;
}

static char **split_command_to_exec(char *command, char **command_splited)
{

  int count = 0;
  char *token;

  // counter arguments of command
  for (int i = 0; i < strlen(command); i++)
  {

    token = strtok(command, " ");
    int flag = 0;
    //! ajustar free e max_arguments numero
    command_splited = (char **)malloc(sizeof(char *) * MAX_ARGUMENTS);
    while (token != NULL)
    {
      command_splited[count] = strdup(token);
      count++;
      token = strtok(NULL, " ");
    }

    return command_splited;
  }
}

// void launch_all_commands(char **commands_array, int qtd_commands, int *first_pid)
// {
//   for (int i = 0; i < qtd_commands; i++)
//   {
//     printf("Comando %s\n", commands_array[i]);
//     // psh_launch(commands_array[i], qtd_commands, first_pid);
//   }
// }

// int psh_launch(char *command, int qtd_commands, int *first_pid)
// {
//   pid_t pid, wpid;
//   int status;
//   char **array_parameters;

//   if (qtd_commands == 1) // Não vacinados
//   {
//     pid = fork();
//     if (pid == 0) //! filho - cada um tem que ter o seu grupo
//     {
//       printf("Grupo do pai: %d,Grupo desse processo Filho: %d\n", getpgid(getppid()), getpgid(getpid()));
//       printf("ID DO FILHO: %d\n", getpid());
//       setpgid(getpid(), getpid()); //! setando o grupo do filho com id do filho
//       printf("Grupo do pai: %d,Grupo desse processo Filho: %d\n", getpgid(getppid()), getpgid(getpid()));
//       // Child process
//       array_parameters = split_command_to_exec(command, array_parameters);
//       if (execvp(array_parameters[0], array_parameters) == -1)
//       {
//         perror("psh");
//       }
//       exit(EXIT_FAILURE);
//     }

//     else if (pid < 0)
//     {
//       // Error forking
//       perror("psh");
//     }

//     else
//     {
//       // Parent process
//       do
//       {
//         wpid = waitpid(pid, &status, WUNTRACED);
//       } while (!WIFEXITED(status) && !WIFSIGNALED(status));
//     }
//   }
//   else if (qtd_commands > 1)
//   {
//     //! criando um grupo de processos vacinados e executar os processos
//     for (int i = 0; i < qtd_commands; i++)
//     {
//       pid = fork();

//       if (pid == 0)
//       {

//         if (*first_pid == -1)
//         {
//           printf("Valor anterior do first_pid: %d\n", *first_pid);
//           *first_pid = getpid();
//           printf("O id do primeiro ----%d\n", *first_pid);
//         }
//         printf("----------------------\n");
//         printf("Grupo do pai: %d,Grupo desse processo Filho: %d\n", getpgid(getppid()), getpgid(getpid()));
//         printf("ID DO FILHO: %d\n", getpid());
//         setpgid(getpid(), *first_pid); //! setando o grupo do filho com id do filho
//         printf("Grupo do pai: %d,Grupo desse processo Filho ALterado: %d\n", getpgid(getppid()), getpgid(getpid()));
//         // Child process
//         array_parameters = split_command_to_exec(command, array_parameters);
//         if (execvp(array_parameters[0], array_parameters) == -1)
//         {
//           perror("psh");
//         }
//         exit(EXIT_FAILURE);
//       }
//     }
//   }

//   return 1;
// }

int psh_launch(char **commands_array, int qtd_commands, int pipe1[2])
{
  pid_t pid, wpid;
  int status;
  char **array_parameters;

  if (qtd_commands == 1) // Não vacinados
  {
    pid = fork();
    if (pid == 0) //! filho - cada um tem que ter o seu grupo
    {
      printf("Grupo do pai: %d,Grupo desse processo Filho: %d\n", getpgid(getppid()), getpgid(getpid()));
      printf("ID DO FILHO: %d\n", getpid());
      setpgid(getpid(), getpid()); //! setando o grupo do filho com id do filho
      printf("Grupo do pai: %d,Grupo desse processo Filho N vacinado: %d\n", getpgid(getppid()), getpgid(getpid()));
      // Child process
      array_parameters = split_command_to_exec(commands_array[0], array_parameters);
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
      do
      {
        wpid = waitpid(pid, &status, WUNTRACED);
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
  }
  else if (qtd_commands > 1)
  {
    //! fazer pipes
    //! criando um grupo de processos vacinados e executar os processos
    for (int i = 0; i < qtd_commands; i++)
    {
      pid = fork();

      if (pid == 0)
      {
        int x = 0;
        read(pipe1[0], &x, sizeof(x));
        printf("Valor do pipe: %d\n", x);
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

        printf("\n----------------------\n");
        printf("Grupo do pai: %d,Grupo desse processo Filho: %d\n", getpgid(getppid()), getpgid(getpid()));
        printf("ID DO FILHO: %d\n", getpid());
        int seila = setpgid(getpid(), x); //! setando o grupo do filho com id do filho
        printf("\t\t\t\tHORA DA VERDADE, VOCES ESTAO PRONTOS?: %d\n", seila);
        printf("Grupo do pai: %d,Grupo desse processo Filho ALterado: %d\n", getpgid(getppid()), getpgid(getpid()));
        // Child process
        array_parameters = split_command_to_exec(commands_array[i], array_parameters);
        if (execvp(array_parameters[0], array_parameters) == -1)
        {
          perror("psh");
        }
        exit(EXIT_FAILURE);
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
