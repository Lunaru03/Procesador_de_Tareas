#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "priority_queue.h"
//alojamos memoria dinamica e inicializamos el elemento de la cola
struct ecola *crea_ecola(struct proceso* proceso)
{
    struct ecola *nuevo =(struct ecola *) malloc(sizeof(struct ecola));
    if(!nuevo)
        return NULL;
    nuevo->elemento=proceso;
    nuevo->sig=NULL;
    return nuevo;
}
/*Inserta_ecola usa crea_ecola para el nodo. Si cola->primero no tiene nada, pues la prioridad del proceso es irrelevante, va al comienzo.
Si el elemento es prioritario, busca con buscar_prioridad la posicion del ultimo elemento prioritario. Si encuentra uno, se inserta despues de el.
Si no encuentra otro elemento prioritario, se inserta al principio.
*/
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
        if(!aux)//Si no encuentra, se inserta al inicio.
        {
            nuevo->sig=cola->primero;
            cola->primero=nuevo;
        }
        else//Si encuentra, nuevo sig es igual al sig del aux y auxsig es nuevo, metiendolo en medio.
        {
            nuevo->sig=aux->sig;
            aux->sig=nuevo;
            if(nuevo->sig==NULL)//Necesite este caso especial para cuando metemos un prioritario despues 
                cola->ultimo=nuevo; //de otro, pero no hay ningun nodo no prioritario para que sea su sig y siga siendo el final
        }
        return 1;
    }
    cola->ultimo->sig=nuevo;// Insercion normal.
    cola->ultimo=nuevo;
    return 1;
}

//retorna el puntero que seria el ultimo prioritario de la cola o NULL cuando llega al final y no encuentra ninguno
struct ecola* buscar_prioridad(struct ecola* inicio)
{
    if(!inicio)
        return inicio;
    if(inicio->elemento->prioridad && (inicio->sig==NULL ||inicio->sig->elemento->prioridad==0))//Primero revisamos si es NULL para que 
        return inicio;                                                   //el programa no se rompa al hacer NULL->siguiente.
    return buscar_prioridad(inicio->sig);
}

//Eliminacion normal de cola, pero que regresa el proceso dentro del nodo para mandarlo al hilo de la lista.
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
    free(aux);// liberamos el nodo.
    return proceso;
}

/*Hice esta funcion para que la generacion de procesos fuese variable en cada ocacion.
*/
int encola_procesos_random(struct cola *cola)
{
    int cantidad=genera_numero(1,3);
    struct proceso *proceso= NULL;
    printf("Nuevos procesos: ");
    for(int i=0; i<cantidad; i++)
    {
        proceso=genera_proceso(cola);//primero generamos el proceso para luego mandarlo a insertacola
        inserta_ecola(cola, proceso);
        if(i==cantidad-1)
        {
            printf("(ID=%d, PRIO= %d)\n", proceso->ID, proceso->prioridad); //vamos imprimiendo los objetos que se generan
            break;//break en vez de else 
        }
        printf("(ID=%d, PRIO= %d)", proceso->ID, proceso->prioridad);
    }
    return 1;
}

//hace la alocacion de memoria y la inicializacion del proceso.
struct proceso *genera_proceso(struct cola *cola)
{
    struct proceso *nuevo=(struct proceso *)malloc(sizeof(struct proceso));
    if(!nuevo)
        return NULL;
    nuevo->ID=genera_id();
    nuevo->prioridad=genera_numero(0,1);//generamos aleatoriamente su prioridad para dinamismo.
    return nuevo;
}

//genera id va haciendo ids que no se repiten para generaproceso.
int genera_id()
{
  static int contador=1;
  return contador++;
}

//funcion para inicializar cola y que se vea mejor que simplemente hacerlo en el main.
int inicializar_cola(struct cola *cola)
{
    cola->primero=NULL;
    cola->ultimo=NULL;
    return 1;
}

//imprimir cola recursiva
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

//Mandamos un 1 si la cola esta vacia o 0 si no. Para cuestiones de logica.
int cola_vacia(struct cola* cola)
{
    if(!cola || !cola->primero)
        return 1;
    else   
        return 0;
}

//liberamos todos los nodos de la cola porque somos coders responsables.
//esta funcion ni deberia ser necesaria porque vamos desarmando nuestra cola mientras el programa corre, con sus debidos frees.
int liberar_cola(struct cola* cola)
{
    if(!cola || cola->primero==NULL)
        return 1;
    struct ecola* aux=cola->primero;
    if(aux->elemento!=NULL) //normalmente los nodos de cola no deberian de tener ya un proceso, porque lo mandamos a la lista
        free(aux->elemento);// pero revisamos por si acaso para evitar fugas de memoria.
    cola->primero=cola->primero->sig;
    free(aux);
    return liberar_cola(cola);
}