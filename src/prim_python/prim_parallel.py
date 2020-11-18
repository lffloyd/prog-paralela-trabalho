from mpi4py import MPI
from time import time
import sys
import csv

'''
Recebe:

 - V: número total de vértices;
   
Retorna:

 - Um grafo VxV conexo e com pesos de cada aresta diferentes

'''
def generateGraph(V):
    graph = [[0 for column in range(V)]  
            for row in range(V)] 
    value = 1
    for i in range(V):
        for j in range(V):
            if i==j:
                graph[i][j] = 0
            elif i>j:
                graph[i][j] = value
                graph[j][i] = graph[i][j]
            value += 1

    return graph

'''
Recebe:

 - graph: grafo do problema

Retorna:

 - desenho da matriz do grafo no terminal

 '''
def printGraph(graph):
    for i in range(V):
        for j in range(V):
            print("{} ".format(graph[i][j]), end="")
        print()
    print()

'''
Recebe:

 - V: total de vértices do grafo
 - parent: Array indicando em cada posição, quem é o pai (através de uma aresta, qual é o nó que incide sobre mim) de cada vértice. A posição é o "id" do nó. O valor é o "id" do nó pai
 - key: Array de chaves. Cada posição é o peso de uma aresta leve que liga naquele vértice. O "id" do vértice é sua posicão. O valor é o peso da aresta incidente.

Retorna:

 - Mostra no terminal as arestas utilizadas entre quais nós

'''
def printMST(V, parent, key): 
    print("Edge \tWeight")
    for i in range(1, V): 
        print(parent[i], "-", i, "\t", key[i] )
    
    print("\ntotal cost: {}".format(totalCost(V, key)))

'''
Recebe:

 - V: total de vértices do grafo
 - key: Array de chaves. Cada posição é o peso de uma aresta leve que liga naquele vértice. O "id" do vértice é sua posicão. O valor é o peso da aresta incidente.

 Retorna:
  
 - O custo da árvore geradora mínima

'''    
#Para calcular o peso da MST, basta percorrer o array de keys. Cada posição é o peso de uma aresta leve que liga naquele vértice. O "id" do vértice é sua posicão
def totalCost(V, key): 
    totalCost = 0
    for i in range(1, V):
        totalCost += key[i]
    return totalCost

'''
Recebe:

 - key: Array de chaves. Cada posição é o peso de uma aresta leve que liga naquele vértice. O "id" do vértice é sua posicão. O valor é o peso da aresta incidente.
 - mstSet: Array que indica qual vértice está na Arvore (ou sub-arvore) geradora minima ou não. Quando todas posições estiverem True, todos os vértices estão incluídos na árvore. O "id" do vértice é sua posicão. O valor indica se o vértice está ou não na MST.
 - index: Indica a partir de qual posição dos arrays (key, mstSet e parent) cada processo irá trabalhar
 - sizeKeys: Indica até onde, em cada array, cada processo irá trabalhar
 - parent: Array indicando em cada posição, quem é o pai (através de uma aresta, qual é o nó que incide sobre mim) de cada vértice. A posição é o "id" do nó. O valor é o "id" do nó pai

Retorna:

 - Um array de 3 posições contendo a posição, o valor da aresta incidente mínima e o pai do vértice que a aresta incide.

'''
#Cada processo calcula o melhor candidato a ser eleito para entrar na MST. Para isso, dentre os vértices que cada processo tom conta, ele verifica quem possui o peso mínimo e retorna caso encontre
def minKey(key, mstSet, index, sizeKeys, parent): 
    min = float("inf")
    min_index = float("inf")
    for v in range(index, sizeKeys): 
        if key[v] < min and mstSet[v] == False: 
            min = key[v] 
            min_index = v 

    return [min_index, min, -1] if min_index == float("inf") else [min_index, min, parent[int(min_index)]]

'''
Recebe:
 - graph: Sub grafo que cada processo toma conta. Como estamos utilizando uma matriz de adjascencia espelhada, podemos fazer essa divisão do grafo
 - u: Nó que foi eleito o melhor candidato. Precisamos analisar as arests que levam aos seus vizinhos.
 - key: Array de chaves. Cada posição é o peso de uma aresta leve que liga naquele vértice. O "id" do vértice é sua posicão. O valor é o peso da aresta incidente.
 - mstSet: Array que indica qual vértice está na Arvore (ou sub-arvore) geradora minima ou não. Quando todas posições estiverem True, todos os vértices estão incluídos na árvore. O "id" do vértice é sua posicão. O valor indica se o vértice está ou não na MST.
 - nkeys: quantidade de vértices por processo incluido o valor do ultimo processo que acaba ficando com mais vértices para cuidar
 - rank: rank de cada processo
 - nVertices: também é a quantidade de vértices por processo. Serve para calcular o index
 - parent: Array indicando em cada posição, quem é o pai (através de uma aresta, qual é o nó que incide sobre mim) de cada vértice. A posição é o "id" do nó. O valor é o "id" do nó pai

Retorna:

 - Não possui retorno. Através das referências dos arrays de chave e de pais, ela os atualiza.

'''
#No algoritmo de prim, dado um nó que acabou de entrar na árvore, precisamos atualizar os valores de chave de cada nó vizinho a ele.
#Para isso, percorremos o grafo (matriz de adjascencia) e fazemos as seguntes verificações:
# - Se existe aresta naquela posição do grafo
# - Se o vértice incidente daquela aresta, não está na arvore geradora mínima
# - Se a chave no array de keys for maior que a aresta que está incidindo a partir do melhor candidato
# Se todas essas verificações forem verdadeiras, atualizamos o pai e a chave do vértice vizinho
#OBSERVACAO: Você pode notar que dentro do IF, acessamos o grafo usando 'v' como linha e 'u' e no algoritmo em MPI fazemos o contrário
#precisamos acessar dessa forma nesse algoritmo porque aqui estamos dividindo o grafo gerado inicialmente em submatrizes
#No código de MPI em C, não fazemos essa divisão
#Então se tentarmos indexar como graph[u][v] nesse código, recebemos o erro 'Index out of range'
def updateNeighbors(graph, u, key, mstSet, nkeys, rank, nVertices, parent):
    for v in range(nkeys):  
        index = v + (nVertices*rank) 
        if graph[v][u] > 0 and mstSet[index] == False and key[index] > graph[v][u]: 
            key[index] = graph[v][u] 
            parent[index] = u
    
    #print(str(rank) + ": " + str(graph) + "\nkey "+ str(rank) + ": " + str(key) + "\nparent " + str(rank) + ": " + str(parent) + "\nmstSet" +str(rank) + ": " + str(mstSet) + "\n")



#########################################################################################################################################################################################
########################################     INICIO     #################################################################################################################################
#########################################################################################################################################################################################

args = sys.argv[1:] #Dos argumentos, excluímos o primeiro que é o nome do arquivo para deixar na variável só o array de Número de vértices a serem calculados.

with open('parallel_python_teste.csv', 'w', newline='') as file: #criamos o arquivo.csv para gerar a tabela
    writer = csv.writer(file)                                 
    writer.writerow(["n", "Tempo de Execução"]) #escrevemos o titulo da tabela no arquivo
    for arg in args: 
        if(not arg.isdecimal()):
            print("Tente novamente! Argumento " + arg + " invalido")
            break
        else:
            V = int(arg) #nro de vertices

            comm = MPI.COMM_WORLD
            rank = comm.Get_rank()
            size = comm.Get_size()
            verticesPorProcesso = int(V/size)
            index = rank * verticesPorProcesso

            #instanciamos os arrays que vamos utilizar no programa
            if rank == 0:
                graph = generateGraph(V) #Eh criado a matriz de adjascencias 
                #printGraph(graph)
                key = [float("inf")] * V 
                key[0] = 0 
                parent = [None] * V 
                parent[0] = -1 
                mstSet = [False] * V 
            else:
                graph = None
                key = [float("inf")] * V 
                key[0] = 0 
                parent = [None] * V 
                parent[0] = -1 
                mstSet = [False] * V 

            graph = comm.bcast(graph, root = 0) #Broadcast do grafo criado 

            #Nesse IF-ELSE, fazemos a divisão do grafo para cada processo
            if rank == (size -1) and size>1:
                verticesPorProcesso = V - (verticesPorProcesso*rank)
                newGraph = []
                for i in range(verticesPorProcesso):
                    newGraph.append(graph[i + index]) 
                graph = newGraph
            else:
                newGraph = []
                for i in range(verticesPorProcesso):
                    newGraph.append(graph[i + index]) 
                graph = newGraph

            #Após as variáveis serem instanciadas, começamos a contar o tempo de execução do programa
            comm.barrier()
            if rank ==0:
                ini = time()
            comm.barrier()

            #O programa termina só quando todas as arestas estiverem na MST. Podemos fazer essa verificação de duas formas:
            #Ou percorrendo o mstSet e verificando se todas as posições estiverem igual a True
            #Ou simplesmente iterando sob a quantidade de vértices. Porque só inserimos 1 vértice na MST a cada iteração.
            for cout in range(V):
                #Cada processo seleciona o seu candidato com menor chave
                if rank == (size -1):
                    candidate = minKey(key, mstSet, index, V, parent)
                else:
                    candidate = minKey(key, mstSet, index, index + verticesPorProcesso, parent)
                
                #reunimos todos os candidatos no vetor de candidatos "recvbuf" no processo de rank = 0
                recvbuf = comm.gather(candidate, root=0)
                
                if rank == 0:
                    #Instanciamos o melhor candidato para podermos fazer as comparações a partir do seu index
                    melhorCandidato = [-1, float("inf"), -1]

                    #percorremos o vetor de candidatos para escolher aquele que possui melhor valor de chave
                    for i in range(len(recvbuf)):
                        if(recvbuf[i][1]<melhorCandidato[1]):
                            melhorCandidato[0] = recvbuf[i][0]
                            melhorCandidato[1] = recvbuf[i][1]
                            melhorCandidato[2] = recvbuf[i][2]
                    #Após esse "for" temos o melhor candidato a inserir na MST
                else:
                    #precisamos instanciar a variável nos outros processos para fazer o broadcast que vem a seguir
                    melhorCandidato = None

                #Cada processo recebe o melhor candidato para poder atualizar seus arrays de chave, de pais e de quem está na MST
                melhorCandidato = comm.bcast(melhorCandidato, root = 0)
                
                #Atualiza os arrays
                key[melhorCandidato[0]] = melhorCandidato[1]
                parent[melhorCandidato[0]] = melhorCandidato[2]
                mstSet[melhorCandidato[0]] = True

                #Eh calculado a quantidade de vertices que o ultimo processo terá de lidar
                if rank == (size -1):
                    verticesPorProcesso = V-index
                
                #Atualizamos os vizinhos do melhor candidato
                updateNeighbors(graph, melhorCandidato[0], key, mstSet, verticesPorProcesso, rank, int(V/size), parent)
            
            #Após criar a MST, salvamos o tempo no arquivo .csv (tabela) e printamos na tela o custo dela.
            comm.barrier()
            if rank == 0:
                fim = time()
                writer.writerow([arg, fim-ini])
                print(totalCost(V, key))
                #printMST(V, parent, key)

MPI.Finalize() 