#ifndef priority_queue_H
#define priority_queue_H
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "comun.h"
struct ecola{
    struct proceso* elemento;
    struct ecola* sig;
};

struct cola{
    struct ecola* primero;
    struct ecola* ultimo;
};
struct ecola *crea_ecola(struct proceso* proceso);
int inserta_ecola(struct cola *cola, struct proceso *proceso);
struct ecola* buscar_prioridad(struct ecola* inicio);
struct proceso* elimina_ecola(struct cola *cola);
int cola_procesos_trabajando(struct cola *cola);
int encola_procesos_random(struct cola *cola);
struct proceso *genera_proceso(struct cola *cola);
int genera_id();
int genera_numero(int min, int max);
int inicializar_cola(struct cola *cola);
int imprimir_cola(struct ecola *aux);
int cola_vacia(struct cola* cola);
int liberar_cola(struct cola* cola);
#endif