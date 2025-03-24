# 演算法分析機測
# 學號：10927202/10927207/10927234
# 姓名：陽彩柔/蒲品憶/吳凱鈺
# 中原大學資訊工程學系

import queue
 
class Node:
  def __init__(self, x, y, dist=0, visit=False):
    self.x = x
    self.y = y
    self.dist = dist
    self.visited = visit
 
# Where Knight can move to.
row = [-1, -2, -2, -1, 1, 2, 2, 1]
col = [-2, -1, 1, 2, 2, 1, -1, -2]
 
def isValid(x, y):
  if (x < ord('a') or y < 0 or x > ord('h') or y > 8): return False
  else: return True

def FindShortestDistance(data, dest):
  q = queue.Queue()
  q.put(data)

  while not q.empty():
    node = q.get()
    x = node.x
    y = node.y
    dist = node.dist

    # Go to the destination return the distence
    if x == dest.x and y == dest.y:
      return dist, True

    if not node.visited:
      node.visited = True # Set node as visited
        
      for i in range( len(row) ): # Put the valid node into queue
        x1 = x + row[i]
        y1 = y + col[i]

        if isValid(x1, y1):
          q.put(Node(x1, y1, dist + 1))

  return 0, False # Can't find the way to destination
    
if __name__ == '__main__':
  end = False

  while not end :
    print( "\n***** Chess Knight Problem *****" )
    start, destination = map(str, input("Input Starting point and Destination: ").split())
    if start == '0' and destination == '0' :
      end = True
      print( "******* End This Problem *******" )
    else :
      start_node = Node( ord(start[0]), int(start[1])-1 )
      destination_node = Node( ord(destination[0]), int(destination[1])-1 )
      moves, is_ok = FindShortestDistance( start_node, destination_node )
      if is_ok: print( "From " + start + " to " + destination + ", Knight Moves = " + str(moves)) 
      else: print( "Can't find the way!" ) 
