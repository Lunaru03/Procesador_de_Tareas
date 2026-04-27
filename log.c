#include <stdlib.h>
#include <stdio.h>
#include "log.h"
int inicializar_pila(struct pila *pila)
{
    pila->n_elementos=0;
    pila->tope=NULL;
    return 1;
}
int push (struct pila *pila, struct epila *elemento )
{
    elemento->sig=pila->tope;
    pila->tope=elemento;
    pila->n_elementos++;
    return 1;
}

struct epila *pop(struct pila* pila)
{
    struct epila* aux= pila->tope;
    pila->tope= pila->tope->sig;
    pila->n_elementos--;
    return aux;
}

struct epila * crea_epila(struct proceso* proceso)
{
    struct epila *nuevo =(struct epila *) malloc(sizeof(struct epila));
    if(!nuevo)
        return NULL;
    nuevo->proceso=proceso;
    nuevo->sig=NULL;
    return nuevo;
}

int inserta_epila(struct pila* pila, struct proceso* proceso)
{
    struct epila *nuevo= crea_epila(proceso);
    if(!nuevo)
    {
        printf("stack overflow (memoria llena)\n");
        return 0;
    }
    return  push(pila, nuevo);
}

struct epila *saca(struct pila* pila)
{
    if(pila->n_elementos==0)
    {
        printf("Pila vacia\n");
        return NULL;
    }
    return pop(pila);
}
int imprimir_pila(struct pila* pila)
{
    puts("imprimiendo procesos ya finalizados (log)");
    struct epila *aux=saca(pila);
    if(!aux)
        return 0;
    if(pila->n_elementos==0)
    {
        printf("(ID: %d, PRIO: %d) (FIN)\n", aux->proceso->ID, aux->proceso->prioridad);
        push(pila,aux);
        return 1;
    }
    printf("(ID: %d, PRIO: %d) (FIN)\n", aux->proceso->ID, aux->proceso->prioridad);
    imprimir_pila(pila);
    push(pila, aux);
}
