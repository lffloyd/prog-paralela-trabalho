#include "stdlib.h"
#include "stdio.h"
#include "csv.h"

int write_sequential_prim_csv(Table *table, char *filename) {
    FILE *fp;

    if ((fp = fopen(filename, "w")) == NULL)
    {
        printf("Cannot open file.\n");
        return 1;
    }

    fprintf(fp, "n,Tempo de Execução\n");

    Table *ptr = table;
    while (ptr != NULL) {
        fprintf(fp, "%d,%g\n", ptr->n, ptr->execution_time);
        ptr = ptr->next;
    }

    fclose(fp);

    return 0;
}

int write_omp_prim_csv(Table *table, char *filename) {
    return 0;
}

int write_mpi_prim_csv() {
    return 0;
}
