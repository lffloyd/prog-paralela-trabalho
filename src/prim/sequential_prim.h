#ifndef SEQUENTIAL_PRIM_H_
#define SEQUENTIAL_PRIM_H_

int is_valid_edge(int u, int v, int *vertices_in_mst);
int prim_minimum_spanning_tree(int **cost, int rows, int columns);

#endif
