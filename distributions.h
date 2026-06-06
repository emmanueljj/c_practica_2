/**
 * @file distributions.h
 * @brief Funciones matemáticas transformadoras de variables aleatorias uniformes.
 */

#ifndef DISTRIBUTIONS_H
#define DISTRIBUTIONS_H

/**
 * @brief Genera un float uniforme estricto entre (0, 1) evitando 0 y 1.
 * @return Número pseudoaleatorio uniforme continuo.
 */
double get_base_uniform(void);

void gen_uniforme(double* X, int n, double min, double max);
void gen_normal(double* X, int n, double mu, double sigma, double min, double max, int use_clipping);
void gen_laplace(double* X, int n, double mu, double b, double min, double max, int use_clipping);

#endif // DISTRIBUTIONS_H