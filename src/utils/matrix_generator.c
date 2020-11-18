#include "matrix_generator.h"
#include "string.h"
#include "stdlib.h"
#include "time.h"
#include "stdio.h"
#include "limits.h"

int *fisher_yates_shuffle(int *arr, int length)
{
    srand(time(NULL));

    for (int i = length - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    return arr;
}

int *generate_random_weights(int n)
{
    int *weights = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n - 1; i++)
        weights[i] = i;
    weights[n - 1] = INT_MAX;

    return fisher_yates_shuffle(weights, n);
}

int **generate_matrix(int n) // acho que vou trocr. Só chama na main e nos tests/utils
{
    int *weights = generate_random_weights((n * n) - n);

    int **matrix = (int **)malloc(sizeof(int *) * n);
    for (int i = 0; i < n; i++)
    {
        matrix[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++)
            matrix[i][j] = INT_MAX;
    }

    int idx = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i != j)
            {
                matrix[i][j] = weights[idx];
                idx++;
            }
        }
    }

    free(weights);

    return matrix;
}

int **generate_matrix_test(int n){

    int **matrix = (int**)malloc(sizeof(int*) * n);
    for (int i=0; i<n; i++){
        matrix[i] = (int*)malloc (n*sizeof(int));
    }

    int idx = 1;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(i == j){
                matrix[i][j] = 0;
            }
            else if (i > j){
                matrix[i][j] = idx;
                matrix[j][i] = matrix[i][j];
            }
            idx++;
        }
    }

    return matrix;
}

void show_matrix(int **mtx, int rows, int columns)
{
    printf("\n");

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            printf("%d\t", mtx[i][j]);
        }

        printf("\n");
    }
    return;
}

void free_matrix(int **mtx, int rows)
{
    for (int i = 0; i < rows; i++)
        free(mtx[i]);
    free(mtx);
}
