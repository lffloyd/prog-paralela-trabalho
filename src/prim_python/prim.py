#baseado nos tutoriais https://www.geeksforgeeks.org/prims-minimum-spanning-tree-mst-greedy-algo-5/
#                      https://panda.ime.usp.br/pythonds/static/pythonds_pt/07-Grafos/PrimsSpanningTreeAlgorithm.html
#                      https://rabernat.github.io/research_computing/parallel-programming-with-mpi-for-python.html

from mpi4py import MPI
from time import time
import sys
import random
import csv

class Graph(): 
  
    def __init__(self, vertices): 
        self.V = vertices 
        self.graph = [[0 for column in range(vertices)]  
                    for row in range(vertices)] 
        self.key = [float("inf")] * self.V 
        self.key[0] = 0 
        self.parent = [None] * self.V 
        self.parent[0] = -1 
        self.mstSet = [False] * self.V 
        
    # def fisherYatesShuffle(self,weights,n):
    #     for i in range(n):
    #         j = random.randint(0, n-1)
    #         temp = weights[i]
    #         weights[i] = weights[j]
    #         weights[j] = temp
        
    #     return weights
    
    # def generateRandomWeights(self):
    #     weights = []
    #     n = self.V*self.V
    #     for i in range(n):
    #         weights.append(i)
        
    #     return self.fisherYatesShuffle(weights,n)

    # def generateRandomGraph(self):
    #     weights = self.generateRandomWeights()

    #     idx=0
    #     for i in range(self.V):
    #         for j in range(self.V):
    #             if i==j:
    #                 self.graph[i][j] = 0
    #             else:
    #                 self.graph[i][j] = weights[idx]
                
    #             idx+=1 

    def generateRandomGraph(self):
        for i in range(self.V):
            for j in range(self.V):
                if i==j:
                    self.graph[i][j] = 0
                else:
                    self.graph[i][j] = random.randint(0, self.V) 

    def printGraph(self):
        for i in range(self.V):
            for j in range(self.V):
                print("{} ".format(self.graph[i][j]), end="")
            print()
        print()

    def printMST(self): 
        print("Edge \tWeight")
        for i in range(1, self.V): 
            print(self.parent[i], "-", i, "\t", self.graph[self.parent[i]][i] )
        
        print("\ntotal cost: {}".format(self.totalCost()))

    def minKey(self): 
        min = float("inf")

        for v in range(self.V): 
            if self.key[v] < min and self.mstSet[v] == False: 
                min = self.key[v] 
                min_index = v 
  
        return min_index    

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
    
    def primParallel(self):
        

args = sys.argv[1:]
executionType = args.pop(0)

if executionType == "seq":
    with open('seq.csv', 'w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(["n", "Tempo de Execução"])
        for arg in args:
            if(not arg.isdecimal()):
                print("Tente novamente! Argumento " + arg + " invalido")
                break
            else:
                g = Graph(int(arg)) 
                g.generateRandomGraph()
                ini = time()
                g.primMST()
                fim = time()

                writer.writerow([arg, fim-ini])

elif executionType == "parallel":
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()

    g = Graph(int(12)) 
    g.primParallel()


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
else:
    print("Tente novamente! Argumento " + arg + " invalido")
    


# Contributed by Divyanshu Mehta 