#include <stdlib.h>
#include <stdlib.h>
#include <time.h>
#include "thread_pool.h"
#include "priority_queue.h"
#include "log.h"
int comienzo();
int comprobar_entrada();
int hilos_trabajando(struct elista ** lista, struct pila* pila, int n);
int procesoa_hilo(struct elista** inicio, struct cola* cola);
int procesador_de_tareas(struct elista** inicio, struct cola* cola, struct pila* pila, int n);
int *lista_hilos_ocupados(struct elista* inicio, int n);
int imprimir_sistema(struct elista** inicio, struct cola* cola, struct pila* pila);
int main(int argc, char const*argv[])
{
    srand(time(NULL));
    int comprobacion=comienzo();
    if(comprobacion)
    {
        puts("El programa se ejecuto correctamente");
        return 1;
    }
    puts("hubo un error");
    return 0;
}
int comienzo()
{
    struct elista* inicio=NULL;
    struct cola cola;
    struct pila pila;
    int n, comprobacion=1;
    inicializar_cola(&cola);
    inicializar_pila(&pila);
    puts("Introduce el numero de hilos con los que quieres que trabaje el procesador");
    n=comprobar_entrada();
    crea_pool(&inicio, n);
    puts("Visualizacion de la threadpool con la que se trabajara");
    imprime_lista(inicio);
    system("pause");
    system("cls");
    comprobacion=procesador_de_tareas(&inicio, &cola, &pila, n);
    if(!comprobacion)
        return 0;
    return 1;
}
int comprobar_entrada()
{
    int n=0;
    char entrada[100];
    while (fgets(entrada, sizeof(entrada), stdin)) {
    if (sscanf(entrada, "%d", &n) == 1) {
      break; // Success: break out of the loop
    } else {
      printf("Entrada invalida, por favor ingresa un numero entero positivo: "); 
    }
  }
  return n;
}
int procesador_de_tareas(struct elista** inicio, struct cola* cola, struct pila* pila, int n)
{
    if(n==0 || !inicio)
        return 0;
    int np, comprobacion=1; //np= numero de veces que se generan procesos.
    puts("Cuantas llegadas de procesos quieres que el sistema haga (la cantidad de procesos por llegada es aleatoria) ");
    np=comprobar_entrada();
    while(np>0  || !cola_vacia(cola) || hilos_ocupados(inicio))
    {
        if(np>0)
        {
            encola_procesos_random(cola);
            puts("imprimimos la cola");
            imprimir_cola(cola->primero);
            np--;
        }
        procesoa_hilo(inicio, cola);
        comprobacion=hilos_trabajando(inicio, pila, n);
        if(!comprobacion) 
            return 0;
        system("pause");
        system("cls");
        imprimir_sistema(inicio, cola, pila);
        system("pause");
        system("cls");
    }
    puts("terminamos el sistema");
    generar_log(pila);
    liberar_cola(cola);
    libera_lista((*inicio));
    liberar_pila(pila);
    return 1;
}
int procesoa_hilo(struct elista** inicio, struct cola* cola)
{
    if(cola_vacia(cola))
    {
        puts("Cola vacia");
        return 1;
    }
    if(!hay_hilo_libre(inicio))
    {
        puts("Espacio insuficiente en la thread pool");
        return 0;
    }
    struct proceso* aux=elimina_ecola(cola);
    asigna_hilo(inicio, aux);
    return procesoa_hilo(inicio, cola);
}
int hilos_trabajando(struct elista ** lista, struct pila* pila, int n)
{
    if(n==0 || !lista) 
        return 0;
    int min=1, max=n/2, hilos;
    struct proceso* proceso_aux;
    hilos=genera_numero(min, max);
    imprime_lista((*lista));
    for(int i=0; i<hilos; i++)
    {
        proceso_aux=libera_hilox(lista);
        if(proceso_aux)
        inserta_epila(pila, proceso_aux);
    }
    return 1;
}

int imprimir_sistema(struct elista** inicio, struct cola* cola, struct pila* pila)
{
    imprime_lista((*inicio));
    imprimir_cola(cola->primero);
    imprimir_pila(pila);
    return 1;
}