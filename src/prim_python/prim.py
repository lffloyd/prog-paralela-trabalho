#baseado nos tutoriais https://www.geeksforgeeks.org/prims-minimum-spanning-tree-mst-greedy-algo-5/
#                      https://panda.ime.usp.br/pythonds/static/pythonds_pt/07-Grafos/PrimsSpanningTreeAlgorithm.html
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
        
    def printMST(self): 
        print("Edge \tWeight")
        for i in range(1, self.V): 
            print(self.parent[i], "-", i, "\t", self.graph[i][self.parent[i]] )
        
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

g = Graph(5) 
g.graph = [ [0, 2, 0, 6, 0], 
            [2, 0, 3, 8, 5], 
            [0, 3, 0, 0, 7], 
            [6, 8, 0, 0, 9], 
            [0, 5, 7, 9, 0]] 
  
g.primMST()

  
# Contributed by Divyanshu Mehta 