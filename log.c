#include <stdlib.h>
#include <stdio.h>
#include "log.h"
//inicializamos el struct pila para no tener que hacerlo en el main
int inicializar_pila(struct pila *pila)
{
    pila->n_elementos=0;
    pila->tope=NULL;
    return 1;
}

//push normal de pila
int push (struct pila *pila, struct epila *elemento )
{
    elemento->sig=pila->tope;
    pila->tope=elemento;
    pila->n_elementos++;
    return 1;
}

//pop normal de pila
struct epila *pop(struct pila* pila)
{
    struct epila* aux= pila->tope;
    pila->tope= pila->tope->sig;
    pila->n_elementos--;
    return aux;
}

//alocacion de memoria del nodo de pila
struct epila * crea_epila(struct proceso* proceso)
{
    struct epila *nuevo =(struct epila *) malloc(sizeof(struct epila));
    if(!nuevo)//comprobacion por robustez.
        return NULL;
    nuevo->proceso=proceso;//recibimos el puntero de proceso para asignarlo al nuevo nodo.
    nuevo->sig=NULL;
    return nuevo;
}

//push pero con checkeo para evitar stack overflow
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

//pop con checkeo para evitar stack underflow
struct epila *saca(struct pila* pila)
{
    if(pila->n_elementos==0)
    {
        printf("Pila vacia\n");
        return NULL;
    }
    return pop(pila);
}

//recorremos recursivamente la pila para imprimir los procesos guardados, al terminar todas las llamadas recursivas, volvemos a pushear todos los nodos.
int imprimir_pila(struct pila* pila)
{
    struct epila *aux=saca(pila);
    if(!aux)// comprobacion por robustez.
        return 0;
    if(pila->n_elementos==0)
    {//comprobacion necesaria para no perder el ultimo nodo de la pila al salir de la recursion.
        printf("(ID: %d, PRIO: %d) (FIN)\n", aux->proceso->ID, aux->proceso->prioridad);
        push(pila,aux);
        return 1;
    }
    printf("(ID: %d, PRIO: %d)\n", aux->proceso->ID, aux->proceso->prioridad);
    imprimir_pila(pila);
    push(pila, aux);
}

/*Cuando estaba pensando en la cuestion de los archivos, decidi que no queria simplemente hacer un archivo con el mismo nombre siempre
para que no se sobreescriba y poder ver los cambios entre ejecucion y ejecucion
*/
int generar_log(struct pila* pila)
{
    int contador=1, bandera=1;
    char nombre[12];
    FILE* file=NULL;
    while(bandera)
    {
        sprintf(nombre, "log%d.txt", contador);//sprintf concatena un texto con una variable 
        file=fopen(nombre, "r");//buscamos abrir el archivo logx.text
        if(!file)
        {
            file=fopen(nombre, "w");//si no lo encuentra, entonces lo creamos, y este sera el logx de esta ejecucion.
            bandera=0;
        }
        else
        {
            fclose(file); //Si lo encuentra, lo cerramos
            contador++;//aumentamos el contador para intentarlo de nuevo con el siguiente indice
        }
    }
    if(!file)//comprobacion para robustez.
        return 0;
    rellenar_log(pila, file);//al crear el archivo txt, llamamos a la funcion que imprimira sobre el
    return 1;
}

//recorremos recursivamente la pila como en imprimir, pero ahora imprimiendo en el archivo txt con la funcion rellenar aux.
int rellenar_log(struct pila* pila, FILE* file)
{
    struct epila*aux = saca(pila);
    if(!aux)//comprobacion para robustez.
    {
        fclose(file);
        return 0;
    }
    if(pila->n_elementos==0)// comprobacion para no perder el ultimo elemento de la pila.
    {
        rellenar_aux(aux, file);
        push(pila, aux);
        fclose(file);
        return 1;
    }
    rellenar_aux(aux, file);//llamamos a la funcion que imprime en el archivo, mandandole el proceso actual y el puntero file.
    pila->tope=aux->sig;
    rellenar_log(pila, file);
    push(pila, aux);
}
int rellenar_aux(struct epila* aux, FILE* file)
{
    char prioridad[12]="PRIORITARIO";
    char prioridad2[7]="NORMAL";
    if((aux->proceso->prioridad))//un if para imprimir en texto el estado de prioridad.
    fprintf(file, "(PROCESO ID: %d, PRIORIDAD: %s)\n",
    aux->proceso->ID, prioridad);
    else
        fprintf(file, "(PROCESO ID: %d, PRIORIDAD: %s)\n",
        aux->proceso->ID, prioridad2);
    return 1;
}

//vamos liberando los nodos de pila uno por uno
int liberar_pila(struct pila* pila)
{
    if(!pila || pila->tope==NULL) //si la pila esta vacia o si ya nos la acabamos.
        return 1;
    struct epila* aux=pila->tope;
    pila->tope=pila->tope->sig;
    free(aux->proceso);//usamos un aux para liberar el proceso.
    free(aux);//para luego poder liberar el nodo actual. Evitando cualquier fuga de memoria.
    return liberar_pila(pila);
}