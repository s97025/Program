# 演算法分析機測
# 學號：10927202/10927207/10927234
# 姓名：陽彩柔/蒲品憶/吳凱鈺
# 中原大學資訊工程學系

import time
import numpy as np
import collections

box_x, box_y, person_x, person_y, target_x, target_y = 0, 0, 0, 0, 0, 0
row, col = 0, 0
direction = [[-1, 0], [1, 0], [0, -1], [0, 1]]
op = ['n', 's', 'w', 'e']
mark = [[[]]] # Marks whether the position around the box has been walked.
visited = [[]] # Record the location where people walked.

class Box:
    def __init__(self):
        self.x = 0
        self.y = 0
        self.PersonX = 0  # PersonX,PersonY 表示箱子固定，人所在的位置
        self.PersonY = 0
        self.ans = ""
box_now = Box()

class Person:
    def __init__(self):
        self.x = 0
        self.y = 0
        self.ans = ""
person_now = Person()

# Convert lowercase letter to uppercase.
def ChangeToUpper(c): # ord() return the ascii of the char.
    return chr(ord(c) - ord('a') + ord('A'))

# Determine whether the current person can reach the position
# specified by the  box.
def PersonFindBox(person_target_x, person_target_y, box_now_x, box_now_y, box_now_PX, box_now_PY, Map):
    global person_now
    person = collections.deque()
    if person_target_x < 0 or person_target_x >= row or person_target_y < 0 or person_target_y >= col or Map[person_target_x][person_target_y] == '#':
        return False
    person_now.x = box_now_PX
    person_now.y = box_now_PY
    person_now.ans = ""

    visited = np.zeros((row, col), dtype=int)  # Record the location people walk
    visited[box_now_x][box_now_y] = 1  # Can't go through the box.
    visited[box_now_PX][box_now_PY] = 1  # Starting point.
    person.append(person_now)
    while len(person) != 0:
        person_now = person.popleft()
        if person_now.x == person_target_x and person_now.y == person_target_y: # each the position specified by the box.
            return True
        for i in range(4): # Walk to find box
            person_tmp = Person()
            person_tmp.x = person_now.x + direction[i][0]
            person_tmp.y = person_now.y + direction[i][1]
            if (
                person_tmp.x >= 0
                and person_tmp.x < row
                and person_tmp.y >= 0
                and person_tmp.y < col
                and not visited[person_tmp.x][person_tmp.y]
                and Map[person_tmp.x][person_tmp.y] != "#"
            ): 
                person_tmp.ans = person_now.ans + op[i]  # Record the path.
                visited[person_tmp.x][person_tmp.y] = 1
                person.append(person_tmp)
    return False

# Determine whether the current person can push the box to reach the destination
def GoToTarget(map):
    global mark, box_now
    box = collections.deque()
    box_now.x = box_x
    box_now.y = box_y
    box_now.ans = ""
    box_now.PersonX = person_x
    box_now.PersonY = person_y

    box.append(box_now)
    while len(box) != 0:
        box_now = box.popleft()
        if box_now.x == target_x and box_now.y == target_y: # The box reaches the target.
            return True
    
        for i in range(4): # Push the box to the target
            box_tmp = Box()
            box_tmp.x = box_now.x + direction[i][0]
            box_tmp.y = box_now.y + direction[i][1]

            if (
                box_tmp.x >= 0 
                and box_tmp.x < row 
                and box_tmp.y >= 0 
                and box_tmp.y < col 
                and not mark[box_tmp.x][box_tmp.y][i] 
                and Map[box_tmp.x][box_tmp.y] != "#"):

                if PersonFindBox(box_tmp.x-2*direction[i][0], box_tmp.y-2*direction[i][1], box_now.x, box_now.y, box_now.PersonX, box_now.PersonY, map):
                    box_tmp.PersonX = box_now.x
                    box_tmp.PersonY = box_now.y
                    box_tmp.ans = box_now.ans + person_now.ans + ChangeToUpper(op[i])
                    mark[box_tmp.x][box_tmp.y][i] = True
                    box.append(box_tmp)
                    
    return False

if __name__ == '__main__':
    print("\n***** Pushing Box Game Problem *****")
    end = False
    num = 0
    while not end:
        row, col = map(int, input("Input Row and Column: ").split())
        if row == 0 and col == 0:
            end = True
        
        else:
            num += 1
            mark = np.zeros((row, col, 4), dtype=bool)
            visited = np.zeros((row, col), dtype=int)
            Map = np.zeros((row, col), dtype=str) # Create an 2D array,size is row*col
            print("Input Map: ")
            for i in range(row):
                line = input()
                for j in range(col):
                    Map[i][j] = line[j]
                    if Map[i][j] == 'S':
                        person_x = i
                        person_y = j
                    elif Map[i][j] == 'T':
                        target_x = i
                        target_y = j
                    elif Map[i][j] == 'B':
                        box_x = i
                        box_y = j
    
            print("\nMaze #", num)
            start_time = time.time()
            if GoToTarget(Map):
                print(box_now.ans)
            else:
                print("Impossible.\n")
            total_time = time.time() - start_time
            print("Total time: ", total_time, '\n')
