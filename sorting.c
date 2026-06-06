/**
 * @file sorting.c
 * @brief Implementación de las 8 lógicas de ordenamiento.
 */

#include "sorting.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

void sort_bubble(double* X, int n) {
    for (int i = 0; i < n - 1; i++) {
        int intercambiado = 0;
        for (int j = 0; j < n - i - 1; j++) {
            if (X[j] > X[j + 1]) {
                double temp = X[j];
                X[j] = X[j + 1];
                X[j + 1] = temp;
                intercambiado = 1;
            }
        }
        if (!intercambiado) break; // Optimización por bandera [cite: 128]
    }
}

void sort_odd_even(double* X, int n) {
    int ordenado = 0;
    while (!ordenado) {
        ordenado = 1;
        // Fase impar
        for (int i = 1; i < n - 1; i += 2) {
            if (X[i] > X[i + 1]) {
                double temp = X[i];
                X[i] = X[i + 1];
                X[i + 1] = temp;
                ordenado = 0;
            }
        }
        // Fase par
        for (int i = 0; i < n - 1; i += 2) {
            if (X[i] > X[i + 1]) {
                double temp = X[i];
                X[i] = X[i + 1];
                X[i + 1] = temp;
                ordenado = 0;
            }
        }
    }
}

void sort_selection(double* X, int n) {
    for (int i = 0; i < n - 1; i++) {
        int idx_min = i;
        for (int j = i + 1; j < n; j++) {
            if (X[j] < X[idx_min]) {
                idx_min = j;
            }
        }
        double temp = X[idx_min];
        X[idx_min] = X[i];
        X[i] = temp;
    }
}

void sort_insertion(double* X, int n) {
    for (int i = 1; i < n; i++) {
        double clave = X[i];
        int j = i - 1;
        while (j >= 0 && X[j] > clave) {
            X[j + 1] = X[j];
            j--;
        }
        X[j + 1] = clave; // [cite: 215]
    }
}

void sort_cocktail(double* X, int n) {
    int intercambiado = 1;
    int inicio = 0;
    int fin = n - 1;
    while (intercambiado) {
        intercambiado = 0;
        for (int i = inicio; i < fin; ++i) {
            if (X[i] > X[i + 1]) {
                double temp = X[i];
                X[i] = X[i + 1];
                X[i + 1] = temp;
                intercambiado = 1;
            }
        }
        if (!intercambiado) break;
        intercambiado = 0;
        fin--;
        for (int i = fin - 1; i >= inicio; --i) {
            if (X[i] > X[i + 1]) {
                double temp = X[i];
                X[i] = X[i + 1];
                X[i + 1] = temp;
                intercambiado = 1;
            }
        }
        inicio++;
    }
}

void sort_shell(double* X, int n) {
    for (int brecha = n / 2; brecha > 0; brecha /= 2) {
        for (int i = brecha; i < n; i++) {
            double temp = X[i];
            int j;
            for (j = i; j >= brecha && X[j - brecha] > temp; j -= brecha) {
                X[j] = X[j - brecha];
            }
            X[j] = temp;
        }
    } // [cite: 204]
}

void sort_bucket(double* X, int n, double min, double max) {
    int num_buckets = 100; // Configuración parametrizada estipulada por reporte [cite: 114, 115]
    if (max == min) max += 1e-5;
    
    // Lista enlazada simple para el balde
    typedef struct Node {
        double value;
        struct Node* next;
    } Node;

    Node** buckets = (Node**)calloc(num_buckets, sizeof(Node*));

    for (int i = 0; i < n; i++) {
        int bucket_idx = (int)((X[i] - min) / (max - min) * (num_buckets - 1));
        if (bucket_idx < 0) bucket_idx = 0;
        if (bucket_idx >= num_buckets) bucket_idx = num_buckets - 1;

        Node* new_node = (Node*)malloc(sizeof(Node));
        new_node->value = X[i];
        new_node->next = NULL;

        // Inserción ordenada en el balde
        if (!buckets[bucket_idx] || buckets[bucket_idx]->value >= X[i]) {
            new_node->next = buckets[bucket_idx];
            buckets[bucket_idx] = new_node;
        } else {
            Node* current = buckets[bucket_idx];
            while (current->next && current->next->value < X[i]) {
                current = current->next;
            }
            new_node->next = current->next;
            current->next = new_node;
        }
    }

    int idx = 0;
    for (int i = 0; i < num_buckets; i++) {
        Node* current = buckets[i];
        while (current) {
            X[idx++] = current->value;
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(buckets); // [cite: 205]
}

void sort_counting(double* X, int n, double min, double max) {
    // Mapeo por discretización de floats a bines enteros [cite: 217]
    int k = 10000; // Rango numérico finito controlado [cite: 113, 206]
    int* count = (int*)calloc(k, sizeof(int));
    double* output = (double*)malloc(n * sizeof(double));
    
    if (max == min) max += 1e-5;

    for (int i = 0; i < n; i++) {
        int bin = (int)((X[i] - min) / (max - min) * (k - 1));
        if (bin < 0) bin = 0;
        if (bin >= k) bin = k - 1;
        count[bin]++;
    }

    for (int i = 1; i < k; i++) {
        count[i] += count[i - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        int bin = (int)((X[i] - min) / (max - min) * (k - 1));
        if (bin < 0) bin = 0;
        if (bin >= k) bin = k - 1;
        output[count[bin] - 1] = X[i];
        count[bin]--;
    }

    memcpy(X, output, n * sizeof(double));
    free(count);
    free(output); // [cite: 206]
}