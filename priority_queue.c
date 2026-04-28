#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "priority_queue.h"

struct ecola *crea_ecola(struct proceso* proceso)
{
    struct ecola *nuevo =(struct ecola *) malloc(sizeof(struct ecola));
    if(!nuevo)
        return NULL;
    nuevo->elemento=proceso;
    nuevo->sig=NULL;
    return nuevo;
}

int inserta_ecola(struct cola *cola, struct proceso *proceso)
{
    struct ecola* nuevo= crea_ecola(proceso);
    if(!nuevo)
        return 0;
    if(!(cola->primero))
    {
        cola->primero=nuevo;
        cola->ultimo=nuevo;
        return 1;
    }
    if(nuevo->elemento->prioridad)
    {
        struct ecola*aux=buscar_prioridad(cola->primero);
        if(!aux)
        {
            nuevo->sig=cola->primero;
            cola->primero=nuevo;
        }
        else
        {
            nuevo->sig=aux->sig;
            aux->sig=nuevo;
            if(nuevo->sig==NULL)
                cola->ultimo=nuevo;
        }
        return 1;
    }
    cola->ultimo->sig=nuevo;
    cola->ultimo=nuevo;
    return 1;
}
   
struct ecola* buscar_prioridad(struct ecola* inicio)
{
    if(!inicio)
        return inicio;
    if(inicio->elemento->prioridad && (inicio->sig==NULL ||inicio->sig->elemento->prioridad==0))
        return inicio;
    return buscar_prioridad(inicio->sig);
}
struct proceso* elimina_ecola(struct cola *cola)
{
    if(!cola)
        return 0;
    struct ecola* aux=cola->primero;
    struct proceso* proceso=cola->primero->elemento;
    if(cola->primero==cola->ultimo)
    {
        cola->primero=NULL;
        cola->ultimo=NULL;
    }
    else
        cola->primero=cola->primero->sig;
    free(aux);
    return proceso;
}

int encola_procesos_random(struct cola *cola)
{
    int cantidad=genera_numero(1,3);
    struct proceso *proceso= NULL;
    printf("Nuevos procesos: ");
    for(int i=0; i<cantidad; i++)
    {
        proceso=genera_proceso(cola);
        inserta_ecola(cola, proceso);
        if(i==cantidad-1)
        {
            printf("(ID=%d, PRIO= %d)\n", proceso->ID, proceso->prioridad);
            break;
        }
        printf("(ID=%d, PRIO= %d)", proceso->ID, proceso->prioridad);
    }
    return 1;
}

struct proceso *genera_proceso(struct cola *cola)
{
    struct proceso *nuevo=(struct proceso *)malloc(sizeof(struct proceso));
    if(!nuevo)
        return NULL;
    nuevo->ID=genera_id();
    nuevo->prioridad=genera_numero(0,1);
    return nuevo;
}

int genera_id()
{
  static int contador=1;
  return contador++;
}

int inicializar_cola(struct cola *cola)
{
    cola->primero=NULL;
    cola->ultimo=NULL;
    return 1;
}

int imprimir_cola(struct ecola *aux)
{
    if(!aux)
        return 1;
    if(aux->sig==NULL)
    {
        printf("(ID: %d, PRIORIDAD: %d)\n", aux->elemento->ID, aux->elemento->prioridad);
        return 1;
    }
    printf("(ID: %d, PRIORIDAD: %d)\n", aux->elemento->ID, aux->elemento->prioridad);
    return imprimir_cola(aux->sig);
}

int cola_vacia(struct cola* cola)
{
    if(!cola || !cola->primero)
        return 1;
    else   
        return 0;
}

int liberar_cola(struct cola* cola)
{
    if(!cola || cola->primero==NULL)
        return 1;
    struct ecola* aux=cola->primero;
    if(aux->elemento!=NULL)
        free(aux->elemento);
    cola->primero=cola->primero->sig;
    free(aux);
    return liberar_cola(cola);
}