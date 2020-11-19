#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <time.h>
#include "sequential_prim.h"

/*
Versao sequencial do algoritmo de Prim.

Recebe:

   - cost: matriz de adjacencias contendo os custos de cada aresta;
   - rows: total de linhas;
   - columns: total de colunas;
   - ntrials: numero de repeticoes do cenario, para calculo das medias de tempo de execucao;
   - line: linha da tabela de resultados a ser preenchida com o tempo de execucao da funcao

Retorna:

    - custo minimo da MST encontrada pelo algoritmo
*/
long prim_minimum_spanning_tree(int **cost, int rows, int columns, int ntrials, Table *line)
{
    double partial_time = 0.0; //inicializa tempo parcial de execucao
    long minimum_cost; //define custo da MST

    //Itera ntrials vezes, agregando o tempo parcial de execucao, para depois calcular sua media
    for (int i = 0; i < ntrials; i++)
    {
        //inicializa o tempo de inicio
        clock_t begin = clock();
        
        //inicializa a lista de vertices atualmente na MST sem nenhum elemento
        int *vertices_in_mst = (int *)malloc(rows * sizeof(int));
        memset(vertices_in_mst, 0, rows * sizeof(int));
        //insere o primeiro no da matriz na lista de nos na MST parcial
        vertices_in_mst[0] = 1;

        minimum_cost = 0; //inicializa o custo minimo atual
        int edge_count = 0; //inicializa o total de arestas inseridas
        while (edge_count < rows - 1)
        {
            //define as variaveis usadas para armazenar a aresta minima, quando for encontrada.
            //valores iniciais refletem a inexistencia de uma aresta minima no presente momento
            int min = INT_MAX, a = -1, b = -1;

            //itera sobre todas as linhas da matriz
            for (int i = 0; i < rows; i++)
            {
                //itera sobre todas as colunas da matriz
                for (int j = 0; j < columns; j++)
                {
                    //se o custo da aresta atual for menor que o custo minimo atual e se
                    //a aresta atual for uma aresta valida para insercao na MST parcial
                    if (cost[i][j] < min && is_valid_edge(i, j, vertices_in_mst))
                    {
                        //define a aresta atual como a de menor custo atual,
                        //atualizando seu custo, sua origem (a) e seu destino (b)
                        min = cost[i][j];
                        a = i;
                        b = j;
                    }
                }
            }

            //Se alguma aresta de custo minimo foi de fato encontrada...
            if (a != -1 && b != -1)
            {
                //Exibe a aresta adicionada na MST parcial, atualiza contador de arestas adicionadas,
                //marca os nos da aresta como nos inclusos na MST e atualiza o custo minimo da MST parcial
                printf("Selected edge %d:(%d, %d), cost: %d\n",
                       edge_count++, a, b, min);
                minimum_cost = minimum_cost + min;
                vertices_in_mst[b] = vertices_in_mst[a] = 1;
            }
        }

        //libera conjunto de nos ja inclusos
        free(vertices_in_mst);

        //exibe o custo da MST encontrada
        printf("MST cost: %ld\n", minimum_cost);

        //marca o tempo final de execucao
        clock_t end = clock();
        //atualiza o tempo total parcial de execucao do algoritmo
        partial_time += (double)(end - begin) / CLOCKS_PER_SEC;
    }

    //atualiza a linha de tabela passada com o tempo de execucao medio calculado
    line->execution_time = partial_time / ntrials;

    return minimum_cost; //retorna valor da MST encontrada
}
