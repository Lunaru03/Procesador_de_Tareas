#ifndef thread_pool_H
#define thread_pool_H
#include "comun.h"
struct hilo{
    int ID;
    int estado;
    struct proceso* tarea_actual;
};
struct elista{
    struct hilo* hilo;
    struct elista* sig;
};
int crea_pool(struct elista** inicio, int n);
struct hilo* crea_hilo();
int genera_ID();
struct elista * crea_elista(struct hilo* hilo);
int inserta_elista(struct elista ** inicio);
int imprime_lista(struct elista* inicio);
int asigna_hilo(struct elista** inicio, struct proceso* t_asig);
struct proceso *libera_hilox(struct elista** inicio, int ID);
int libera_lista(struct elista* inicio);
struct elista* hay_hilo_libre(struct elista** inicio);
int hilos_ocupados(struct elista** inicio);
#endif