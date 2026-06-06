/**
 * @file distributions.c
 * @brief Implementaciones matemáticas de transformaciones estadísticas.
 */

#include "distributions.h"
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double get_base_uniform(void) {
    return ((double)rand() + 1.0) / ((double)RAND_MAX + 2.0); // Previene indeterminaciones por ln(0) [cite: 137, 138]
}

void gen_uniforme(double* X, int n, double min, double max) {
    for (int i = 0; i < n; i++) {
        double u = get_base_uniform();
        X[i] = min + (max - min) * u; // [cite: 148]
    }
}

void gen_normal(double* X, int n, double mu, double sigma, double min, double max, int use_clipping) {
    for (int i = 0; i < n; i++) {
        double x;
        int rechazar = 1;
        while (rechazar) {
            double u1 = get_base_uniform();
            double u2 = get_base_uniform();
            double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2); // Box-Muller [cite: 155]
            x = mu + sigma * z; // [cite: 156]

            if (x >= min && x <= max) {
                rechazar = 0;
            } else if (use_clipping) { // Si está fuera de rango, aplicar recorte si es requerido [cite: 175, 176]
                if (x < min) x = min;
                if (x > max) x = max;
                rechazar = 0;
            } // Si no se activa clipping, repite el bucle (Remuestreo/Rechazo) [cite: 177]
        }
        X[i] = x;
    }
}

void gen_laplace(double* X, int n, double mu, double b, double min, double max, int use_clipping) {
    for (int i = 0; i < n; i++) {
        double x;
        int rechazar = 1;
        while (rechazar) {
            double u = get_base_uniform();
            double sgn = (u - 0.5 > 0) ? 1.0 : ((u - 0.5 < 0) ? -1.0 : 0.0);
            x = mu - b * sgn * log(1.0 - 2.0 * fabs(u - 0.5)); // Transformada inversa [cite: 165]

            if (x >= min && x <= max) {
                rechazar = 0;
            } else if (use_clipping) {
                if (x < min) x = min;
                if (x > max) x = max;
                rechazar = 0;
            }
        }
        X[i] = x;
    }
}