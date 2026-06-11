#include "distribuciones.h"
#include <stdlib.h>
#include <math.h>

#define M_PI 3.14159265358979323846


double obtener_base_uniforme(void) {
    return ((double)rand() + 1.0) / ((double)RAND_MAX + 2.0); // previene indeterminaciones por ln(0) 
}

void gen_uniforme(double* X, int n, double min, double max) {
    for (int i = 0; i < n; i++) {
        double u = obtener_base_uniforme();
        X[i] = min + (max - min) * u; 
    }
}


void gen_normal(double* X, int n, double mu, double sigma, double min, double max, int recorte) {
    for (int i = 0; i < n; i++) {
        double x;
        int rechazar = 1;
        while (rechazar) {
            double u1 = obtener_base_uniforme();
            double u2 = obtener_base_uniforme();
            double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2); // Box-Muller 
            x = mu + sigma * z; 

            if (x >= min && x <= max) {
                rechazar = 0;
            } else if (recorte) { // Si está fuera de rango, aplicar recorte si es requerido 
                if (x < min) x = min;
                if (x > max) x = max;
                rechazar = 0;
            } // Si no se activa clipping, repite el bucle (Remuestreo/Rechazo)
        }
        X[i] = x;
    }
}

void gen_laplace(double* X, int n, double mu, double b, double min, double max, int recorte) {
    for (int i = 0; i < n; i++) {
        double x;
        int rechazar = 1;
        while (rechazar) {
            double u = obtener_base_uniforme();
            double sgn = (u - 0.5 > 0) ? 1.0 : ((u - 0.5 < 0) ? -1.0 : 0.0);
            x = mu - b * sgn * log(1.0 - 2.0 * fabs(u - 0.5)); // Transformada inversa 

            if (x >= min && x <= max) {
                rechazar = 0;
            } else if (recorte) {
                if (x < min) x = min;
                if (x > max) x = max;
                rechazar = 0;
            }
        }
        X[i] = x;
    }
}