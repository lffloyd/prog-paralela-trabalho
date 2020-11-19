#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <limits.h>
#include "matrix_generator.h"

/*
Randomiza a posição de elementos numa lista de inteiros de acordo com o algoritmo Fisher-Yates (https://en.wikipedia.org/wiki/Fisher–Yates_shuffle).

Recebe:

   - arr: ponteiro para lista de inteiros a ser randomizada;
   - length: tamanho da lista

Retorna:

    - ponteiro para inicio da lista randomizada
*/
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

/*
Gera uma lista randomizada de pesos/custos a serem distribuidos na matriz de adjacencias.

Recebe:

   - n: numero de custos distintos a gerar

Retorna:

    - ponteiro para inicio da lista de custos randomizada
*/
int *generate_random_weights(int n)
{
    int *weights = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n - 1; i++)
        weights[i] = i;
    weights[n - 1] = INT_MAX;

    return fisher_yates_shuffle(weights, n);
}

/*
Gera de forma randomizada uma matriz de adjacencias de tamanho nxn

Recebe:

   - n: numero de nos do grafo ou linhas/colunas da matriz

Retorna:

    - ponteiro para inicio da matriz de adjacencias gerada
*/
int **generate_matrix(int n)
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

/*
Exibe uma matriz de adjacencias

Recebe:

   - mtz: ponteiro para inicio da matriz;
   - rows: num. de linhas;
   - columns: num. de colunas
*/
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

/*
Libera uma matriz de adjacencias

Recebe:

   - mtx: ponteiro para inicio da matriz;
   - rows: num. de linhas da matriz
*/
void free_matrix(int **mtx, int rows)
{
    for (int i = 0; i < rows; i++)
        free(mtx[i]);
    free(mtx);
}
