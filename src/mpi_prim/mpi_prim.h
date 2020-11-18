#ifndef MPI_PRIM_H_
#define MPI_PRIM_H_

#include "../utils/prim.h"
#include "../stats/stats.h"

void debug_print(int rank, int* key, int* parent, int* mstSet, int rows);
long total_cost(int V, int* key);
void update_neighbors(int** graph, int u, int* key, int* mstSet, int* parent, int index, int vertexPerProcess);
int* min_key(int *key, int* mstSet, int* parent, int index, int rows);
double* mpi_prim_minimum_spanning_tree(int **cost, int rows, int columns, int rank, int size, int ntrials, Table *line);

#endif
