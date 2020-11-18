#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <omp.h>
#include "omp_prim.h"

/*
Obtem a aresta de menor custo dentro da lista de arestas candidatas passadas.

Recebe:

   - edges: lista de arestas candidatas, cada aresta foi encontrada por um thread em sua area de procura

Retorna:

    - aresta de menor custo encontrada na lista ou NULL caso a lista seja vazia
*/
Edge *get_minimum_cost_edge(Edge *edges)
{
    if (!edges)
        return NULL;

    if (!edges->next)
        return edges;

    Edge *best_edge = edges;
    for (Edge *ptr = edges->next; ptr != NULL; ptr = ptr->next)
        if (ptr->cost < best_edge->cost)
            best_edge = ptr;

    return best_edge;
}

/*
Versao paralela em OpenMP do algoritmo de Prim.

Recebe:

   - cost: matriz de adjacencias contendo os custos de cada aresta;
   - rows: total de linhas;
   - columns: total de colunas;
   - nthreads: numero de threads de execucao a utilizar;
   - ntrials: numero de repeticoes do cenario, para calculo das medias de tempo de execucao;
   - line: linha da tabela de resultados a ser preenchida com o tempo de execucao da funcao

Retorna:

    - custo minimo da MST encontrada pelo algoritmo
*/
long omp_prim_minimum_spanning_tree(int **cost, int rows, int columns, int nthreads, int ntrials, Table *line)
{
    double partial_time = 0.0;
    long minimum_cost;

    for (int i = 0; i < ntrials; i++)
    {
        partial_time -= omp_get_wtime();

        omp_set_num_threads(nthreads);

        int *vertices_in_mst = (int *)malloc(rows * sizeof(int));
        memset(vertices_in_mst, 0, rows * sizeof(int));

        vertices_in_mst[0] = 1;

        minimum_cost = 0;
        int edge_count = 0;
        while (edge_count < rows - 1)
        {
            Edge *edges = NULL;

#pragma omp parallel shared(edges, cost, rows, columns, edge_count, vertices_in_mst)
            {
                int id = omp_get_thread_num();
                int nthreads = omp_get_num_threads();

                int min = INT_MAX, a = -1, b = -1;

#pragma omp parallel for
                for (int i = 0; i < rows; i++)
                {
#pragma omp parallel for
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

                if (a != -1 && b != -1 && min != INT_MAX)
                {
                    Edge *edge = create_edge_node(a, b, min);
#pragma omp critical
                    edges = insert_node(edge, edges);
                }
            }

            if (edges != NULL)
            {
                Edge *best_edge = get_minimum_cost_edge(edges);

                printf("Selected edge %d:(%d, %d), cost: %d\n", edge_count, best_edge->a, best_edge->b, best_edge->cost);

                minimum_cost = minimum_cost + best_edge->cost;
                vertices_in_mst[best_edge->b] = vertices_in_mst[best_edge->a] = 1;
                edge_count++;

                free_edge_list(edges);
            }
        }

        printf("MST cost: %ld\n", minimum_cost);

        free(vertices_in_mst);

        partial_time += omp_get_wtime();
    }

    line->execution_time = partial_time / ntrials;

    return minimum_cost;
}