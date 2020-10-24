#ifndef MATRIX_GENERATOR_H_
#define MATRIX_GENERATOR_H_

int *fisher_yates_shuffle(int *arr, int length);
int *generate_random_weights(int n);
int **generate_matrix(int n);
void show_matrix(int **mtx, int rows, int columns);
void free_matrix(int **mtx, int rows);

#endif
