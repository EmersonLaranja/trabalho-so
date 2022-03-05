#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>


typedef struct celula Celula;

struct celula {
    int pid;
    Celula* prox;
};

struct list {
    Celula* prim;
    Celula* ult;
};


List *initList(void)
{
    List* nova = (List*)malloc(sizeof(List));
    nova->prim=NULL;
    nova->ult=NULL;

    return nova;
}

void insertList(List *list, int pid)
{
    Celula* cel = (Celula*)malloc(sizeof(Celula));
    //adicionando a celula
    cel->pid = pid;
    cel->prox = list->prim;

    list->prim = cel;
    if(list->ult ==  NULL){
        list->ult = cel;
    }
}

//Função que mata os filhos pelo pid salvo na lista
void destroyList(List *list)
{
    Celula* aux= list->prim;
    Celula* prox;

    while( aux != NULL){
        prox = aux->prox;
        kill(aux->pid, SIGKILL); //matando o processo de acordo com o PID
        free(aux); 
        aux = prox;
    }
    free(list); //não liberar a lista
}


