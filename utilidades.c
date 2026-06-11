#include "utilidades.h"
#include <stdio.h>
#include <time.h>

/* Obtiene el tiempo actual del reloj del CPU en segundos. */
double get_time_sec(void) {
    return (double)clock() / CLOCKS_PER_SEC;
}