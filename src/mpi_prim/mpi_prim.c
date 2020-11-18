#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include "mpi.h"
#include "mpi_prim.h"
#include "stats/stats.h"
#include "stats/csv.h"
#include "limits.h"


/*
Recebe:
   - rank: numero do rank de cada processo que está executando a função;
   - key: Array de chaves. Cada posição é o peso de uma aresta leve que liga naquele vértice. O "id" do vértice é sua posicão. O valor é o peso da aresta incidente.
   - parent: Array indicando em cada posição, quem é o pai (através de uma aresta, qual é o nó que incide sobre mim) de cada vértice. A posição é o "id" do nó. O valor é o "id" do nó pai
   - mstSet: Array que indica qual vértice está na Arvore (ou sub-arvore) geradora minima ou não. Quando todas posições estiverem True, todos os vértices estão incluídos na árvore. O "id" do vértice é sua posicão. O valor indica se o vértice está ou não na MST.
Retorna:
    - Printa na tela as informações mandadas de forma organizada, para facilitar o debug
*/
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
/*
Recebe:
 - V: numero de vértices no grafo;
 - key: Array de chaves. Cada posição é o peso de uma aresta leve que liga naquele vértice. O "id" do vértice é sua posicão. O valor é o peso da aresta incidente.

Retorna:
 - O custo da árvore geradora mínima
*/
//Para calcular o peso da MST, basta percorrer o array de keys. Cada posição é o peso de uma aresta leve que liga naquele vértice. O "id" do vértice é sua posicão
long total_cost(int V, int* key){
    long totalCost = 0;
    for(int i = 1; i<V; i++){
        totalCost+=key[i];
    }
    return totalCost;
}
/*
Recebe:
 - graph: Sub grafo que cada processo toma conta. Como estamos utilizando uma matriz de adjascencia espelhada, podemos fazer essa divisão do grafo
 - u: Nó que foi eleito o melhor candidato. Precisamos analisar as arests que levam aos seus vizinhos.
 - key: Array de chaves. Cada posição é o peso de uma aresta leve que liga naquele vértice. O "id" do vértice é sua posicão. O valor é o peso da aresta incidente.
 - mstSet: Array que indica qual vértice está na Arvore (ou sub-arvore) geradora minima ou não. Quando todas posições estiverem True, todos os vértices estão incluídos na árvore. O "id" do vértice é sua posicão. O valor indica se o vértice está ou não na MST.
 - parent: Array indicando em cada posição, quem é o pai (através de uma aresta, qual é o nó que incide sobre mim) de cada vértice. A posição é o "id" do nó. O valor é o "id" do nó pai
 - index: Indica a partir de qual posição dos arrays (key, mstSet e parent) cada processo irá trabalhar
 - vertexPerProcess: quantidade de vértices por processo incluido o valor do ultimo processo que acaba ficando com mais vértices para cuidar

Retorna:

 - Não possui retorno. Através das referências dos arrays de chave e de pais, ela os atualiza.

No algoritmo de prim, dado um nó que acabou de entrar na árvore, precisamos atualizar os valores de chave de cada nó vizinho a ele.
Para isso, percorremos o grafo (matriz de adjascencia) e fazemos as seguntes verificações:
 - Se existe aresta naquela posição do grafo
 - Se o vértice incidente daquela aresta, não está na arvore geradora mínima
 - Se a chave no array de keys for maior que a aresta que está incidindo a partir do melhor candidato
 Se todas essas verificações forem verdadeiras, atualizamos o pai e a chave do vértice vizinho
*/
void update_neighbors(int** graph, int u, int* key, int* mstSet, int* parent, int index, int vertexPerProcess){
    for(int v=index; v<(index + vertexPerProcess); v++){
        if((graph[u][v] > 0) && (mstSet[v] == 0) && key[v]>graph[u][v]){
            key[v] = graph[u][v];
            parent[index] = u;
        }
    }
}
/*
Recebe:

 - key: Array de chaves. Cada posição é o peso de uma aresta leve que liga naquele vértice. O "id" do vértice é sua posicão. O valor é o peso da aresta incidente.
 - mstSet: Array que indica qual vértice está na Arvore (ou sub-arvore) geradora minima ou não. Quando todas posições estiverem True, todos os vértices estão incluídos na árvore. O "id" do vértice é sua posicão. O valor indica se o vértice está ou não na MST.
 - parent: Array indicando em cada posição, quem é o pai (através de uma aresta, qual é o nó que incide sobre mim) de cada vértice. A posição é o "id" do nó. O valor é o "id" do nó pai
 - index: Indica a partir de qual posição dos arrays (key, mstSet e parent) cada processo irá trabalhar
 - rows: Indica até onde, em cada array, cada processo irá trabalhar

Retorna:

 - Um array de 3 posições contendo a posição, o valor da aresta incidente mínima e o pai do vértice que a aresta incide.

*/
//Cada processo calcula o melhor candidato a ser eleito para entrar na MST. Para isso, dentre os vértices que cada processo tom conta, ele verifica quem possui o peso mínimo e retorna caso encontre
int* min_key(int *key, int* mstSet, int* parent, int index, int rows){
    int min = INT_MAX;
    int min_index = INT_MAX;

    //alocando memória para retornar o candidato
    int* candidate = (int*)malloc(3*sizeof(int));

    //descobrindo quem é o melhor candidato dentro do subconjunto do processo correntej
    for(int i=index; i<rows; i++){
        if(key[i] < min && mstSet[i] == 0){
            min = key[i];
            min_index = i;
        }
    }

    //caso não tenha atualizado os vértices do processo (no update neighbors) pode ser que não tenhamos um candidato
    if (min_index == INT_MAX){
        candidate[0] = min_index;
        candidate[1] = min;
        candidate[2] = -1;
    }//Caso o processo encontre algum possível candidato, irá o retornar
    else{
        candidate[0] = min_index;
        candidate[1] = min;
        candidate[2] = parent[min_index];               
    }

    return candidate;

}
/*
Versao paralela em MPI do algoritmo de Prim.

Recebe:

   - cost: matriz de adjacencias contendo os custos de cada aresta;
   - rows: total de linhas (que também é o numero de vértices no grafo);
   - columns: total de colunas;
   - rank: numero do rank de cada processo que está executando a função;
   - size: quantidade total de processos que estão executando
   - ntrials: numero de repeticoes do cenario, para calculo das medias de tempo de execucao;
   - line: linha da tabela de resultados a ser preenchida com o tempo de execucao da funcao

Retorna:
    - custo minimo da MST encontrada pelo algoritmo
*/
double* mpi_prim_minimum_spanning_tree(int **cost, int rows, int columns, int rank, int size, int ntrials, Table *line){

    double* partial_time = (double*)malloc(2*sizeof(double)); //0 - communication time | 1 -> execution time
    partial_time[0] = 0.0; //Valores iniciais dos tempos parciais
    partial_time[1] = 0.0;
    int *key = (int *)malloc(rows * sizeof(int)); //instanciamos a variável fora do laço principal para podermos printar o custo total no final
    
    for (int asdf = 0; asdf < ntrials; asdf++) //executamos esse programa ntrials vezes para pegarmos o tempo médio das execuções
    {
        MPI_Barrier(MPI_COMM_WORLD);
        
        //instanciando todas as variáveis a serem utilizaads
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
        for(int i=1; i<rows; i++){ //não conseguimos utilizar o memset aqui pois estava dando valores errados
            key[i] = INT_MAX;
        }

        if ((rank == size-1) && size>1){
            // aqui nós temos os indices pelo qual os ranks vao trabalhar 
            // do index até os elementosFaltando, temos as linhas do grafo  
            // também serve para separar as areas da key que cada processo irá mexer
            int elementosFaltando = rows - index;
        }   

        MPI_Barrier(MPI_COMM_WORLD); //usamos o barrier para todo mundo começar a contar o tempo de execução simultaneamente
        comm_time_exec -= MPI_Wtime();

        /*
        O programa termina só quando todas as arestas estiverem na MST. Podemos fazer essa verificação de duas formas:
        Ou percorrendo o mstSet e verificando se todas as posições estiverem igual a True
        Ou simplesmente iterando sob a quantidade de vértices. Porque só inserimos 1 vértice na MST a cada iteração.  <--usamos essa abordagem*/
        for (int cout=0; cout<rows; cout++){ 
            //Cada processo elege um candidato a ser inserido na MST
            if (rank == (size-1)){ //Se a divisão do grafo não for igual, o ultimo rank irá ficar com a parcela restante
                candidate = min_key(key, mstSet, parent, index, rows);
            }else{
                candidate = min_key(key, mstSet, parent, index, index + vertexPerProcess);
            }

            // Resolvemos deixar esse print para debug
            // MPI_Barrier(MPI_COMM_WORLD);
            // printf("Rank - %d\n Candidato:\nIndex - %d; Chave - %d; Pai - %d\n______________________________\n", rank, candidate[0],candidate[1], candidate[2]);
            // MPI_Barrier(MPI_COMM_WORLD);

            //alocando memoria para o receiver buffer do rank 0 para receber os candidatos de cada processo
            if (rank == 0) { 
                rbuf = (int *)malloc(size*3*sizeof(int)); 
            }

            //recebendo os candidatos no mestre pra selecionar o melhor
            //MPI_Barrier(MPI_COMM_WORLD);
            //comm_time_comm -= MPI_Wtime();
            MPI_Gather(candidate, 3, MPI_INT, rbuf, 3, MPI_INT, 0, MPI_COMM_WORLD);
            // MPI_Barrier(MPI_COMM_WORLD);
            // if (rank==0) comm_time_comm += MPI_Wtime();
            
            //selecionando melhor candidato
            if (rank == 0) { 
                bestCandidate = (int*)malloc(3*sizeof(int));
                bestCandidate[1] = INT_MAX;
                for(int i=1; i<(3*size); i+=3){   //Percorre o vetor de candidatos e escolhe aquele que tem menor peso
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
            mstSet[bestCandidate[0]] = 1; //Após todos os processos receberem o melhor candidato, inserimos ele na MST

            if (rank == (size -1)) vertexPerProcess = rows-index; //a quantidade de vértices por processo do ultimo é maior
            
            //debug_print(rank, key, parent, mstSet, rows);
            //Atualizando os vizinhos
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
