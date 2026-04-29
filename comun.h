#ifndef comun_H
#define comun_H
/*
Para simular que la cola le manda procesos a los hilos y luego los hilos al terminarlos los mandan al log, decidi hacer un struct proceso que las
3 estructuras lo tuvieran en sus nodos.
*/
struct proceso{
    int ID;
    int prioridad;
};
//La funcion genera numero usa rand para lograr el dinamismo.
int genera_numero(int min, int max);
#endif