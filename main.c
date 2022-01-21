#include <stdio.h>
#include <stdlib.h>
#include "prompt.h"
#define TRUE 1

int main(int argc, char const *argv[])
{
  char **commands_array;
  int qtd_commands = 0;
  system("clear");

  // do
  // {
    print_prompt();
    commands_array = read_commands(&qtd_commands);

  // } while (TRUE);

free_commands_array(commands_array,qtd_commands);
  

  return 0;
}
