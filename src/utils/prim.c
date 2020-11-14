#include "prim.h"
#include "limits.h"
#include "stdlib.h"

int is_valid_edge(int u, int v, int *vertices_in_mst)
{
     if (u == v)
          return 0;
     if (vertices_in_mst[u] == 0 && vertices_in_mst[v] == 0)
          return 0;
     else if (vertices_in_mst[u] == 1 && vertices_in_mst[v] == 1)
          return 0;
     return 1;
}

Edge *create_edge_node(int a, int b, int cost)
{
     Edge *e = (Edge *)malloc(sizeof(Edge));
     e->a = a;
     e->b = b;
     e->cost = cost;
     e->next = NULL;
     return e;
}

Edge *insert_node(Edge *edge, Edge *list)
{
     edge->next = list;
     return edge;
}

void free_edge_list(Edge *list)
{    
     Edge *ptr = list;
     while (ptr != NULL) {
          Edge *temp = ptr->next;
          free(ptr);
          ptr = temp;
     }
}
