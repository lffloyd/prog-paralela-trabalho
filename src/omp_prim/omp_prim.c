#include "stdlib.h"
#include "string.h"
#include "limits.h"
#include "stdio.h"
#include "omp_prim.h"
#include "omp.h"


Edge *get_minimum_cost_edge(Edge *edges, int nedges) {
    Edge *best_edge = &edges[0];
    for (int i = 0; i < nedges; i++)
        if (edges[i].cost < best_edge->cost)
            best_edge = &edges[i];
    
    return best_edge;
}

int omp_prim_minimum_spanning_tree(int **cost, int rows, int columns, int nthreads) {
    omp_set_num_threads(nthreads);

    int *vertices_in_mst = (int *)malloc(rows * sizeof(int));
    memset(vertices_in_mst, 0, rows * sizeof(int));
  
    vertices_in_mst[0] = 1;
  
    int edge_count = 0, minimum_cost = 0; 
    while (edge_count < rows - 1) {

        Edge *edges = (Edge *)malloc(nthreads * sizeof(Edge));

        #pragma omp parallel shared(edges)
        {
            int id = omp_get_thread_num();
            int nthreads = omp_get_num_threads();

            int min = INT_MAX, a = -1, b = -1;
            
            for (int i = id; i < rows; i += nthreads) { 
                for (int j = id; j < columns; j += nthreads) {                
                    if (cost[i][j] < min) { 
                        if (is_valid_edge(i, j, vertices_in_mst)) { 
                            min = cost[i][j]; 
                            a = i; 
                            b = j; 
                        } 
                    } 
                } 
            }

            if (a != -1 && b != -1) { 
                printf("%d --- Edge %d:(%d, %d) cost: %d \n",  
                            id, edge_count++, a, b, min); 
                Edge *edge;
                edge->a = a;
                edge->b = b;
                edge->cost = min;
                edges[id] = *edge;
            }
        } 

        Edge *best_edge = get_minimum_cost_edge(edges, nthreads);
        printf("Best Edge %d:(%d, %d) cost: %d \n",  
                        edge_count++, best_edge->a, best_edge->b, best_edge->cost); 
        minimum_cost = minimum_cost + best_edge->cost; 
        vertices_in_mst[best_edge->b] = vertices_in_mst[best_edge->a] = 1;  
    }

    free(vertices_in_mst);

    return minimum_cost;
}