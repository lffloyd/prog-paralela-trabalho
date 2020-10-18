#include "matrix_generator.h"
#include "string.h"
#include "stdlib.h"

int *fisher_yates_shuffle(int* arr, int length) {
    srand(time(NULL));

    for (int i = length - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    return arr;
}


int *generate_random_weights(int n) {
    int *weights = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        weights[i] = i;

    return fisher_yates_shuffle(weights, n);
}


int **generate_matrix(int n) {
    if (n <= 0) return 1;

    int *weights = generate_random_weights(n);

    int **matrix = malloc(n * n * sizeof(int));
    memset(matrix, 0, n * n * sizeof(int));

    int idx = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                matrix[i][j] = weights[idx];
                idx++;
            }
        }
    }

    free(weights);
    return matrix;
}
