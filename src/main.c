#include "utils/matrix_generator.h"
#include "stdio.h"

int main()
{
    int n = 10;
    int **mtx = generate_matrix(n);
    show_matrix(mtx, n, n);
    free_matrix(mtx, n);
    return 0;
}