#include "estadisticas.h"
#include <math.h>
#include <stdlib.h>

double calc_maximo(const double* Y, int n) {
    return Y[n - 1]; // Al estar ordenado ascendentemente, el último es el máximo
}

double calc_minimo(const double* Y, int n) {
    return Y[0]; // El primer elemento es el mínimo
}

double calc_media_aritmetica(const double* X, int n) {
    double suma = 0.0;
    for (int i = 0; i < n; i++) {
        suma += X[i];
    }
    return suma / n; 
}

double calc_media_geometrica(const double* X, int n) {
    double suma_logs = 0.0;
    for (int i = 0; i < n; i++) {
        if (X[i] <= 0) return 0.0; // Evitar indeterminación por logaritmo de <= 0
        suma_logs += log(X[i]);
    }
    return exp(suma_logs / n); // Equivalente robusto a la raíz n-ésima de la productoria 
}

double calc_mediana(const double* Y, int n) {
    if (n % 2 != 0) {
        return Y[(n + 1) / 2 - 1]; // Caso impar
    } else {
        return (Y[(n / 2) - 1] + Y[(n / 2)]) / 2.0; // Caso par
    }
}

double calc_moda(const double* Y, int n) {
    // Buscar la secuencia más larga de valores idénticos sobre el arreglo ordenado
    double moda = Y[0];
    int max_conteo = 1;
    int conteo_actual = 1;
    
    for (int i = 1; i < n; i++) {
        if (fabs(Y[i] - Y[i - 1]) < 1e-6) {
            conteo_actual++;
        } else {
            if (conteo_actual > max_conteo) {
                max_conteo = conteo_actual;
                moda = Y[i - 1];
            }
            conteo_actual = 1;
        }
    }
    if (conteo_actual > max_conteo) {
        moda = Y[n - 1];
    }
    return moda; 
}

double calc_varianza(const double* X, int n, double mu) {
    double suma = 0.0;
    for (int i = 0; i < n; i++) {
        suma += pow(X[i] - mu, 2);
    }
    return suma / n; 
}

double calc_desviacion_std(double varianza) {
    return sqrt(varianza); 
}

double calc_coef_variacion(double sigma, double mu) {
    if (fabs(mu) < 1e-9) return 0.0;
    return sigma / fabs(mu); 
}

double calc_cuartil(const double* Y, int n, int i) {
    int idx = (int)ceil((i * n) / 4.0) - 1;
    if (idx < 0) idx = 0;
    if (idx >= n) idx = n - 1;
    return Y[idx]; 
}

double calc_decil(const double* Y, int n, int i) {
    int idx = (int)ceil((i * n) / 10.0) - 1;
    if (idx < 0) idx = 0;
    if (idx >= n) idx = n - 1;
    return Y[idx];
}

double calc_percentil(const double* Y, int n, int i) {
    int idx = (int)ceil((i * n) / 100.0) - 1;
    if (idx < 0) idx = 0;
    if (idx >= n) idx = n - 1;
    return Y[idx]; 
}

double calc_momento_no_centrado(const double* X, int n, int orden) {
    double suma = 0.0;
    for (int i = 0; i < n; i++) {
        suma += pow(X[i], orden);
    }
    return suma / n;
}

double calc_momento_centrado(const double* X, int n, double mu, int orden) {
    double suma = 0.0;
    for (int i = 0; i < n; i++) {
        suma += pow(X[i] - mu, orden);
    }
    return suma / n;  
}

double calc_curtosis(double mu_4, double sigma) {
    double sigma_4 = pow(sigma, 4);
    if (sigma_4 < 1e-12) return 0.0;
    return mu_4 / sigma_4; 
}