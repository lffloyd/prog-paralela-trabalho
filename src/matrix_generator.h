#ifndef MATRIX_GENERATOR_H_
#define MATRIX_GENERATOR_H_

int *fisher_yates_shuffle(int* arr, int length);
int *generate_random_weights(int n);
int **generate_matrix(int n);

#endif
