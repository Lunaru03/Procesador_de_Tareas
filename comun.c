#include <stdlib.h>
#include "comun.h"
//Encontre esta formulita para lograr que rand te de un numero aleatorio que se encuentre entre max y min
int genera_numero(int min, int max)
{
    int numero= (rand()% (max-min+1)+min);
    return numero;
}