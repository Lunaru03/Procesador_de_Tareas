#ifndef log_H
#define log_H
#include "comun.h"
//cada nodo de pila tambien tiene el struct proceso para poder guardarlo y luego imprimirlo en un txt.
struct epila{
    struct proceso * proceso;
    struct epila * sig;
};

//encapsulamiento de los parametros de la pila.
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
int generar_log(struct pila* pila);
int rellenar_log(struct pila* pila, FILE* file);
int rellenar_aux(struct epila* aux, FILE* file);
int liberar_pila(struct pila* pila);
#endif