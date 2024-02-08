# 演算法分析機測
# 學號：10927202/10927207/10927234
# 姓名：陽彩柔/蒲品憶/吳凱鈺
# 中原大學資訊工程學系

def count_height( tree, index, height ) :
  degree = int(tree[index][0])
  if degree == 0 :
    return height
  else :
    height += 1
    h = 0
    max = 0
    for i in range( 1, degree+1 ) :
      j = int(tree[index][i]) - 1
      h = count_height( tree, j, h )
      if h > max :
        max = h
      h = 0

    return height + max
           

def Build_Tree( forest ) :
  for i in range( len(forest) ) :
    print( "\nRooted Tree ", i+1 )
    node_height = []
    for index in range( len(forest[i]) ) : # j + 1 : the number of the node
      height = 0
      height = count_height( forest[i], index, height )
      node_height.append( height )
    # print( node_height )

    max = 0
    index = 0
    total = 0
    for j in range( len(node_height) ) :
      if max < node_height[j] :
        max = node_height[j]
        index = j
      total = total + node_height[j]
    print( index+1 )
    print( total )

def Read_Txt() :
  forest = []
  run = True
  while ( run ) :
    print( "\n***** Rooted Tree Problem *****" )
    node_num = int(input('How many node: '))
    if ( node_num == 0 ) :
      run = False
      print( "******* End This Problem ******" )
      
    else :
      tree_node = []
      i = 0
      while i < node_num :
        node = input('Input node ' + str(i+1) + ' Child\'s Num and Node: ')
        arr = node.split(' ')
        tree_node.append( arr )
        i += 1
      forest.append( tree_node )

      Build_Tree( forest )

if __name__ == '__main__':
  Read_Txt()