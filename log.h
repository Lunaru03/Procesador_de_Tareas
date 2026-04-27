#ifndef log_H
#define log_H
#include "comun.h"
struct epila{
    struct proceso * proceso;
    struct epila * sig;
};

struct pila{
    struct epila * tope;
    int n_elementos;
};
int inicializar_pila(struct pila *pila);
int push (struct pila *pila, struct epila *elemento );
struct epila *pop(struct pila* pila);
struct epila * crea_epila(struct proceso* proceso);
int inserta_epila(struct pila* pila, struct proceso* proceso);
struct epila *saca(struct pila* pila);
int imprimir_pila(struct pila* pila);
#endif