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
int genera_error(struct pila* pila, struct cola* cola);
int pausar();
int limpiar();
int main(int argc, char const*argv[])
{
    srand(time(NULL));//inicializamos la semilla para rand.
    int comprobacion=comienzo();//llamamaos a la funcion que hace la parte inicial del programa
    if(comprobacion)
    {
        puts("El programa se ejecuto correctamente");
        return 1;
    }
    puts("hubo un error");
    return 0;
}//comprobamos si todo lo demas se ejecuto correctamente o no y terminamos el programa.

//funcion que simplemente busca inicializar todo para poder empezar a trabajar.
int comienzo()
{
    struct elista* inicio=NULL;
    struct cola cola;
    struct pila pila;
    int n, comprobacion=1;
    inicializar_cola(&cola);
    inicializar_pila(&pila);
    puts("Introduce el numero de hilos con los que quieres que trabaje el procesador");//el usuario decide cuantos hilos tendra el procesador.
    n=comprobar_entrada();//comprobar entradas para evitar que el usuario rompa el programa con un input erroneo.
    crea_pool(&inicio, n);
    puts("Visualizacion de la threadpool con la que se trabajara");//imprimimos la lista para que el usuario pueda ver que se crearon los hilos que el decidio
    imprime_lista(inicio);
    pausar();
    limpiar();
    comprobacion=procesador_de_tareas(&inicio, &cola, &pila, n);//llamamos a la funcion que lleva la mayoria del proceso.
    if(!comprobacion)
        return 0;
    return 1;
}
int comprobar_entrada()//funcion que encontre en w3schools para comprobar que un numero sea un entero
{
    int n=0;
    char entrada[100];
    while (fgets(entrada, sizeof(entrada), stdin)) {
    if (sscanf(entrada, "%d", &n) == 1 && n>0) {
      break; // Success: break out of the loop
    } else {
      printf("Entrada invalida, por favor ingresa un numero entero positivo: "); 
    }
  }
  return n;
}

/*Lleva la logica de que el programa termina hasta que se acaben los procesos de la cola y que se terminen de 
procesar en los hilos
*/
int procesador_de_tareas(struct elista** inicio, struct cola* cola, struct pila* pila, int n)
{
    if(n==0 || !inicio)//comprobaciones por robustez.
        return 0;
    int np, comprobacion=1; //np= numero de veces que se generan procesos.
    getchar();
    puts("Cuantas llegadas de procesos quieres que el sistema haga (la cantidad de procesos por llegada es aleatoria) ");
    np=comprobar_entrada();//el usuario decide cuantas veces generamos procesos y comprobamos su entrada.
    while(np>0  || !cola_vacia(cola) || hilos_ocupados(inicio))//acabamos hasta que todos los procesos terminen.
    {
        limpiar();
        printf("                       ITERACION ACTUAL                          \n");
        puts("=====================================================================");
        if(np>0)
        {//llamamos encolar procesos las veces que diga el usuario.
            encola_procesos_random(cola);
            np--;
        }
        puts("imprimimos la cola");
        imprimir_cola(cola->primero);//imprimimos la cola para ver que procesos quedan en espera.
        procesoa_hilo(inicio, cola);//llamamos proceso a hilo ir mandando los procesos de la cola para los hilos de la lista.
        comprobacion=hilos_trabajando(inicio, pila, n);//llamamos a la funcion que libera dinamicamente los hilos como si ya hubieran acabado el proceso.
        if(!comprobacion)//si algo salio mal en hilos trabajando, salimos para evitar que el programa truene.
            return 0;
        genera_error(pila, cola);//llamamos a la funcion que genera errores dinamicamente.
        puts("=====================================================================");
        puts("--- IMPRIMIENDO ESTADO DEL SISTEMA---");
        imprimir_sistema(inicio, cola, pila);//imprimimos todo.
        pausar();
        getchar();
    }
    generar_log(pila);
    liberar_cola(cola);
    libera_lista((*inicio));
    liberar_pila(pila);
    return 1;
}

//buscamos hilos libres para meter los procesos.
int procesoa_hilo(struct elista** inicio, struct cola* cola)
{
    if(cola_vacia(cola))//comprobacion para robustez.
    {
        puts("Cola vacia"); 
        return 1;
    }
    if(!hay_hilo_libre(inicio)) //Si no hay ningun hilo libre, tampoco podemos asignar ningun proceso. 
    {//esta comprobacion tambien la hace asignahilo, pero si la hacemos ahorita podemos ahorrarnos seguir esta funcion y llamar la otra.
        puts("Espacio insuficiente en la thread pool");//Tambien hacemos mas robusto el programa con multiples capas de comprobacion.
        return 0;
    }
    struct proceso* aux=elimina_ecola(cola);// conseguimos el proceso del nodo actual y lo eliminamos al mismo tiempo.
    if(aux)
    asigna_hilo(inicio, aux);//llamamos a la funcion que busca el hilo a asignar y lo asigna si tenemos un proceso para mandarle
    return procesoa_hilo(inicio, cola);
}

//Esta funcion hace la liberacion dinamica de ls hilos, para evitar que sea todo lineal.
int hilos_trabajando(struct elista ** lista, struct pila* pila, int n)
{
    if(n==0 || !lista) //si tenemos 0 hilos o si la lista no existe por alguna razon, retorna 0. robustez.
        return 0;
    int min=1, max=0, hilos;
    if(n!=1)
        max=n/2;
    else
        max=n;//En el caso de que el usuario quiera solo un hilo, evitamos dividir 1/2 y tratar de asignarlo a un entero. Mas robustez.
    struct proceso* proceso_aux;
    hilos=genera_numero(min, max);//generamos la cantidad de hilos a liberar en esta "vuelta" del programa aleatoriamente. 
    imprime_lista((*lista));
    for(int i=0; i<hilos; i++)//Podemos liberar desde 1 hasta n/2 hilos por vez
    {
        proceso_aux=libera_hilox(lista);//recibimos el proceso que manda la funcion liberahilo.
        if(proceso_aux)//comprobacion por robustez.
        inserta_epila(pila, proceso_aux);//lo mandamos a la pila.
    }
    return 1;
}

//llamamos las impresiones de las 3 estructuras.
int imprimir_sistema(struct elista** inicio, struct cola* cola, struct pila* pila)
{
    imprime_lista((*inicio));
    puts("cola de procesos");
    imprimir_cola(cola->primero);
    puts("imprimiendo procesos ya finalizados (log)");
    imprimir_pila(pila);
    return 1;
}

//funcion que simula errores dinamicamente
int genera_error(struct pila* pila, struct cola* cola)
{
    int n=genera_numero(1,8);//cada vez que la llamamos (1 vez por ciclo del procesador), hay 1/8 de probabilidad de que se genere un error
    if(n==1)
    {
        struct epila* aux= saca(pila);//sacamos el ultimo nodo de la pila
        if(!aux)//comprobacion por robustez.
            return 0;
        puts("Se ha generado un error. Reinicializando proceso");
        struct proceso* proceso_aux=aux->proceso;//conseguimos el apuntador del proceso dentro del nodo.
        free(aux);//liberamos el nodo.
        if(proceso_aux)//comprobacion por robustez
        {
            printf("El proceso (ID: %d, PRIO: %d) (FIN) volvera a ser encolado\n", proceso_aux->ID, proceso_aux->prioridad);//imprimimos que proceso se reincorpora
            inserta_ecola(cola, proceso_aux);//volvemos a insertar el proceso a la cola.
            return 1;
        }
    }
    return 0;
}

int limpiar()
{
    printf("\e[1;1H\e[2J");
    return 1;
}

int pausar()
{
    puts("pulsa una tecla para continuar");
    fflush(stdout);
    getchar();
    return 1;
}