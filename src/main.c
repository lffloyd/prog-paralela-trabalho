#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include "utils/matrix_generator.h"
#include "prim/sequential_prim.h"
#include "omp_prim/omp_prim.h"
#include "stats/stats.h"
#include "stats/csv.h"

#define NTRIALS 10
#define RESULTS_DIR "results"

struct stat st = {0};

int main(int argc, char *argv[])
{
    int n_values[10];
    int nthreads;
    int size = argc - 2;
    char *filename = (char *)malloc(50);

    nthreads = atoi(argv[1]);
    printf("No. of threads given: %d\n", nthreads);

    for (int i = 2; i < argc; i++)
        n_values[i - 2] = atoi(argv[i]);

    printf("Matrix lenghts given: n = {");
    for (int i = 0; i < size; i++) {
        printf("%d ", n_values[i]);
    }
    printf("}\n");

    Table *sequential_table = NULL;
    Table *omp_table = NULL;
    for (int i = 0; i < size; i++) {
        int n = n_values[i];
        int **mtx = generate_matrix(n);

        Table *sequential_line = create_simple_line(n, (double)INT_MAX);
        prim_minimum_spanning_tree(mtx, n, n, NTRIALS, sequential_line);
        sequential_table = insert_line(sequential_line, sequential_table);

        Table *omp_line = create_simple_line(n, (double)INT_MAX);
        omp_prim_minimum_spanning_tree(mtx, n, n, nthreads, NTRIALS, omp_line);
        omp_table = insert_line(omp_line, omp_table);

        free_matrix(mtx, n);
    }

    if (stat(RESULTS_DIR, &st) == -1) {
        mkdir(RESULTS_DIR, 0700);
    }

    sprintf(filename, "%s/sequential_prim.csv", RESULTS_DIR);
    printf("Saving %s...\n", filename);
    write_sequential_prim_csv(sequential_table, filename);

    sprintf(filename, "%s/omp_prim_nthreads%d.csv", RESULTS_DIR, nthreads);
    printf("Saving %s...\n", filename);
    write_sequential_prim_csv(omp_table, filename);

    free(filename);
    
    return 0;
}