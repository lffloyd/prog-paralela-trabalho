#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include "mpi.h"
#include "utils/matrix_generator.h"
#include "prim/sequential_prim.h"
#include "omp_prim/omp_prim.h"
#include "mpi_prim/mpi_prim.h"
#include "stats/stats.h"
#include "stats/csv.h"

#define NTRIALS 10
#define RESULTS_DIR "results"

struct stat st = {0};

int main(int argc, char *argv[])
{
    int rank, nprocesses; //For MPI implementation only

    int n_values[20];
    int nthreads; //For OMP implementation only
    int is_omp_version = 0;
    
    char *filename = (char *)malloc(50);

    char *version = argv[1];
    printf("Version choosen to be executed: %s\n", version);

    is_omp_version = !strcmp("OMP", version);

    for (int i = 2; i < (is_omp_version ? argc - 1 : argc); i++)
        n_values[i - 2] = atoi(argv[i]);

    int length = is_omp_version ? argc - 3 : argc - 2;
    printf("Matrix lenghts given: n = {");
    for (int i = 0; i < length; i++) {
        printf("%d ", n_values[i]);
    }
    printf("}\n");

    Table *csv_table = NULL;
    if (!strcmp("SEQ", version)) {
        printf("Executing sequential Prim implementation...\n");
        for (int i = 0; i < length; i++) {
            int n = n_values[i];
            int **mtx = generate_matrix(n);

            Table *line = create_simple_line(n, (double)INT_MAX);
            prim_minimum_spanning_tree(mtx, n, n, NTRIALS, line);
            csv_table = insert_line(line, csv_table);

            free_matrix(mtx, n);
        }
    } else if (is_omp_version) {
        printf("Executing OMP Prim implementation...\n");
        nthreads = atoi(argv[argc - 1]);
        printf("No. of threads given (OMP only): %d\n", nthreads);

        for (int i = 0; i < length; i++) {
            int n = n_values[i];
            int **mtx = generate_matrix(n);

            Table *line = create_simple_line(n, (double)INT_MAX);
            long minimum_cost = omp_prim_minimum_spanning_tree(mtx, n, n, nthreads, NTRIALS, line);
            //printf("%ld\n", minimum_cost);
            csv_table = insert_line(line, csv_table);

            free_matrix(mtx, n);
        }
    } else if (!strcmp("MPI", version)) {
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &nprocesses);
        if (rank==0) printf("Executing MPI Prim implementation...\n");
        
        for (int i =0; i<length; i++){
            int n = n_values[i];
            int **mtx = generate_matrix_test(n);
            Table *line = create_line(n, (double)INT_MAX, (double)INT_MAX);
            double* partial_time = mpi_prim_minimum_spanning_tree(mtx, n, n, rank, nprocesses, NTRIALS, line);

            //line->communication_time = partial_time[0]/NTRIALS;
            line->execution_time = partial_time[1];

            if (rank==0) printf("Tempo de execução: %f\n", line->execution_time);
            csv_table = insert_line(line, csv_table);

            free_matrix(mtx, n);
        }
        MPI_Finalize();
        
    }

    if (stat(RESULTS_DIR, &st) == -1 && ((!strcmp("MPI", version) && rank == 0) || strcmp("MPI", version))) {
        mkdir(RESULTS_DIR, 0700);
    }

    if (!strcmp("SEQ", version)) {
        sprintf(filename, "%s/%s_Prim.csv", RESULTS_DIR, version);
    } else {
        if ((!strcmp("MPI", version) && rank == 0) || strcmp("MPI", version)) {
            int number = is_omp_version ? nthreads : nprocesses;
            sprintf(filename, "%s/%s_Prim_%d.csv", RESULTS_DIR, version, number);
        }
    }
    
    if (!strcmp("MPI", version) && rank == 0) {
        printf("Saving %s...\n", filename);
        write_mpi_prim_csv(csv_table, filename);
        printf("%s saved\n", filename);
    } else if (strcmp("MPI", version)) {
        printf("Saving %s...\n", filename);
        write_csv(csv_table, filename);
        printf("%s saved\n", filename);
    }
    
    free(filename);
    if (csv_table != NULL)
        free_table(csv_table);
    
    return 0;
}