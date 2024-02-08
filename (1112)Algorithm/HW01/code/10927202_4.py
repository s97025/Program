# 演算法分析機測
# 學號：10927202/10927207/10927234
# 姓名：陽彩柔/蒲品憶/吳凱鈺
# 中原大學資訊工程學系

import time

friends = []
visited = []

def FindGroups(cur_people):
  visited[cur_people] = True

  while not visited[friends[cur_people]]: # Find and visit friend
    cur_people = friends[cur_people]
    visited[cur_people] = True

if __name__ == '__main__':
  print("***** Samll Groups Problem *****")
  start_time = time.time()
  how_many = int(input("Input how many people: "))

  while (how_many != 0) :
   # how_many = int(input("Input how many people: "))
    friends = [int(x) for x in input("Input friends: ").split()] # Read friends
    visited = [False]*how_many # Record whether to visit
    group_num = 0 # Record how many groups
    for i in range(how_many):
      if not visited[i]:
        FindGroups(i)
        group_num += 1

    print("Number of groups: ", group_num)
    how_many = int(input("Input how many people: "))

  total_time = time.time() - start_time
  print("Total time: ", total_time)