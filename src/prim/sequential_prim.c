#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <time.h>
#include "sequential_prim.h"


int prim_minimum_spanning_tree(int **cost, int rows, int columns, int ntrials, Table *line)
{
    double partial_time = 0.0;
    int minimum_cost;
    for (int i = 0; i < ntrials; i++)
    {
        clock_t begin = clock();

        int *vertices_in_mst = (int *)malloc(rows * sizeof(int));
        memset(vertices_in_mst, 0, rows * sizeof(int));

        vertices_in_mst[0] = 1;

        minimum_cost = 0;
        int edge_count = 0;
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
                printf("Selected edge %d:(%d, %d), cost: %d\n",
                       edge_count++, a, b, min);
                minimum_cost = minimum_cost + min;
                vertices_in_mst[b] = vertices_in_mst[a] = 1;
            }
        }

        free(vertices_in_mst);

        printf("MST cost: %d\n", minimum_cost);

        clock_t end = clock();
        partial_time += (double)(end - begin) / CLOCKS_PER_SEC;
    }

    line->execution_time = partial_time / ntrials;

    return minimum_cost;
}
