#ifndef PRIM_H_
#define PRIM_H_

typedef struct Edge
{
    int a, b, cost;
    struct Edge *next;
} Edge;

int is_valid_edge(int u, int v, int *vertices_in_mst);
Edge *create_edge_node(int a, int b, int cost);
Edge *insert_node(Edge *edge, Edge *list);
void free_edge_list(Edge *list);

#endif
