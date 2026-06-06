/**
 * @file utils.h
 * @brief Funciones auxiliares de sistema, manejo de tiempo y formateo de tablas.
 */

#ifndef UTILS_H
#define UTILS_H

#include <time.h>

typedef struct {
    char dist[20];       // "uniform", "normal", "laplace"
    char mode[20];       // "range", "params"
    double min;
    double max;
    double mu;
    double var;
    int n;
    unsigned int seed;
    char sort_alg[20];   // "bubble", "shell", etc.
    int interactive;     // 1 si es por menú, 0 si es por CLI
} SimConfig;

/**
 * @brief Obtiene el tiempo actual del reloj del CPU en segundos.
 * @return Tiempo en segundos de tipo double.
 */
double get_time_sec(void);

/**
 * @brief Limpia el búfer de entrada de la consola estándar.
 */
void clear_buffer(void);

#endif // UTILS_H