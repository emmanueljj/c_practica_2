/*librerias propias de c*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/*arhcivos de funciones declaradas*/
#include "utilidades.h"
#include "distribuciones.h"
#include "ordenamiento.h"
#include "estadisticas.h"

/*prototipos*/
void procesar(char distribucion[], char modo[], double val_minimo, double val_maximo, double val_mu, double val_var,
                 int num_datos, unsigned int semilla_val, char alg_ordenamiento[]);

int main(int argc, char* argv[]) {

    /* esta funcion tiene como propocito obtener los valores de las varibles por argumentos o por consola y ejecutar la funcion prosesar*/
    char distribucion[20] = "";
    char modo[20] = "";
    double val_minimo = 0.0;
    double val_maximo = 0.0;
    double val_mu = 0.0;
    double val_var = 0.0;
    int num_datos = 0;
    unsigned int semilla_val = 12345;
    char alg_ordenamiento[20] = "shell"; 
    int interactivo = 1;        //funciona como bandera para usar o no los argumentos (si los hay)

    int num_parametros = 8;

    // 2. Parser manual (Argumentos de consola)
    if (argc >= num_parametros) {

        interactivo = 0;

        for (int i = 1; i < num_parametros; i++) {
            
            if (i == 1) strcpy(distribucion, argv[i]);
            else if (i == 2) strcpy(modo, argv[i]);
            else if (i == 3) {
                if (strcmp(modo, "params") == 0) val_mu = atof(argv[i]);
                else val_minimo = atof(argv[i]);
            }
            else if (i == 4) {
                if (strcmp(modo, "params") == 0) val_var = atof(argv[i]);
                else val_maximo = atof(argv[i]);
            }
            else if (i == 5) num_datos = atoi(argv[i]);

            else if (i == 6) semilla_val = (unsigned int)atoi(argv[i]);

            else if (i == 7) strcpy(alg_ordenamiento, argv[i]);
        }
    }

    // 3. Menú interactivo integrado directamente en main()
    if (interactivo) {
        int opc_distribucion, opc_modo, opc_ordenamiento;

        printf("=== PRACTICA 2: ANALISIS ESTADISTICO ===\n");
        printf("Ingrese numero de muestras (N <= 100000): ");

        if (scanf("%d", &num_datos) != 1) return 1;  //devuelve 1 en caso de ser 0

        printf("\nSeleccione la distribucion:\n1. Uniforme\n2. Normal\n3. Laplace\nSeleccion: ");

        if (scanf("%d", &opc_distribucion) != 1) return 1;
        if (opc_distribucion == 1) strcpy(distribucion, "uniform");
        else if (opc_distribucion == 2) strcpy(distribucion, "normal");
        else strcpy(distribucion, "laplace");

        printf("\nSeleccione modalidad de ingreso de parametros:\n1. Por rango (min/max)\n2. Por parametros propios\nSeleccion: ");
        
        if (scanf("%d", &opc_modo) != 1) return 1;
        if (opc_modo == 1) strcpy(modo, "range");
        else strcpy(modo, "params");

        if (strcmp(modo, "range") == 0) {

            printf("Ingrese valor minimo: ");
            if (scanf("%lf", &val_minimo) != 1) return 1;

            printf("Ingrese valor maximo: ");
            if (scanf("%lf", &val_maximo) != 1) return 1;
        } else {
            printf("Ingrese Media (mu): ");
            if (scanf("%lf", &val_mu) != 1) return 1;

            printf("Ingrese Varianza (var): ");
            if (scanf("%lf", &val_var) != 1) return 1;
        }

        printf("Ingrese semilla del generador: ");
        if (scanf("%u", &semilla_val) != 1) return 1;

        printf("\nSeleccione algoritmo de ordenamiento:\n");
        printf("1. Bubble\n2. Odd-Even\n3. Selection\n4. Insertion\n5. Cocktail\n6. Shell\n7. Bucket\n8. Counting\nSeleccion: ");
        if (scanf("%d", &opc_ordenamiento) != 1) return 1;
        
        char* algoritmos[] = {"bubble", "oddeven", "selection", "insertion", "cocktail", "shell", "bucket", "counting"};
        if (opc_ordenamiento >= 1 && opc_ordenamiento <= 8) strcpy(alg_ordenamiento, algoritmos[opc_ordenamiento - 1]);       // -1 por el indice 0
    }

    //llamamos a funcion procesar
    procesar(distribucion, modo, val_minimo, val_maximo, val_mu, val_var, num_datos, semilla_val, alg_ordenamiento);

    return 0;
}

void procesar(char distribucion[], char modo[], double val_minimo, double val_maximo, double val_mu, double val_var, int num_datos, unsigned int semilla_val, char alg_ordenamiento[]) {

    srand(semilla_val); 

    double rango_minimo = val_minimo;
    double rango_maximo = val_maximo;
    double gen_mu = val_mu;
    double gen_sigma = 0.0;
    double gen_b = 0.0;

    // Procesar parametrizaciones cruzadas
    if (strcmp(distribucion, "uniform") == 0) {

        if (strcmp(modo, "params") == 0) {

            rango_minimo = val_mu - sqrt(3.0 * val_var); 
            rango_maximo = val_mu + sqrt(3.0 * val_var); 
        }
    } else if (strcmp(distribucion, "normal") == 0) {

        if (strcmp(modo, "range") == 0) {

            gen_mu = (val_minimo + val_maximo) / 2.0; 
            gen_sigma = (val_maximo - val_minimo) / 6.0; 
        } else {

            gen_mu = val_mu;
            gen_sigma = sqrt(val_var); 
            rango_minimo = gen_mu - 3.0 * gen_sigma; 
            rango_maximo = gen_mu + 3.0 * gen_sigma; 
        }
    } else if (strcmp(distribucion, "laplace") == 0) {

        if (strcmp(modo, "range") == 0) {
            gen_mu = (val_minimo + val_maximo) / 2.0; 
            gen_b = (val_maximo - val_minimo) / (6.0 * sqrt(2.0)); 
        } else {

            gen_mu = val_mu;
            gen_b = sqrt(val_var / 2.0); 
            rango_minimo = gen_mu - 3.0 * sqrt(2.0) * gen_b; 
            rango_maximo = gen_mu + 3.0 * sqrt(2.0) * gen_b; 
        }
    }

    double* arreglo_x = (double*)malloc(num_datos * sizeof(double));
    double* arreglo_y = (double*)malloc(num_datos * sizeof(double));

    /*GENERA EL ARREGLO SEGUN LA DISTRIBUCION*/

    if (strcmp(distribucion, "uniform") == 0) {

        gen_uniforme(arreglo_x, num_datos, rango_minimo, rango_maximo);
    } else if (strcmp(distribucion, "normal") == 0) {

        gen_normal(arreglo_x, num_datos, gen_mu, gen_sigma, rango_minimo, rango_maximo, 0); 
    } else if (strcmp(distribucion, "laplace") == 0) {

        gen_laplace(arreglo_x, num_datos, gen_mu, gen_b, rango_minimo, rango_maximo, 0);
    }

    memcpy(arreglo_y, arreglo_x, num_datos * sizeof(double));

    /*LLAMA A LAS FUNCIONES DE ORDENAMIENTO */
    double t_inicio_ordenamiento = get_time_sec();       //hora en que inicio a ordenar
    if (strcmp(alg_ordenamiento, "bubble") == 0) sort_bubble(arreglo_y, num_datos);
    else if (strcmp(alg_ordenamiento, "oddeven") == 0) sort_odd_even(arreglo_y, num_datos);
    else if (strcmp(alg_ordenamiento, "selection") == 0) sort_selection(arreglo_y, num_datos);
    else if (strcmp(alg_ordenamiento, "insertion") == 0) sort_insertion(arreglo_y, num_datos);
    else if (strcmp(alg_ordenamiento, "cocktail") == 0) sort_cocktail(arreglo_y, num_datos);
    else if (strcmp(alg_ordenamiento, "shell") == 0) sort_shell(arreglo_y, num_datos);
    else if (strcmp(alg_ordenamiento, "bucket") == 0) sort_bucket(arreglo_y, num_datos, rango_minimo, rango_maximo);
    else if (strcmp(alg_ordenamiento, "counting") == 0) sort_counting(arreglo_y, num_datos, rango_minimo, rango_maximo);
    double t_fin_ordenamiento = get_time_sec(); //tiempo en el que termino de ordenar
    double tiempo_total_ordenamiento = t_fin_ordenamiento - t_inicio_ordenamiento; //tiempo qu tardo ordenando

    //calcula las estadisitcas llamnndo funciones
    double t_inicio_calculo = get_time_sec();
    
    // Medidas base (se calculan una sola vez)
    double maximo = calc_maximo(arreglo_y, num_datos);
    double minimo = calc_minimo(arreglo_y, num_datos);
    double media_aritmetica = calc_media_aritmetica(arreglo_x, num_datos);
    double media_geometrica = calc_media_geometrica(arreglo_x, num_datos);
    double mediana = calc_mediana(arreglo_y, num_datos);
    double moda = calc_moda(arreglo_y, num_datos);
    double varianza = calc_varianza(arreglo_x, num_datos, media_aritmetica);
    double desviacion_estandar = calc_desviacion_std(varianza);
    double coef_variacion = calc_coef_variacion(desviacion_estandar, media_aritmetica);
    
    // El 4to momento centrado y desviación estándar se necesitan por adelantado para la curtosis
    double momento_centrado_4 = calc_momento_centrado(arreglo_x, num_datos, media_aritmetica, 4);
    double curtosis = calc_curtosis(momento_centrado_4, desviacion_estandar);
    double momento_centrado_3 = calc_momento_centrado(arreglo_x, num_datos, media_aritmetica, 3);
    double asimetria = 0.0;
    if (pow(desviacion_estandar, 3) >= 1e-12) {
        asimetria = momento_centrado_3 / pow(desviacion_estandar, 3);
    }

    double t_fin_calculo = get_time_sec();
    double tiempo_total_calculo = t_fin_calculo - t_inicio_calculo;
    
    printf("\nMEDIDAS ESTADISTICAS\n");
    printf("______________________________________________________________________\n");
    printf("Maximo: %.6f\n", maximo);
    printf("Minimo: %.6f\n", minimo);
    printf("Media Aritmetica: %.6f\n", media_aritmetica);
    printf("Media Geometrica: %.6f\n", media_geometrica);
    printf("Mediana: %.6f\n", mediana);
    printf("Moda: %.6f\n", moda);
    printf("Varianza: %.6f\n", varianza);
    printf("Desviacion Estandar: %.6f\n", desviacion_estandar);
    printf("Coeficiente de Variacion (CV): %.6f\n", coef_variacion);
    printf("Curtosis: %.6f\n", curtosis);
    
    printf("\nMOMENTOS ESTADISTICOS\n");
    printf("______________________________________________________________________\n");
    for(int i = 1; i <= 4; i++) {
        printf("Momento NO Centrado (Orden %d): %.6f\n", i, calc_momento_no_centrado(arreglo_x, num_datos, i));
    }
    for(int i = 1; i <= 4; i++) {
        printf("Momento Centrado (Orden %d): %.6f\n", i, calc_momento_centrado(arreglo_x, num_datos, media_aritmetica, i));
    }
    printf("Momento Estandar 1: %.6f\n", 0.0);
    printf("Momento Estandar 2: %.6f\n", 1.0);
    printf("Momento Estandar 3 (Asimetria): %.6f\n", asimetria);
    printf("Momento Estandar 4 (Curtosis): %.6f\n", curtosis);

    printf("\nCUARTILES (Q1 - Q3)\n");
    printf("______________________________________________________________________\n");
    for(int i = 1; i <= 3; i++) {
        printf("Cuartil %d (Q%d): %.6f\n", i, i, calc_cuartil(arreglo_y, num_datos, i));
    }

    printf("\nDECILES (D1 - D9)\n");
    printf("______________________________________________________________________\n");
    for(int i = 1; i <= 9; i++) {
        printf("Decil %d (D%d): %.6f\n", i, i, calc_decil(arreglo_y, num_datos, i));
    }

    printf("\nPERCENTILES (P1 - P99)\n");
    printf("______________________________________________________________________\n");
    for(int i = 1; i <= 99; i++) {
        printf("Percentil %d (P%d): %.6f\n", i, i, calc_percentil(arreglo_y, num_datos, i));
    }

    printf("\nRENDIMIENTO (TIEMPOS)\n");
    printf("______________________________________________________________________\n");
    printf("Tiempo de Ordenamiento (%s): %.6f seg\n", alg_ordenamiento, tiempo_total_ordenamiento);
    printf("Tiempo de Calculo de Medidas: %.6f seg\n\n", tiempo_total_calculo);

    free(arreglo_x);
    free(arreglo_y);
    
}