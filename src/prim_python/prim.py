from mpi4py import MPI
from time import time
import sys
import csv

#Nessa versão sequencial, optamos por criar uma classe de grafos, pois caso contrário, o laço principal do problema ficaria muito poluído
#MST - minimum spanning tree ou árvore geradora mínima
class Graph(): 

    '''
    Método construtor do grafo:

    - V: número de vértices
    - graph: Matriz de custos
    - key: Array de chaves. Cada posição é o peso de uma aresta leve que liga naquele vértice. O "id" do vértice é sua posicão. O valor é o peso da aresta incidente.
    - A primeira posição de key começa com 0 pois o 0 é o nó raiz
    - mstSet: Array que indica qual vértice está na Arvore (ou sub-arvore) geradora minima ou não. Quando todas posições estiverem True, todos os vértices estão incluídos na árvore. O "id" do vértice é sua posicão. O valor indica se o vértice está ou não na MST.
    - parent: Array indicando em cada posição, quem é o pai (através de uma aresta, qual é o nó que incide sobre mim) de cada vértice. A posição é o "id" do nó. O valor é o "id" do nó pai

    Retorna:

    - Não possui retorno. Através das referências dos arrays de chave e de pais, ela os atualiza.

    '''
    def __init__(self, vertices): 
        self.V = vertices 
        self.graph = [[0 for column in range(vertices)]  
                    for row in range(vertices)] 
        self.key = [float("inf")] * self.V 
        self.key[0] = 0 
        self.parent = [None] * self.V 
        self.parent[0] = -1 
        self.mstSet = [False] * self.V 

    #Método para geração da matriz de custo que representará o grafo. Um grafo VxV conexo, unidirecional e com pesos de cada aresta diferentes
    def generateGraph(self):
        value = 1
        for i in range(self.V):
            for j in range(self.V):
                if i==j:
                    self.graph[i][j] = 0
                elif i>j:
                    self.graph[i][j] = value
                    self.graph[j][i] = self.graph[i][j]
                value += 1
    
    #Método para printar no prompt a matriz de custo
    def printGraph(self):
        for i in range(self.V):
            for j in range(self.V):
                print("{} ".format(self.graph[i][j]), end="")
            print()
        print()

    #Método que através do array de pais e do array de chaves, conseguimos dizer quais arestas pertencem a MST
    def printMST(self): 
        print("Edge \tWeight")
        for i in range(1, self.V): 
            print(self.parent[i], "-", i, "\t", self.key[i] )
        
        print("\ntotal cost: {}".format(self.totalCost()))

    #A cada iteração do algoritmo, precisamos pegar uma aresta leve (uma aresta entre os vértices que estão na mst com os vértices que não estão na mst que seja a com menor peso possível) para pegar o nó incidente a aquela aresta e por na MST
    def minKey(self): 
        min = float("inf")
        
        for v in range(self.V): #Para cada nó
            if self.key[v] < min and self.mstSet[v] == False: #Se a chave desse nó até agora for menor que a minima encontrada até agora e se esse nó não está inserido na MST
                min = self.key[v]  #Atualizamos o menor valor
                min_index = v      #Atualizamos quem possui o menor valor
  
        return min_index    #Retornamos o vertice que possui a aresta leve

    #Função que calcula o peso da Arvore geradora mínima. 
    #Utilizar apenas quando se tem a MST pronta.
    #Para descobrir o peso total, basta percorrer o array de keys e somar seus valores. Lembrar que a primeira posição dele seria o "peso de uma aresta que incide no nó raiz" e por isso não contamos
    def totalCost(self):
        totalCost = 0
        for i in range(1, self.V):
            totalCost += self.key[i]
        return totalCost
  
    #Utilizamos esse método para debug do código. Ela simplesmente verificava quem são os vizinhos de um determinado nó "u".
    #Podemos usar essa função apenas percorrendo a linha de "u" pois a matriz de custo que usamos é espelhada.
    def getAdj(self, u):
        adj = []
        for v in range(self.V):
            if self.graph[u][v] > 0:
                adj.append(v)

        return adj
    
    #Também utilizamos esse método para debug do código. Basicamente limpa o objeto restaurando os valores iniciais
    def cleanObj(self):
        self.key = [float("inf")] * self.V 
        self.key[0] = 0 
        self.parent = [None] * self.V 
        self.parent[0] = -1 
        self.mstSet = [False] * self.V

    #É o método principal. Dado um grafo de entrada, achamos a árvore geradora mínima
    #O algoritmo é descrito nos próximos comentários
    def primMST(self):
        #O programa termina só quando todas as arestas estiverem na MST. Podemos fazer essa verificação de duas formas:
        #Ou percorrendo o mstSet e verificando se todas as posições estiverem igual a True
        #Ou simplesmente iterando sob a quantidade de vértices. Porque só inserimos 1 vértice na MST a cada iteração.
        for cout in range(self.V):  #no nosso caso usamos a segunda oção
            u = self.minKey()       #Retiramos o vértice com menor chave
            self.mstSet[u] = True   #Inserimos ele na MST
  
            for v in range(self.V): #Para cada vértice "v" 
                #se "v" é vizinho de "u" (se tem aresta de "u" para "v") E se "v" não está na MST E se a aresta de "u" para "v" for menor que a chave de "v"
                if self.graph[u][v] > 0 and self.mstSet[v] == False and self.key[v] > self.graph[u][v]:
                    #atualizamos a chave e o pai de "v"
                    self.key[v] = self.graph[u][v] 
                    self.parent[v] = u 
                    
        #Após os 2 "fors" temo a nossa MST
        #self.printMST()
        print(self.totalCost()) #Calcula o custo da MST
        #self.printGraph()
    
#Retiramos o nome do programa da lista de argumentos para termos uma lista de numero de vértices
args = sys.argv[1:]

#Abrimos um arquivo para escrita
with open('seq_python.csv', 'w', newline='') as file:
    writer = csv.writer(file) #instanciamos o objeto que irá escrever nesse arquivo
    writer.writerow(["n", "Tempo de Execução"]) #A primeira linha da tabela é o título das colunas
    for arg in args: #Para cada nro de vértices dado como entrada
        if(not arg.isdecimal()): #se enviou algum caracter ou algum float em vez de um número inteiro de vértices
            print("Tente novamente! Argumento " + arg + " invalido") #da erro e termina o programa
            break
        else: #Caso contrário
            g = Graph(int(arg)) #instanciamos o grafo com o número de vértices dado na lista
            g.generateGraph()   #geramos a matriz de custo que irá representar o grafo
            ini = time()        
            g.primMST()         #Chamamos o método que dado a matriz de custo, gera a MST
            fim = time()        

            writer.writerow([arg, fim-ini]) #anotamos no arquivo o nro de vértices e o tempo que custou calcular o MST dado esse nro de vértices