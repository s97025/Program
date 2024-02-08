# 演算法分析機測
# 學號：10927202/10927207/10927234
# 姓名：陽彩柔/蒲品憶/吳凱鈺
# 中原大學資訊工程學系

INFINITY = 1e30


import numpy as np


def Cal_num(MST, Map):  
    
  # "Minimum Spanning Tree:" 
  for i in range( len(Map) ):
    count = 0
    for j in range( len(Map) ):
      if i != j :
        if Map[i][j] == '-' :
            MST[i][j] = 0
        else:
          MST[i][j] = -1
          count += 1

    MST[i][i] = count
    
  '''
  print( "Minimum Spanning Tree:" )
  for i in range( len(MST) ):
    for j in range( len(MST)  ):
      print( MST[i][j], end = " " )
    print()
  '''

  MST.pop(0)
  for i in range (len(MST)):
    MST[i].pop(0)

  matrix = np.asarray(MST)
  ans = np.linalg.det(matrix)

  # print( "Minimum Spanning Tree:\n", int(float(" %1f " % ans)) )
  return int(float(" %1f " % ans))


def Prim( source, n_vertices, Map ):
  
  parent = [ 0 for i in range( n_vertices ) ] # 找父節點 紀錄ndex
  key = [ INFINITY for i in range( n_vertices ) ] # 紀錄目前每個點的最小權重
  set = [ False for i in range( n_vertices ) ] # 未被訪問
  MST = [ [ 0 for j in range( n_vertices ) ] for i in range( n_vertices ) ]

  key[source]	= 0
  parent[source] = 0
  set[source] = False

  # print( "Prim's Algorithm (MST Sequence)" )
  for i in range( 0, n_vertices ):
    min = INFINITY
    min_idx = 0
    for j in range( 0, n_vertices ):
      if ( set[j] == False and key[j] < min ):
        min = key[j]
        min_idx = j 

    set[min_idx] = True # find min 在第幾個index

    for j in range( 0, n_vertices ):
      if ( ( Map[min_idx][j] != '0' and Map[min_idx][j] != '-' ) ):
        if ( set[j] == False and int(Map[min_idx][j]) < key[j] ):
          parent[j] = min_idx
          key[j] = int(Map[min_idx][j])

  num = Cal_num(MST, Map)    

  cost = 0 
  for i in range( len(key) ):
    cost = cost + key[i]
  
  return cost, num



if __name__ == '__main__':
  end = False

  while not end :
    print( "\n***** Minimum Spanning Trees Problem *****" )
    vertices = int( input( "Input how many vertices: " ) )
    
    if vertices == 0 :
      print( "************ End This Problem ************" )
      end = True
    else:
      Map = []
      for i in range(vertices):
        A = list(input().split())
        Map.append(A)

      cost, num = Prim( 0 , vertices, Map ) 
      print( "Number of Spanning Trees = " + str( num ) )
      print( "Minimum Cost = " + str(cost) )


