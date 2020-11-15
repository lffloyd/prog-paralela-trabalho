#ifndef SEQUENTIAL_PRIM_H_
#define SEQUENTIAL_PRIM_H_

#include "../utils/prim.h"
#include "../stats/stats.h"

long prim_minimum_spanning_tree(int **cost, int rows, int columns, int ntrials, Table *line);

#endif
