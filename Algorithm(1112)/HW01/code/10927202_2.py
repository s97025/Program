# 演算法分析機測
# 學號：10927202/10927207/10927234
# 姓名：陽彩柔/蒲品憶/吳凱鈺
# 中原大學資訊工程學系

import time
import numpy as np

find = False

# Index is the number of vertexs
def Hamiltonian(Map, index, index_list, i ):
  global find
  for j in range(0, index) : # j is Map's line position
    if not find:
      index_list[i:index+1] = 0 
      pre = index_list[i - 1]
      if (Map[pre-1][j] == 1) and j+1 not in index_list : # Is neighbor with pre
        index_list[i] = j+1
        Hamiltonian(Map, index, index_list, i+1 )
      elif i == vertexs and Map[pre-1][0] == 1 :
        index_list[i] = j+1
        print( "Route:", index_list )
        find = True
        return


if __name__ == '__main__':
  print("***** Hamiltonian Cycle Problem *****")
  
  vertexs, edges = map(int, input("Input Vertexs and Edges: ").split())
  Map = np.zeros((vertexs, vertexs), dtype=int) # Create an 2D array
                                                # size is vertexs*vertexs

  node1, node2 = map(int, input().split())
  while node1 != 0 and node2 != 0:
      Map[node1-1][node2-1] = 1
      Map[node2-1][node1-1] = 1
      node1, node2 = map(int, input().split())

  start_time = time.time()
  index_list = np.zeros(vertexs+1, dtype=int) # Create an 1D array, size is index
  index_list[0] = 1 # Set starting point is 1
  list = Hamiltonian(Map, vertexs, index_list, 1) 
  total_time = time.time() - start_time
  print("Total time: ", total_time)