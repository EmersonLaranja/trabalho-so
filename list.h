#ifndef LISTWITHOUTWARD_H
#define LISTWITHOUTWARD_H
#include <stdio.h>

    typedef struct list List;

    List *initList(void);
    void insertList(List *list, int pid);
    void printList(List *list);
    int getPIDs(List* list);
    List *removeList(List *list, char *chave);
    void destroyList(List *list);


#endif /* LISTWITHOUTWARD_H */