#include "prompt.h"


#define DELIMITER_TOKEN ";"
#define MAX_COMMAND 5
#define MAX_ARGUMENTS 20

char* remove_spaces(char* command);


//função para veriifcar se foi recebido um um sinal SGUSR1, caso recebido, destroy todos os processos.
int handle_SIGUSR1(int status,List* pid_list){
    if(WTERMSIG(status) == SIGUSR1){
        destroyList(pid_list);
        pid_list = initList();
        print_gandalf();
        return 1;
      }
      return 0;
};

//função para veriifcar se foi recebido um um sinal SGUSR2 (term), caso recebido, destroy todos os processos.
int handle_SIGUSR2(int status,List* pid_list){
   if(WTERMSIG(status) == SIGUSR2){
        destroyList(pid_list);
        return 1;
      }
      return 0;
};

//função para redirecionar a escrita para um arquivo
void redirect_command(char* file);

void print_prompt(void)
{
  printf("psh> ");
}

//Handle para a psh para os sinais definidos
void SIG_VAC(int signo){
  printf("Desista - Estou Vacinado!\n");
}

//Função para contar a quantidade de comandos
static void count_qnt_commands(char *line, int *qtd_comands)
{
  
  for (int i = 0; i < strlen(line); i++)
  {
    if (line[i] == ';')
    {
      (*qtd_comands)++;
    }
  }
}

//Definindo tratadores para os sinais para psh
void block_signals(){
  signal(SIGTERM, SIG_VAC);
  signal(SIGUSR1, SIG_VAC);
  signal(SIGINT, SIG_VAC); 
  signal(SIGQUIT, SIG_VAC);
  signal(SIGTSTP, SIG_VAC);
};

//Função que realiza a leitura dos comandos
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
    char* new_comamnd = remove_spaces(strdup(command));
    commands_array[i] = new_comamnd;
    command = strtok(NULL, DELIMITER_TOKEN);
    i++;
  }



  free(buffer_line);

  return commands_array;
}

//Função que faz o split dos comandos de acordo com um delimitador
static char **split_command_to_exec(char *command, char **command_splited,char delimiter)
{

  int count = 0;
  char *token;

  // counter arguments of command
  for (int i = 0; i < strlen(command); i++)
  {

    token = strtok(command, &delimiter);
    int flag = 0;
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

//função que sera responsavel por colocar o grupo de processos em background para foreground durante 30s
void fg(int pgid){
  tcsetpgrp(0, pgid); //coloca o grupo em fg
  
  sleep(3); //espera 30 segundos
  signal(SIGTTOU, SIG_IGN);//ignora o signal

  
  tcsetpgrp(0, getppid());//coloca o grupo em bg
  
  signal(SIGTTOU, SIG_DFL);//volta ao normal
}

//função que dá o exec para cada comando, definindo seus handlers
int psh_launch(char **commands_array, int qtd_commands, int pipe1[2], List* pid_list)
{
  pid_t pid, wpid;
  int status;
  char **array_parameters;

  if (qtd_commands == 1) // quando o processo vai pertencer ao grupo de não vacinados
  {
    pid = fork();
    if (pid == 0) // filho - cada um tem que ter o seu grupo
    {
      //* verificar se é finalizador
      if(strcmp(commands_array[0], "term") == 0){
        kill(getpid(), SIGUSR2);
      }

      //* verificar se é fg
      if(strcmp(commands_array[0], "fg") == 0){
        int y=0;
        //lẽ e escreve no pipe o pgid do grupo de vacinados
        read(pipe1[0], &y, sizeof(y));
        write(pipe1[1], &y, sizeof(y));
        close(pipe1[0]);
        close(pipe1[1]);
        if(y!=-1){//se for -1 n existe o grupo de vacinados
          fg(y);
        }
      }

      //verificando se é um redirecionamento com '>' e fazendo os tratamentos
      if (strstr(commands_array[0], ">") != NULL)
      {
        array_parameters = split_command_to_exec(commands_array[0], array_parameters, '>'); //separando os parametros do comando
        commands_array[0] = array_parameters[0];
        char* new_command = remove_spaces( array_parameters[1]); //removendo os espaços para criar o arquivo sem espaços
        redirect_command(new_command);   //redirecionando o resultado para o arquivo    
      }
      int new_pgid = setpgid(getpid(),getpid());

      signal(SIGINT, SIG_DFL); //definindo os handles default para os sinais
      signal(SIGTSTP, SIG_DFL);
      signal(SIGQUIT, SIG_DFL);
  
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
      // adicionando o pid do filho na lista
       insertList(pid_list, pid);
    }
  }
  else if (qtd_commands > 1)
  {
    // criando um grupo de processos vacinados e executar os processos
    for (int i = 0; i < qtd_commands; i++)
    {
      pid = fork();
      int x = 0;

      if (pid == 0)
      {
        x=0;
        //le o valor do pipe
        read(pipe1[0], &x, sizeof(x));
        if (x == -1) 
        {
          int first = getpid();//se for o primeiro processo, armazena o PID para tornar esse o pgid do grupo de vacinados
          x = first;

          write(pipe1[1], &first, sizeof(first)); //escreve no pipe o pid do grupo de vacinados
          close(pipe1[0]);  //fecha o pipe de leitura
          close(pipe1[1]);  //fecha o pipe de escrita
        }
        else
        {
          //escrevendo o valor que será o pgid 
          write(pipe1[1], &x, sizeof(x)); //escreve no pipe o pid do grupo de vacinados
          close(pipe1[0]);  //fecha o pipe de leitura
          close(pipe1[1]);  //fecha o pipe de escrita
        }

        //* setando o novo pgid
        int new_pgid = setpgid(getpid(),x);

        // Child process
        char delimiter=' ';
        array_parameters = split_command_to_exec(commands_array[i], array_parameters,delimiter);


        signal(SIGINT, SIG_IGN); // ignorando sinal de interrupção,que vão continuar apos o exec
        signal(SIGTSTP, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);

        if (execvp(array_parameters[0], array_parameters) == -1) //executando o comando
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
        // adicionando o pid da lista de vacinados
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


//Função que imprime a saida de um processo em um arquivo
void redirect_command(char* f){
  int arquivo=open(f,O_WRONLY|O_CREAT|O_TRUNC,0644); //criando o arquivo
  if(arquivo==-1){ //se não conseguir abrir o arquivo
    perror("psh");  //imprime o erro
    exit(EXIT_FAILURE); //encerra o programa
  }
  if(dup2(arquivo,STDOUT_FILENO)==-1){ //se não conseguir duplicar o arquivo
    perror("psh"); //imprime o erro
    exit(EXIT_FAILURE);//encerra o programa
  }
}


//Função que remove os espaços antes e depois do comando
char* remove_spaces(char* command){
  int i=0;
  while(command[i]==' '){
    i++;
  }
  int j=strlen(command)-1;
  while(command[j]==' '){
    j--;
  }
  char* new_command=malloc(sizeof(char)*(j-i+2));
  for(int k=0;k<j-i+1;k++){
    new_command[k]=command[i+k];
  }
  new_command[j-i+1]='\0';
  return new_command;
}

