#include "prim.h"


int is_valid_edge(int u, int v, int *vertices_in_mst) { 
   if (u == v) 
       return 0; 
   if (vertices_in_mst[u] == 0 && vertices_in_mst[v] == 0) 
        return 0; 
   else if (vertices_in_mst[u] == 1 && vertices_in_mst[v] == 1) 
        return 0; 
   return 1; 
} 
