#ifndef PRIM_H_
#define PRIM_H_

typedef struct Edge
{
    int a, b, cost;
} Edge;

int is_valid_edge(int u, int v, int *vertices_in_mst);

#endif
