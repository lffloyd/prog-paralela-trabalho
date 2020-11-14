#include "sequential_prim.h"
#include "../utils/prim.h"
#include "stdlib.h"
#include "string.h"
#include "limits.h"
#include "stdio.h"

int prim_minimum_spanning_tree(int **cost, int rows, int columns)
{
    int *vertices_in_mst = (int *)malloc(rows * sizeof(int));
    memset(vertices_in_mst, 0, rows * sizeof(int));

    vertices_in_mst[0] = 1;

    int edge_count = 0, minimum_cost = 0;
    while (edge_count < rows - 1)
    {

        int min = INT_MAX, a = -1, b = -1;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                if (cost[i][j] < min)
                {
                    if (is_valid_edge(i, j, vertices_in_mst))
                    {
                        min = cost[i][j];
                        a = i;
                        b = j;
                    }
                }
            }
        }
        if (a != -1 && b != -1)
        {
            printf("Edge %d:(%d, %d) cost: %d \n",
                   edge_count++, a, b, min);
            minimum_cost = minimum_cost + min;
            vertices_in_mst[b] = vertices_in_mst[a] = 1;
        }
    }

    free(vertices_in_mst);

    return minimum_cost;
}
