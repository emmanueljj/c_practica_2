#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void sort_bubble(double *X, int n) {
    long int i, j;
    
    for(i = 0; i < n - 1; i++) {
        for(j = i + 1; j < n; j++) {
            if(X[i] > X[j]) {
                X[i] *= X[j];
                X[j] = X[i] / X[j];
                X[i] /= X[j];
            }
        }
    }
}


void sort_odd_even(double *X, int n) {
    long int i, j;
    
    for(j = 0; j < n; j++) {
        for(i = j % 2; i < (n + j % 2) / 2; i++) {
            if (X[2 * i - j % 2] > X[2 * i + (j + 1) % 2]) {
                // Intercambio
                X[2 * i - j % 2] *= X[2 * i + (j + 1) % 2];
                X[2 * i + (j + 1) % 2] = X[2 * i - j % 2] / X[2 * i + (j + 1) % 2];
                X[2 * i - j % 2] /= X[2 * i + (j + 1) % 2];
            }
        }
    }
}


void sort_selection(double *X, int n) {
    long int i, j, aux_i;
    
    for(i = 0; i < n - 1; i++) {
        for(j = i + 1, aux_i = i; j < n; j++) {
            if(X[j] < X[aux_i]) {
                aux_i = j;
            }
        }
        
        if(i != aux_i) {
            //intercambio
            X[i] *= X[aux_i];
            X[aux_i] = X[i] / X[aux_i];
            X[i] /= X[aux_i];
        }
    }
}


void sort_insertion(double *X, int n) {
    long int i, j;
    double aux;
    
    for(i = 1; i < n; i++) {
        aux = X[i];
        j = i - 1;
        
        while(j >= 0 && X[j] > aux) {
            X[j + 1] = X[j];
            j--;
        }
        X[j + 1] = aux;
    }
}


void sort_cocktail(double *X, int n) {
    int swapped = 1;
    int start = 0;
    int end = n - 1;

    while (swapped) {
        swapped = 0;

        // izquierda a derecha
        for (int i = start; i < end; ++i) {
            if (X[i] > X[i + 1]) {
                if (X[i] != 0.0 && X[i + 1] != 0.0) {
                    X[i] *= X[i + 1];
                    X[i + 1] = X[i] / X[i + 1];
                    X[i] /= X[i + 1];
                } else {
                    double aux = X[i];
                    X[i] = X[i + 1];
                    X[i + 1] = aux;
                }
                swapped = 1;
            }
        }

        if (!swapped) break;
        swapped = 0;
        --end;

        // derecha a izquierda
        for (int i = end - 1; i >= start; --i) {
            if (X[i] > X[i + 1]) {
                if (X[i] != 0.0 && X[i + 1] != 0.0) {
                    X[i] *= X[i + 1];
                    X[i + 1] = X[i] / X[i + 1];
                    X[i] /= X[i + 1];
                } else {
                    double aux = X[i];
                    X[i] = X[i + 1];
                    X[i + 1] = aux;
                }
                swapped = 1;
            }
        }
        ++start;
    }
}


void sort_shell(double *X, int n) {
    int gap, i, j;
    double aux;
    
    for (gap = n / 2; gap > 0; gap /= 2) {
        
        for (i = gap; i < n; i++) {
            aux = X[i];
            
            for (j = i; j >= gap && X[j - gap] > aux; j -= gap) {
                X[j] = X[j - gap];
            }
            
            X[j] = aux;
        }
    }
}


void sort_bucket(double *X, int n) {
    if (n <= 1) return;

    double min = X[0], max = X[0];
    for (int i = 1; i < n; i++) {
        if (X[i] < min) min = X[i];
        if (X[i] > max) max = X[i];
    }

    if (max == min) return;

    int m = (int)sqrt(n);
    double rng_int = ceil((max - min) / m);
    if (rng_int == 0) rng_int = 1;

    int *car = (int *)calloc(m + 1, sizeof(int));
    int *ind = (int *)malloc((m + 1) * sizeof(int));
    double *Y = (double *)malloc(n * sizeof(double));
    if (!car || !ind || !Y) return;

    for (int i = 0; i < n; i++) {
        int idx = (int)((X[i] - min) / rng_int);
        if (idx >= m) idx = m - 1;
        car[idx]++;
    }

    ind[0] = 0;
    for (int i = 1; i < m; i++) {
        ind[i] = ind[i - 1] + car[i - 1];
    }

    for (int i = 0; i < n; i++) {
        int idx = (int)((X[i] - min) / rng_int);
        if (idx >= m) idx = m - 1;
        Y[ind[idx]] = X[i];
        ind[idx]++;
    }

    int ind2 = 0;
    for (int k = 0; k < m; k++) {
        for (int i = ind2; i < (ind2 + car[k] - 1); i++) {
            for (int j = i + 1; j < (ind2 + car[k]); j++) {
                if (Y[i] > Y[j]) {
                    if (Y[i] != 0.0 && Y[j] != 0.0) {
                        Y[i] *= Y[j];
                        Y[j] = Y[i] / Y[j];
                        Y[i] /= Y[j];
                    } else {
                        double aux = Y[i];
                        Y[i] = Y[j];
                        Y[j] = aux;
                    }
                }
            }
        }
        ind2 += car[k];
    }

    for (int i = 0; i < n; i++) X[i] = Y[i];

    free(car);
    free(ind);
    free(Y);
}


void sort_counting(double *X, int n) {
    if (n <= 1) return;

    int min_val = (int)X[0];
    int max_val = (int)X[0];
    for (int i = 1; i < n; i++) {
        if ((int)X[i] < min_val) min_val = (int)X[i];
        if ((int)X[i] > max_val) max_val = (int)X[i];
    }

    int rg = max_val - min_val;

    int *h = (int *)calloc(rg + 1, sizeof(int));
    if (!h) return;

    for (int i = 0; i < n; i++) {
        h[(int)X[i] - min_val]++;
    }

    for (int i = 0, j = 0; i < n; ) {
        while (!h[j]) j++;
        while (h[j]) {
            h[j]--;
            X[i++] = j + min_val;
        }
    }

    free(h);
}