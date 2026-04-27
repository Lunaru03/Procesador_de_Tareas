#include <stdlib.h>
#include "comun.h"
int genera_numero(int min, int max)
{
    int numero= (rand()% (max-min+1)+min);
    return numero;
}