#ifndef PRIM_H_
#define PRIM_H_

/*
Estrutura que define uma representacao de lista de arestas (ou um no unico).

Campos:

   - a: origem da aresta;
   - b: destino da aresta;
   - cost: custo da aresta;
   - next: ponteiro para proxima aresta na lista (se existir)
*/
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
