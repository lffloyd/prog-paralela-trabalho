#include "sequential_prim.h"
#include "stdlib.h"
#include "string.h"
#include "limits.h"


int is_valid_edge(int u, int v, int *vertices_in_mst) { 
   if (u == v) 
       return 0; 
   if (vertices_in_mst[u] == 0 && vertices_in_mst[v] == 0) 
        return 0; 
   else if (vertices_in_mst[u] == 1 && vertices_in_mst[v] == 1) 
        return 0; 
   return 1; 
} 


int prim_minimum_spanning_tree(int **cost, int rows, int columns) {
    int *vertices_in_mst = (int *)malloc(rows * sizeof(int));
    memset(vertices_in_mst, 0, rows * sizeof(int));
  
    // Include first vertex in MST 
    vertices_in_mst[0] = 1; 
  
    int edge_count = 0, minimum_cost = 0; 
    while (edge_count < rows - 1) { 
  
        // Find minimum weight valid edge.   
        int min = INT_MAX, a = -1, b = -1; 
        for (int i = 0; i < rows; i++) { 
            for (int j = 0; j < columns; j++) {                
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
            printf("Edge %d:(%d, %d) cost: %d \n",  
                         edge_count++, a, b, min); 
            minimum_cost = minimum_cost + min; 
            vertices_in_mst[b] = vertices_in_mst[a] = 1; 
        } 
    }

    free(vertices_in_mst);

    return minimum_cost;
}
