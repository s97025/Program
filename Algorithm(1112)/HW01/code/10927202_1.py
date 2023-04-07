# 演算法分析機測
# 學號：10927202/10927207/10927234
# 姓名：陽彩柔/蒲品憶/吳凱鈺
# 中原大學資訊工程學系

import time

def FindMaxCrossSumArray(A, low, mid, high):
  left_Sum = -3000000
  sum = 0
  for i in range(mid, low-1, -1):
    sum += A[i]
    if (sum > left_Sum):
      left_Sum = sum
      max_left = i

  right_Sum = -3000000
  sum = 0
  for j in range(mid+1, high+1):
    sum += A[j]
    if (sum > right_Sum):
      right_Sum = sum
      max_right = j

  return(max_left, max_right, left_Sum+right_Sum)

def FindMaximumSubArray(A, low, high):
  if (high == low):
    return (low, high, A[low])
  else:
    mid = (low+high)//2
    left_low, left_high, left_sum = FindMaximumSubArray(A, low, mid)
    right_low, right_high, right_sum = FindMaximumSubArray(A, mid+1, high)
    cross_low, cross_high, cross_sum = FindMaxCrossSumArray(A, low, mid, high)
    
    if (left_sum >= right_sum and left_sum >= cross_sum):
      return(left_low, left_high, left_sum)
    elif(right_sum >= left_sum and right_sum >= cross_sum):
      return(right_low, right_high, right_sum)
    else:
      return(cross_low, cross_high, cross_sum)
    
if __name__ == '__main__':
  print("***** Maximum-Subarray Problem *****")
  end = False
  while not end :
    how_many = int(input("Input number: "))
    start_time = time.time()
    if (how_many == 0): end = True
    else :
      A = list(map(int, input("Input Data: ").split()))
      (low, high, sum) = FindMaximumSubArray(A, 0, how_many-1)
      print("Low = ", low+1, " High = ", high+1, " Sum = ", sum)
      total_time = time.time() - start_time
      print("Total time: ", total_time)