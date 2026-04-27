#include <stdlib.h>
#include <stdio.h>
#include "thread_pool.h"
int crea_pool(struct elista** inicio, int n)
{
    for(int i=0; i<n; i++)
    {
        int comprobacion=inserta_elista(inicio);
        if(!comprobacion)
        {
            puts("Fallamos al crear el hilo");
            return 0;
        }
    }
    return 1;
}
struct hilo* crea_hilo()
{
    struct hilo* nuevo=(struct hilo *)malloc(sizeof(struct hilo));
    if(!nuevo)
        return NULL;
    nuevo->estado=1;
    nuevo->ID=genera_ID();
    nuevo->tarea_actual=NULL;
    return nuevo;
}

int genera_ID()
{
    static int contador=1;
    return contador++;
}

struct elista * crea_elista(struct hilo* hilo)
{
    struct elista *nuevo =(struct elista *) malloc(sizeof(struct elista));
    if(!nuevo)
        return NULL;
    nuevo->hilo=hilo;
    nuevo->sig=NULL;
    return nuevo;
}

int inserta_elista(struct elista ** inicio)
{
    struct hilo* hilo=crea_hilo();
    struct elista* aux= crea_elista(hilo);
    if(!aux)
        return 0;
    aux->sig=(*inicio);
    (*inicio)=aux;
    return 1;
}

int imprime_lista(struct elista* inicio)
{
    if(!inicio)
    {
        puts("Fin de la lista");
        return 1;
    }
    if(inicio->hilo->estado)
        printf("(ID: %d, ESTADO: LIBRE) \n", inicio->hilo->ID);
    else
        printf("(ID: %d, ESTADO: OCUPADO) \n", inicio->hilo->ID);
    return imprime_lista(inicio->sig);
}

int asigna_hilo(struct elista** inicio, struct proceso* t_asig)
{
    if(!(*inicio))
    {
        puts("No hay hilos disponibles");
        return 0;
    }
    struct elista* aux= hay_hilo_libre(inicio);
    if(aux)
    {
        puts("Hilo disponible encontrado: ");
        aux->hilo->tarea_actual=t_asig;
        aux->hilo->estado=0;
        printf("(ID: %d, TAREA ASIGNADA: %d, ESTADO: OCUPADO) \n", aux->hilo->ID, aux->hilo->tarea_actual->ID);
        return 1;
    }
    else
    {
        puts("no hay hilos disponibles");
        return 0;
    }
}

struct elista* hay_hilo_libre(struct elista** inicio)
{
    if(!(*inicio))
        return NULL;
    if((*inicio)->hilo->estado)
        return (*inicio);
    return hay_hilo_libre(&((*inicio)->sig));
}
int hilos_ocupados(struct elista** inicio)
{
    if(!(*inicio))
        return 0;
    if(!((*inicio)->hilo->estado))
        return 1;
    return hilos_ocupados(&((*inicio)->sig));
}
struct proceso *libera_hilox(struct elista** inicio, int ID)
{
    if(!(*inicio))
    {
        puts("No se encontro el hilo buscado");
        return NULL;
    }
        
    if((*inicio)->hilo->ID==ID )
    {
        if((*inicio)->hilo->estado==0)
        {
            puts("Hilo encontrado: ");
            (*inicio)->hilo->estado=1;
            printf("(ID: %d, ESTADO: LIBRE) \n", (*inicio)->hilo->ID);
            struct proceso* aux=(*inicio)->hilo->tarea_actual;
            (*inicio)->hilo->tarea_actual=NULL;
            return aux;
        }
        else
        {
            puts("Hilo ya disponible");
            return NULL;
        }
    }
    return libera_hilox(&((*inicio)->sig), ID);
}

int libera_lista(struct elista* inicio)
{
    if(!inicio)
        return 1;
    struct elista *aux=inicio;
    inicio=inicio->sig;
    free(aux->hilo);
    free(aux);
    return libera_lista(inicio);
}