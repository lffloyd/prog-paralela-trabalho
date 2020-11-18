#baseado nos tutoriais https://www.geeksforgeeks.org/prims-minimum-spanning-tree-mst-greedy-algo-5/
#                      https://panda.ime.usp.br/pythonds/static/pythonds_pt/07-Grafos/PrimsSpanningTreeAlgorithm.html
#                      https://rabernat.github.io/research_computing/parallel-programming-with-mpi-for-python.html
#                      https://www.kth.se/blogs/pdc/2019/11/parallel-programming-in-python-mpi4py-part-2/

from mpi4py import MPI
from time import time
import sys
import csv


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

def printGraph(graph):
    for i in range(V):
        for j in range(V):
            print("{} ".format(graph[i][j]), end="")
        print()
    print()

def printMST(V, parent, key): 
    print("Edge \tWeight")
    for i in range(1, V): 
        print(parent[i], "-", i, "\t", key[i] )
    
    print("\ntotal cost: {}".format(totalCost(V, key)))
    
def totalCost(V, key):
    totalCost = 0
    for i in range(1, V):
        totalCost += key[i]
    return totalCost

def minKey(key, mstSet, index, sizeKeys, parent): 
    min = float("inf")
    min_index = float("inf")
    for v in range(index, sizeKeys): 
        if key[v] < min and mstSet[v] == False: 
            min = key[v] 
            min_index = v 

    return [min_index, min, -1] if min_index == float("inf") else [min_index, min, parent[int(min_index)]]

def updateNeighbors(graph, u, key, mstSet, nkeys, rank, nVertices, parent):
        
    for v in range(nkeys):  # 0, 1
        index = v + (nVertices*rank) # 
        if graph[v][u] > 0 and mstSet[index] == False and key[index] > graph[v][u]: 
            key[index] = graph[v][u] 
            parent[index] = u
        
    #print(str(rank) + ": " + str(graph) + "\nkey "+ str(rank) + ": " + str(key) + "\nparent " + str(rank) + ": " + str(parent) + "\nmstSet" +str(rank) + ": " + str(mstSet) + "\n")

#####################################################################################################
#############################################inicio##################################################
#####################################################################################################

args = sys.argv[1:]

with open('parallel_python_n8.csv', 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(["n", "Tempo de Execução"])
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

            if rank == 0:
                #print("Numero de vertices por rank eh: " + str(verticesPorProcesso))

                graph = generateGraph(V)
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

            graph = comm.bcast(graph, root = 0)

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

            comm.barrier()
            if rank ==0:
                ini = time()
            comm.barrier()

            for cout in range(V):
                if rank == (size -1):
                    candidate = minKey(key, mstSet, index, V, parent)
                else:
                    candidate = minKey(key, mstSet, index, index + verticesPorProcesso, parent)

                recvbuf = comm.gather(candidate, root=0)

                if rank == 0:
                    melhorCandidato = [-1, float("inf"), -1]
                    for i in range(len(recvbuf)):
                        if(recvbuf[i][1]<melhorCandidato[1]):
                            melhorCandidato[0] = recvbuf[i][0]
                            melhorCandidato[1] = recvbuf[i][1]
                            melhorCandidato[2] = recvbuf[i][2]
                    #recvbuf.sort(key=lambda x: x[1])#em vez de dar sort, eu posso só percorrer e pegar o minimo
                    #melhorCandidato = recvbuf[0]
                    # melhorCandidato.append(parent[melhorCandidato[0]])
                    # print(melhorCandidato)
                else:
                    melhorCandidato = None

                melhorCandidato = comm.bcast(melhorCandidato, root = 0)

                key[melhorCandidato[0]] = melhorCandidato[1]
                parent[melhorCandidato[0]] = melhorCandidato[2]
                mstSet[melhorCandidato[0]] = True

                if rank == (size -1):
                    verticesPorProcesso = V-index
                
                updateNeighbors(graph, melhorCandidato[0], key, mstSet, verticesPorProcesso, rank, int(V/size),parent)

            comm.barrier()
            if rank == 0:
                fim = time()
                writer.writerow([arg, fim-ini])
                print(totalCost(V, key))
                #printMST(V, parent, key)

MPI.Finalize() 

# Contributed by Divyanshu Mehta '''