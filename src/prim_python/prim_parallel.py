#baseado nos tutoriais https://www.geeksforgeeks.org/prims-minimum-spanning-tree-mst-greedy-algo-5/
#                      https://panda.ime.usp.br/pythonds/static/pythonds_pt/07-Grafos/PrimsSpanningTreeAlgorithm.html
#                      https://rabernat.github.io/research_computing/parallel-programming-with-mpi-for-python.html
#                      https://www.kth.se/blogs/pdc/2019/11/parallel-programming-in-python-mpi4py-part-2/

from mpi4py import MPI
from time import time
import sys
import random
import csv


def generateRandomGraph(V):
    graph = [[0 for column in range(V)]  
            for row in range(V)] 

    for i in range(V):
        for j in range(V):
            if i==j:
                graph[i][j] = 0
            else:
                graph[i][j] = random.randint(0, V) 

    return graph

def printGraph(graph):
    for i in range(V):
        for j in range(V):
            print("{} ".format(graph[i][j]), end="")
        print()
    print()

def printMST(self): 
    print("Edge \tWeight")
    for i in range(1, self.V): 
        print(self.parent[i], "-", i, "\t", self.graph[self.parent[i]][i] )
    
    print("\ntotal cost: {}".format(self.totalCost()))

def minKey(V, key, mstSet): 
    min = float("inf")
    min_index = -1
    for v in range(V): 
        if key[v] < min and mstSet[v] == False: 
            min = key[v] 
            min_index = v 

    candidato = (min_index, min)
    return candidato
    
def totalCost(self):
    totalCost = 0
    for i in range(1, self.V):
        totalCost += self.key[i]
    return totalCost

def getAdj(self, u):
    adj = []
    for v in range(self.V):
        if self.graph[u][v] > 0:
            adj.append(v)

    return adj
    
def cleanObj(self):
    self.key = [float("inf")] * self.V 
    self.key[0] = 0 
    self.parent = [None] * self.V 
    self.parent[0] = -1 
    self.mstSet = [False] * self.V

def primMST(self):
    for cout in range(self.V): 
        u = self.minKey() 
        self.mstSet[u] = True

        for v in range(self.V): 
            if self.graph[u][v] > 0 and self.mstSet[v] == False and self.key[v] > self.graph[u][v]: 
                    self.key[v] = self.graph[u][v] 
                    self.parent[v] = u 

    self.printMST()

def findCandidate(V, key, mst):
    for _ in range(V):
        u = minKey(V, key, mst) 

    return u

def primParallel(self):
        for _ in range(V): 
            u = self.minKey() 
        self.mstSet[u] = True

        for v in range(self.V): 
            if self.graph[u][v] > 0 and self.mstSet[v] == False and self.key[v] > self.graph[u][v]: 
                    self.key[v] = self.graph[u][v] 
                    self.parent[v] = u 

V = 12 #nro de vertices

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()
veticesPorProcesso = int(V/size)


if rank == 0:
    print("Numero de vertices por rank eh: " + str(veticesPorProcesso))

    graph = generateRandomGraph(V)
    printGraph(graph)

    # dividindo o grafo em chunks
    chunks = [[] for _ in range(size)]
    for i, chunk in enumerate(graph):
        chunks[i % size].append(chunk)

    key = [float("inf")] * veticesPorProcesso 
    key[0] = 0 

    parent = [None] * veticesPorProcesso 
    parent[0] = -1 

    mstSet = [False] * veticesPorProcesso 

elif rank == (size-1):
    index = rank * veticesPorProcesso
    countV = V - index

    key = [float("inf")] * countV 
    parent = [None] * countV
    mstSet = [False] * countV

    graph = None
    chunks = None

else:
    graph = None
    chunks = None
    
    key = [float("inf")] * veticesPorProcesso 
    parent = [None] * veticesPorProcesso
    mstSet = [False] * veticesPorProcesso

graph = comm.scatter(chunks, root=0)

# comm.Barrier()
# print(str(rank) + ": " + str(graph))
# comm.Barrier()
# print("key "+ str(rank) + ": " + str(key))
# comm.Barrier()
# print("parent " + str(rank) + ": " + str(parent))
# comm.Barrier()
# print("mstSet" +str(rank) + ": " + str(mstSet))
# comm.Barrier()

if rank == (size -1):
    candidate = findCandidate(countV, key, mstSet)
else:
    candidate = findCandidate(veticesPorProcesso, key, mstSet)

recvbuf = None
if rank == 0:
    recvbuf = [[] for _ in range(size)]
    
# perform the Gather:
comm.Gather(candidate, recvbuf, root=0)

if rank == 0:
    print("\n_________________________________________________")
    print(recvbuf)


    # with open('parallel.csv', 'w', newline='') as file:
    #     writer = csv.writer(file)
    #     writer.writerow(["n", "Tempo de Execução"])
    #     for arg in args:
    #         if(not arg.isdecimal()):
    #             print("Tente novamente! Argumento " + arg + " invalido")
    #             break
    #         else:
    #             g = Graph(int(arg)) 
    #             g.generateRandomGraph()
    #             ini = time()
    #             g.primParallel()
    #             fim = time()

    #             writer.writerow([arg, fim-ini])

    


# Contributed by Divyanshu Mehta 