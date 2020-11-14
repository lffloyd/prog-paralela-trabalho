#ifndef OMP_PRIM_H_
#define OMP_PRIM_H_

#include "../utils/prim.h"
#include "../stats/stats.h"

Edge *get_minimum_cost_edge(Edge *edges, int nedges);
int omp_prim_minimum_spanning_tree(int **cost, int rows, int columns, int nthreads, int ntrials, Table *line);

#endif
