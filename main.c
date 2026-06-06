/**
 * @file main.c
 * @brief Función de arranque principal y manejo de flujos de control.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "utils.h"
#include "distributions.h"
#include "sorting.h"
#include "statistics.h"

void run_interactive(SimConfig* config);
void run_simulation(SimConfig* config);

int main(int argc, char* argv[]) {
    SimConfig config;
    // Configuración por defecto
    memset(&config, 0, sizeof(SimConfig));
    config.seed = 12345;
    config.interactive = 1;
    strcpy(config.sort_alg, "shell"); // Algoritmo robusto por defecto

    // Parser manual básico para simular control CLI [cite: 182]
    if (argc > 1) {
        config.interactive = 0;
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "--dist") == 0) strcpy(config.dist, argv[++i]);
            else if (strcmp(argv[i], "--mode") == 0) strcpy(config.mode, argv[++i]);
            else if (strcmp(argv[i], "--min") == 0) config.min = atof(argv[++i]);
            else if (strcmp(argv[i], "--max") == 0) config.max = atof(argv[++i]);
            else if (strcmp(argv[i], "--mu") == 0) config.mu = atof(argv[++i]);
            else if (strcmp(argv[i], "--var") == 0) config.var = atof(argv[++i]);
            else if (strcmp(argv[i], "-n") == 0) config.n = atoi(argv[++i]);
            else if (strcmp(argv[i], "--seed") == 0) config.seed = (unsigned int)atoi(argv[++i]);
            else if (strcmp(argv[i], "--sort") == 0) strcpy(config.sort_alg, argv[++i]);
        }
    }

    if (config.interactive) {
        run_interactive(&config);
    } else {
        run_simulation(&config);
    }

    return 0;
}

void run_interactive(SimConfig* config) {
    int opc_dist, opc_mode, opc_sort;
    printf("=== PRACTICA 2: ANALISIS ESTADISTICO ===\n");
    printf("Ingrese numero de muestras (N <= 100000): ");
    if (scanf("%d", &config->n) != 1) return;

    printf("\nSeleccione la distribucion:\n1. Uniforme\n2. Normal\n3. Laplace\nSeleccion: ");
    if (scanf("%d", &opc_dist) != 1) return;
    if (opc_dist == 1) strcpy(config->dist, "uniform");
    else if (opc_dist == 2) strcpy(config->dist, "normal");
    else strcpy(config->dist, "laplace");

    printf("\nSeleccione modalidad de ingreso de parametros:\n1. Por rango (min/max)\n2. Por parametros propios\nSeleccion: ");
    if (scanf("%d", &opc_mode) != 1) return;
    if (opc_mode == 1) strcpy(config->mode, "range");
    else strcpy(config->mode, "params");

    if (strcmp(config->mode, "range") == 0) {
        printf("Ingrese valor minimo: ");
        if (scanf("%le", &config->min) != 1) { /* No-op para manejo tipado */ };
        if (scanf("%lf", &config->min) != 1) return;
        printf("Ingrese valor maximo: ");
        if (scanf("%lf", &config->max) != 1) return;
    } else {
        printf("Ingrese Media (mu): ");
        if (scanf("%lf", &config->mu) != 1) return;
        printf("Ingrese Varianza (var): ");
        if (scanf("%lf", &config->var) != 1) return;
    }

    printf("Ingrese semilla del generador: ");
    if (scanf("%u", &config->seed) != 1) return;

    printf("\nSeleccione algoritmo de ordenamiento:\n");
    printf("1. Bubble\n2. Odd-Even\n3. Selection\n4. Insertion\n5. Cocktail\n6. Shell\n7. Bucket\n8. Counting\nSeleccion: ");
    if (scanf("%d", &opc_sort) != 1) return;
    
    char* algs[] = {"bubble", "oddeven", "selection", "insertion", "cocktail", "shell", "bucket", "counting"};
    if (opc_sort >= 1 && opc_sort <= 8) strcpy(config->sort_alg, algs[opc_sort - 1]);

    run_simulation(config);
}

void run_simulation(SimConfig* config) {
    srand(config->seed); // Fijar semilla para reproducibilidad [cite: 21, 188]

    double min_range = config->min;
    double max_range = config->max;
    double mu_gen = config->mu;
    double sigma_gen = 0.0;
    double b_gen = 0.0;

    // Procesar parametrizaciones cruzadas de acuerdo a indicaciones del documento [cite: 22, 139]
    if (strcmp(config->dist, "uniform") == 0) {
        if (strcmp(config->mode, "params") == 0) {
            min_range = config->mu - sqrt(3.0 * config->var); // [cite: 151]
            max_range = config->mu + sqrt(3.0 * config->var); // [cite: 151]
        }
    } else if (strcmp(config->dist, "normal") == 0) {
        if (strcmp(config->mode, "range") == 0) {
            mu_gen = (config->min + config->max) / 2.0; // [cite: 120, 160]
            sigma_gen = (config->max - config->min) / 6.0; // [cite: 120, 160]
        } else {
            mu_gen = config->mu;
            sigma_gen = sqrt(config->var); // [cite: 157]
            min_range = mu_gen - 3.0 * sigma_gen; // Acotamiento sugerido por la guia [cite: 178]
            max_range = mu_gen + 3.0 * sigma_gen; // [cite: 178]
        }
    } else if (strcmp(config->dist, "laplace") == 0) {
        if (strcmp(config->mode, "range") == 0) {
            mu_gen = (config->min + config->max) / 2.0; // [cite: 121, 171]
            b_gen = (config->max - config->min) / (6.0 * sqrt(2.0)); // [cite: 121, 172]
        } else {
            mu_gen = config->mu;
            b_gen = sqrt(config->var / 2.0); // [cite: 168]
            min_range = mu_gen - 3.0 * sqrt(2.0) * b_gen; // [cite: 178]
            max_range = mu_gen + 3.0 * sqrt(2.0) * b_gen; // [cite: 178]
        }
    }

    // Alojamiento dinámico de los vectores de muestras [cite: 15]
    double* X = (double*)malloc(config->n * sizeof(double));
    double* Y = (double*)malloc(config->n * sizeof(double));

    // Generar muestras según la función de densidad elegida
    if (strcmp(config->dist, "uniform") == 0) {
        gen_uniforme(X, config->n, min_range, max_range);
    } else if (strcmp(config->dist, "normal") == 0) {
        gen_normal(X, config->n, mu_gen, sigma_gen, min_range, max_range, 0); // Re-muestreo por defecto [cite: 177]
    } else if (strcmp(config->dist, "laplace") == 0) {
        gen_laplace(X, config->n, mu_gen, b_gen, min_range, max_range, 0);
    }

    // Clonar el arreglo original en Y para salvaguardar X inalterado
    memcpy(Y, X, config->n * sizeof(double));

    // Ejecutar ordenamiento midiendo el tiempo exacto de forma aislada [cite: 112, 219]
    double t_sort_start = get_time_sec();
    if (strcmp(config->sort_alg, "bubble") == 0) sort_bubble(Y, config->n);
    else if (strcmp(config->sort_alg, "oddeven") == 0) sort_odd_even(Y, config->n);
    else if (strcmp(config->sort_alg, "selection") == 0) sort_selection(Y, config->n);
    else if (strcmp(config->sort_alg, "insertion") == 0) sort_insertion(Y, config->n);
    else if (strcmp(config->sort_alg, "cocktail") == 0) sort_cocktail(Y, config->n);
    else if (strcmp(config->sort_alg, "shell") == 0) sort_shell(Y, config->n);
    else if (strcmp(config->sort_alg, "bucket") == 0) sort_bucket(Y, config->n, min_range, max_range);
    else if (strcmp(config->sort_alg, "counting") == 0) sort_counting(Y, config->n, min_range, max_range);
    double t_sort_end = get_time_sec();
    double total_sort_time = t_sort_end - t_sort_start;

    // Medición aislada de cálculos analíticos estadísticas [cite: 127]
    double t_calc_start = get_time_sec();
    double maximo = calc_maximo(Y, config->n);
    double minimo = calc_minimo(Y, config->n);
    double media_a = calc_media_aritmetica(X, config->n);
    double media_g = calc_media_geometrica(X, config->n);
    double mediana = calc_mediana(Y, config->n);
    double moda = calc_moda(Y, config->n);
    double varianza = calc_varianza(X, config->n, media_a);
    double sigma = calc_desviacion_std(varianza);
    double cv = calc_coef_variacion(sigma, media_a);
    double q1 = calc_cuartil(Y, config->n, 1);
    double d5 = calc_decil(Y, config->n, 5);
    double p95 = calc_percentil(Y, config->n, 95);
    double alpha_3 = calc_momento_no_centrado(X, config->n, 3);
    double mu_4 = calc_momento_centrado(X, config->n, media_a, 4);
    double curtosis = calc_curtosis(mu_4, sigma);
    double t_calc_end = get_time_sec();
    double total_calc_time = t_calc_end - t_calc_start;

    // Despliegue de Resultados Estructurados en Tabla única [cite: 123, 126]
    printf("\n+------------------------------------+------------------------------------+\n");
    printf("| MEDIDA ESTADISTICA                 | VALOR CALCULADO                    |\n");
    printf("+------------------------------------+------------------------------------+\n");
    printf("| 1. Maximo                          | %-34.6f |\n", maximo);
    printf("| 2. Minimo                          | %-34.6f |\n", minimo);
    printf("| 3. Media Aritmetica                | %-34.6f |\n", media_a);
    printf("| 4. Media Geometrica                | %-34.6f |\n", media_g);
    printf("| 5. Mediana                         | %-34.6f |\n", mediana);
    printf("| 6. Moda                            | %-34.6f |\n", moda);
    printf("| 7. Varianza                        | %-34.6f |\n", varianza);
    printf("| 8. Desviacion Estandar             | %-34.6f |\n", sigma);
    printf("| 9. Coeficiente de Variacion (CV)   | %-34.6f |\n", cv);
    printf("| 10. Cuartil 1 (Q1)                 | %-34.6f |\n", q1);
    printf("| 11. Decil 5 (D5)                   | %-34.6f |\n", d5);
    printf("| 12. Percentil 95 (P95)             | %-34.6f |\n", p95);
    printf("| 13. Momento No Centrado (Orden 3)  | %-34.6f |\n", alpha_3);
    printf("| 14. Momento Centrado (Orden 4)     | %-34.6f |\n", mu_4);
    printf("| 15. Curtosis                       | %-34.6f |\n", curtosis);
    printf("+------------------------------------+------------------------------------+\n");
    printf("| Tiempo de Ordenamiento (%-9s) | %-31.6f seg |\n", config->sort_alg, total_sort_time);
    printf("| Tiempo de Calculo de Medidas       | %-31.6f seg |\n", total_calc_time);
    printf("+------------------------------------+------------------------------------+\n\n");

    free(X);
    free(Y);
}