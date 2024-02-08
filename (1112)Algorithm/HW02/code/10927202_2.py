# 演算法分析機測
# 學號：10927202/10927207/10927234
# 姓名：陽彩柔/蒲品憶/吳凱鈺
# 中原大學資訊工程學系

class Item():
  def __init__(self):
    self.weight = 0
    self.value = 0

def Knapsack( data, total_weight, how_many ):
  items = []
  K = [ [ 0 for w in range( total_weight + 1 ) ] for i in range( how_many + 1 ) ]

  for w in range( total_weight + 1):
    K[0][w] = 0
  
  for i in range( how_many + 1):
    K[i][0] = 0
    for w in range( total_weight + 1 ) :
      if i == 0 or w == 0:
        K[i][w] = 0
      elif data[i-1].weight > w:
        K[i][w] = K[i-1][w]
      elif data[i-1].weight <= w:
        K[i][w] = max( data[i-1].value + K[i-1][w-data[i-1].weight], K[i-1][w])

  total_value = K[how_many][total_weight]
  w = total_weight

  for i in range(how_many, 0, -1):
    if total_value <= 0:
      break
    if total_value == K[i-1][w]:
      continue
    else:
      items.append(i)
      total_value -= data[i-1].value
      w -= data[i-1].weight

  items.sort( reverse = False )
  return K[how_many][total_weight], items

if __name__ == '__main__':
  end = False

  while not end :
    print( "\n***** 0-1 Knapsack Problem *****" )
    weight = int( input( "Input weight: " ) )
    how_many = int( input( "Input num of item: " ) )
    A = []
    if ( weight == 0 and how_many == 0 ) :
      print( "******* End This Problem *******" )
      end = True
    else:
      for i in range(how_many):
        items = Item()
        data = list(map(int, input("Input weight and value: ").split()))
        items.value = data[1]
        items.weight = data[0]
        A.append( items )

      value, items = Knapsack( A, weight, how_many )
      print( "Total Value = ", str(value) )
      print( "Take Items ", end = '')
      for i in range( len(items) ):
        if i == len(items) - 1 :
          print( items[i] )
        else:
          print( str(items[i]) + ', ' , end = '')