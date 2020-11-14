#ifndef CSV_H_
#define CSV_H_

#include "stats.h"

int write_sequential_prim_csv(Table *table, char *filename);
int write_omp_prim_csv(Table *table, char *filename);
int write_mpi_prim_csv();

#endif
