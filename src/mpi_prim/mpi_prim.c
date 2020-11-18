#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include "mpi.h"
#include "mpi_prim.h"
#include "stats/stats.h"
#include "stats/csv.h"
#include "limits.h"

void debug_print(int rank, int* key, int* parent, int* mstSet, int rows){
    printf("Rank - %d", rank);
    printf("\nKey: ");
    for(int i=0; i<rows; i++){
        printf("%d ", key[i]);
    }
    printf("\nParent: ");
    for(int i=0; i<rows; i++){
        printf("%d ", parent[i]);
    }
    printf("\nmstSet: ");
    for(int i=0; i<rows; i++){
        printf("%d ", mstSet[i]);
    }
    printf("\n__________________________________________\n");   

}

long total_cost(int V, int* key){
    long totalCost = 0;
    for(int i = 1; i<V; i++){
        totalCost+=key[i];
    }
    return totalCost;
}

void update_neighbors(int** graph, int u, int* key, int* mstSet, int* parent, int index, int vertexPerProcess){
    for(int v=index; v<(index + vertexPerProcess); v++){
        if((graph[u][v] > 0) && (mstSet[v] == 0) && key[v]>graph[u][v]){
            key[v] = graph[u][v];
            parent[index] = u;
        }
    }
}

int* min_key(int *key, int* mstSet, int* parent, int index, int rows){
    int min = INT_MAX;
    int min_index = INT_MAX;

    int* candidate = (int*)malloc(3*sizeof(int));

    for(int i=index; i<rows; i++){
        if(key[i] < min && mstSet[i] == 0){
            min = key[i];
            min_index = i;
        }
    }

    if (min_index == INT_MAX){
        candidate[0] = min_index;
        candidate[1] = min;
        candidate[2] = -1;
    }
    else{
        candidate[0] = min_index;
        candidate[1] = min;
        candidate[2] = parent[min_index];               
    }

    return candidate;

}

double* mpi_prim_minimum_spanning_tree(int **cost, int rows, int columns, int rank, int size, int ntrials, Table *line){

    double* partial_time = (double*)malloc(2*sizeof(double)); //0 - communication time | 1 -> execution time
    partial_time[0] = 0.0;
    partial_time[1] = 0.0;
    int *key = (int *)malloc(rows * sizeof(int));
    for (int asdf = 0; asdf < ntrials; asdf++)
    {
        MPI_Barrier(MPI_COMM_WORLD);
        
        double comm_time_comm = 0.0;
        double comm_time_exec = 0.0;   
        int vertexPerProcess = rows/size;
        int index = rank * vertexPerProcess;
        int *candidate, *rbuf, *bestCandidate;

        int *mstSet = (int *)malloc(rows * sizeof(int));
        memset(mstSet, 0, rows * sizeof(int));

        int *parent = (int *)malloc(rows * sizeof(int));
        memset(parent, -5, rows * sizeof(int));
        parent[0] = -1;

        key[0] = 0;
        for(int i=1; i<rows; i++){
            key[i] = INT_MAX;
        }

        if ((rank == size-1) && size>1){
            // aqui nós temos os indices pelo qual os ranks vao trabalhar 
            //do index até os elementosFaltando, temos as linhas do grafo  
            // também serve para separar as areas da key que cada processo irá mexer
            int elementosFaltando = rows - index;
        }   

        MPI_Barrier(MPI_COMM_WORLD);
        comm_time_exec -= MPI_Wtime();

        //Cada processo Pi encontra uma aresta candidata de peso minimo Ei conectando a MST no vertice Vi
        for (int cout=0; cout<rows; cout++){ //mudar pra rows dps
            if (rank == (size-1)){
                //printf("Index: %d \nRows: %d \n_________________________\n", index, rows);
                candidate = min_key(key, mstSet, parent, index, rows);
            }else{
                //printf("Index: %d \nRows: %d \n_________________________\n", index, index + vertexPerProcess);
                candidate = min_key(key, mstSet, parent, index, index + vertexPerProcess);
            }

            // MPI_Barrier(MPI_COMM_WORLD);
            // printf("Rank - %d\n Candidato:\nIndex - %d; Chave - %d; Pai - %d\n______________________________\n", rank, candidate[0],candidate[1], candidate[2]);
            // MPI_Barrier(MPI_COMM_WORLD);

            //recebendo os candidatos no mestre pra selecionar o melhor
            if (rank == 0) { 
                rbuf = (int *)malloc(size*3*sizeof(int)); 
            }

            //MPI_Barrier(MPI_COMM_WORLD);
            //comm_time_comm -= MPI_Wtime();
            MPI_Gather(candidate, 3, MPI_INT, rbuf, 3, MPI_INT, 0, MPI_COMM_WORLD);
            // MPI_Barrier(MPI_COMM_WORLD);
            // if (rank==0) comm_time_comm += MPI_Wtime();
            
            //selecionando melhor candidato
            if (rank == 0) { 
                bestCandidate = (int*)malloc(3*sizeof(int));
                bestCandidate[1] = INT_MAX;
                for(int i=1; i<(3*size); i+=3){
                    if (rbuf[i]<bestCandidate[1]){
                        bestCandidate[0] = rbuf[i-1];
                        bestCandidate[1] = rbuf[i];
                        bestCandidate[2] = rbuf[i+1];  
                    }
                }
                //printf("MelhorCandidato: Index %d | Key: %d | Pai: %d\n______________________________\n", bestCandidate[0], bestCandidate[1], bestCandidate[2]);
            }
            //avisando para todo mundo quem é o melhor candidato
            // MPI_Barrier(MPI_COMM_WORLD);
            // if (rank==0) comm_time_comm -= MPI_Wtime();
            if (size>1) MPI_Bcast(bestCandidate, 3, MPI_INT, 0, MPI_COMM_WORLD);
            // MPI_Barrier(MPI_COMM_WORLD);
            // if (rank==0) comm_time_comm += MPI_Wtime();
            //atualizando quem ta no movimento sem terra
            key[bestCandidate[0]] = bestCandidate[1];
            parent[bestCandidate[0]] = bestCandidate[2];
            mstSet[bestCandidate[0]] = 1;

            if (rank == (size -1)) vertexPerProcess = rows-index;
            
            //Atualizando os vizinhos
            //debug_print(rank, key, parent, mstSet, rows);
            update_neighbors(cost, bestCandidate[0], key, mstSet, parent, index, vertexPerProcess);
        }

        MPI_Barrier(MPI_COMM_WORLD);
        comm_time_exec += MPI_Wtime();
        partial_time[0] += comm_time_comm;
        partial_time[1] += comm_time_exec;
        free(parent);
        free(mstSet);
    }
    partial_time[1] = partial_time[1]/ntrials;
    //partial_time[1] = partial_time[1]/ntrials;
    //printf("Tempo de execução %f | Tempo de comunicação %f", partial_time[1], partial_time[0]);
    //if (rank==0) printf("Tempo de execução: %f", partial_time / ntrials);
    if (rank==0) printf("Custo minimo: %ld\n", total_cost(rows, key));
    //line->execution_time = partial_time / ntrials;

    return partial_time;
}
