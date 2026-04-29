#include <stdlib.h>
#include <stdio.h>
#include "thread_pool.h"
/*funcion que crea la lista con la cantidad de nodos que el usuario pide.
*/
int crea_pool(struct elista** inicio, int n)
{
    for(int i=0; i<n; i++)
    {
        int comprobacion=inserta_elista(inicio);
        if(!comprobacion)//comprobaciones para asegurar robustez.
        {
            puts("Fallamos al crear el hilo");
            return 0;
        }
    }
    return 1;
}

//alocacion de memoria e inicializacion de los hilos. Estado siempre 1 porque el hilo esta libre por defecto.
//tambien tenemos ids no repetibles con generaid.
struct hilo* crea_hilo()
{
    struct hilo* nuevo=(struct hilo *)malloc(sizeof(struct hilo));
    if(!nuevo)//comprobacion para robustez.
        return NULL;
    nuevo->estado=1;
    nuevo->ID=genera_ID();
    nuevo->tarea_actual=NULL;
    return nuevo;
}

//ids no repetibles.
int genera_ID()
{
    static int contador=1;
    return contador++;
}

//alocacion de memoria e iniciacion del nodo, le mandamos el pointer del hilo para conectarlo con su nodo correspondiente.
struct elista * crea_elista(struct hilo* hilo)
{
    struct elista *nuevo =(struct elista *) malloc(sizeof(struct elista));
    if(!nuevo)//comprobacion para robustez.
        return NULL;
    nuevo->hilo=hilo;
    nuevo->sig=NULL;
    return nuevo;
}

//creamos un hilo, con el que luego creamos el nodo y lo insertamos en la lista, insercion de lista como siempre.
int inserta_elista(struct elista ** inicio)
{
    struct hilo* hilo=crea_hilo();
    if(!hilo)
        return 0; //comprobacion para no mandarle NULL a crea lista.
    struct elista* aux= crea_elista(hilo);
    if(!aux)
        return 0; //comprobaciones por robustez. 
    aux->sig=(*inicio);
    (*inicio)=aux;
    return 1;
}

//imprimir lista recursivo sin nada fuera de lo comun.
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

//Esta funcion usa hay hilo libre para buscar si hay espacio en los hilos para el proceso que se quiere insertar.
int asigna_hilo(struct elista** inicio, struct proceso* t_asig)
{
    if(!(*inicio))
    {
        puts("No hay hilos disponibles");//si la lista estuviera vacia por alguna razon, pues no hay espacio
        return 0;
    }
    struct elista* aux= hay_hilo_libre(inicio);
    if(aux)//si nos manda un puntero, es porque hay espacio donde asignar el proceso, y sabemos donde con el puntero que recibimos.
    {
        puts("Hilo disponible encontrado: ");
        aux->hilo->tarea_actual=t_asig;//le asignamos el proceso al hilo en cuestion
        aux->hilo->estado=0;//estado ocupado
        printf("(ID: %d, TAREA ASIGNADA: %d, ESTADO: OCUPADO) \n", aux->hilo->ID, aux->hilo->tarea_actual->ID);
        return 1;
    }
    else
    {
        puts("no hay hilos disponibles");//si nos manda NULL, no hay hilos disponibles y return 0;
        return 0;
    }
}

// hay hilo libre busca recursivamente la lista para ver si hay un nodo libre.
struct elista* hay_hilo_libre(struct elista** inicio)
{
    if(!(*inicio))// si nos acabamos la lista o esta vacia, return NULL.
        return NULL;
    if((*inicio)->hilo->estado)//Encontramos un hilo disponible.
        return (*inicio);
    return hay_hilo_libre(&((*inicio)->sig));
}

//revisamos si hay al menos un hilo ocupado, ya que significaria que el procesador sigue trabajando.
int hilos_ocupados(struct elista** inicio)
{
    if(!(*inicio))// si nos acabamos la lista o esta vacia, return NULL.
        return 0;
    if(!((*inicio)->hilo->estado))//Encontramos un hilo ocupado
        return 1;
    return hilos_ocupados(&((*inicio)->sig));
}

//buscamos recursivamente el hilo a liberar de forma dinamica. Por cada vez que llamamos la funcion, hay un 50% de liberar el hilo.
struct proceso *libera_hilox(struct elista** inicio)
{
    if(!(*inicio))
        return NULL;
        if((*inicio)->hilo->estado==0 && genera_numero(0,1))
            return libera_aux(inicio);
    return libera_hilox(&((*inicio)->sig));
}


struct proceso* libera_aux(struct elista** inicio)
{
    printf("Hilo a liberar: %d\n ", (*inicio)->hilo->ID);//imprimimos el hilo a liberar
    (*inicio)->hilo->estado=1;//lo marcamos como libre
    printf("(ID: %d, ESTADO: LIBRE) \n", (*inicio)->hilo->ID);//imprimimos que el hilo ya esta libre
    struct proceso* aux=(*inicio)->hilo->tarea_actual;//guardamos el struct del proceso para mandarlo a la pila
    (*inicio)->hilo->tarea_actual=NULL;//le ponemos NULL al puntero del hilo liberado para poder ponerle otro puntero proceso despues.
    return aux;
}

//recorremos la lista recursivamente liberando los hilos y luego el nodo.
int libera_lista(struct elista* inicio)
{
    if(!inicio)//si nos acabamos la lista o ya esta vacia
        return 1;
    struct elista *aux=inicio;//puntero auxiliar para liberar el nodo actual.
    if(aux->hilo->tarea_actual!=NULL)//si por alguna razon el hilo todavia tiene un proceso, lo liberamos para evitar fugas de memoria.
        free(aux->hilo->tarea_actual);
    inicio=inicio->sig;
    free(aux->hilo);//liberamos el hilo
    free(aux);//liberamos el nodo.
    return libera_lista(inicio);
}