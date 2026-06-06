/**
 * @file sorting.h
 * @brief Declaración de algoritmos de ordenamiento in-place.
 */

#ifndef SORTING_H
#define SORTING_H

void sort_bubble(double* X, int n);
void sort_odd_even(double* X, int n);
void sort_selection(double* X, int n);
void sort_insertion(double* X, int n);
void sort_cocktail(double* X, int n);
void sort_shell(double* X, int n);
void sort_bucket(double* X, int n, double min, double max);
void sort_counting(double* X, int n, double min, double max);

#endif // SORTING_H