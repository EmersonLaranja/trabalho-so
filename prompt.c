#include "prompt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DELIMITER_TOKEN ";"
#define MAX_COMMAND 5

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
  if (line <= 1)
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

  print_commands(commands_array, qtd_commands);

  free(buffer_line);

  return commands_array;
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
