# 演算法分析機測
# 學號：10927202/10927207/10927234
# 姓名：陽彩柔/蒲品憶/吳凱鈺
# 中原大學資訊工程學系

import time

def LongestIncreasingSubsequence( input ):

  if not input:
    return []

  # Store LCS in LIS[i]
  LIS = [[] for _ in range(len(input))]
  
  LIS[0].append(input[0])

  # 從列表中的第二個元素開始
  for i in range(1, len(input)):

    for j in range(i):
      if input[j] < input[i] and len(LIS[j]) >= len(LIS[i]):
        LIS[i] = LIS[j].copy() # Copy contents in LIS[j] to LIS[i]

    # 在 `LIS[i]` 中包含 `list[i]`
    LIS[i].append(input[i])

  # Find LCS
  j = 0
  for i in range(len(input)):
    if len(LIS[j]) < len(LIS[i]):
      j = i

  return LIS[j], len(LIS[j])


if __name__ == '__main__':
  end = False
  case = 0
  while not end :
    case += 1
    print( "\n***** Longest Increasing Subsequence Problem *****" )
    how_many = int( input( "Input number: " ) )
    start_time = time.time()
    if ( how_many == 0 ): 
      print( "**************** End This Problem ****************" )
      end = True
    else :
      A = list(map(int, input("Input Data: ").split()))
      ans, sum = LongestIncreasingSubsequence( A )
      total_time = time.time() - start_time
      print( "Case " + str(case) )
      print( "Length of LIS = ", sum )
      print( "LIS = ", end = '' )
      for i in range( len(ans) ):
        if i == len(ans) - 1 :
          print( str(ans[i]) )
        else: print( str(ans[i]) + ", ", end = '' )

      print("Total time: ", total_time)
