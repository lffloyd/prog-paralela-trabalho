#include <limits.h>
#include <stdlib.h>
#include "prim.h"

/*
Indica se uma dada aresta (u, v) pode ser inserida na MST parcial. Para uma
aresta poder ser inserida, um de seus nos deve pertencer a MST e o outro nao

Recebe:

   - u: no origem da aresta;
   - v: no destino da aresta;
   - vertices_in_mst: arranjo de inteiros indicando os nos atualmente presentes na MST parcial;

Retorna:

    - 0 caso aresta nao possa ser inserida na MST, 1 caso contrario
*/
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


/*
Cria um no de aresta a ser posteriormente inserido na lista de arestas

Recebe:

   - a: no origem da aresta;
   - b: no destino da aresta;
   - cost: custo da aresta

Retorna:

    - ponteiro para a aresta criada
*/
Edge *create_edge_node(int a, int b, int cost)
{
     Edge *e = (Edge *)malloc(sizeof(Edge));
     e->a = a;
     e->b = b;
     e->cost = cost;
     e->next = NULL;
     return e;
}

/*
Insere uma aresta numa lista de arestas passada. Insercoes sao feitas no inicio da lista

Recebe:

   - edge: a aresta a ser inclusa;
   - list: a lista a ser modificada

Retorna:

    - ponteiro para inicio da lista atualizada
*/
Edge *insert_node(Edge *edge, Edge *list)
{
     edge->next = list;
     return edge;
}

/*
Libera lista de arestas

Recebe:

   - list: a lista a ser liberada

*/
void free_edge_list(Edge *list)
{    
     Edge *ptr = list;
     while (ptr != NULL) {
          Edge *temp = ptr->next;
          free(ptr);
          ptr = temp;
     }
}
