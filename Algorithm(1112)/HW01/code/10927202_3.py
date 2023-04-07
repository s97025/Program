# 演算法分析機測
# 學號：10927202/10927207/10927234
# 姓名：陽彩柔/蒲品憶/吳凱鈺
# 中原大學資訊工程學系

import time

Parent_Children = {}  # 存放父子節點
Status_list = [] # 表示方式 [WS, WW, ES, EW, B] 西羊、西狼、東羊、東狼、船
route = []  # 一次路徑
way = []  # 總路徑
actions = [[1, 0], [0, 1], [1, 1], [0, 2], [2, 0]]  # 五種操作方式--船的載運方式
Sheeps = 0
Wolfs = 0

# s[0] and s[1] are sheeps and wols in west
# s[2] and s[3] are sheeps and wolfs inf east
def result(s):
    if s[0] < 0 or s[1] < 0 or s[2] < 0 or s[3] < 0:  # 負數
        return
    if (s[0] < s[1] and s[0] != 0) or (s[2] < s[3] and s[2] != 0):  # 羊被吃
        return

    d = tuple(s)    # 元组作為狀態點
    if len(Status_list) > 1: # 建立關係圖
        f = tuple(Status_list[-2][:]) # 取Status_list陣列倒數第二項
        if f in Parent_Children.keys() and d not in Parent_Children[f]:
            Parent_Children[f].append(d)
        else:
            Parent_Children[f] = [d]


    for k in Status_list[:-1]: # (排除)重復狀態  Status_list[:-1]->取Status_list的最後一項之前的數
        if k[0] == s[0] and k[1] == s[1] and k[2] == s[2] and k[3] == s[3] and k[4] == s[4]:
            return

    mid = [0] * 5 # 建一個五格陣列 放移動後值: 西羊、西狼、東羊、東狼、船

    for j in actions: # 套入五種操作方式
        mid[0] = s[0] - j[0] * s[4] # (j[0] * s[4]) 往東西邊船上的羊
        mid[1] = s[1] - j[1] * s[4]
        mid[2] = s[2] + j[0] * s[4]
        mid[3] = s[3] + j[1] * s[4]
        mid[4] = -s[4]
        Status_list.append(mid[:]) # 更新目前狀態
        result(mid)   # 判斷該動作是否合理，並加入字典-->遞迴
        Status_list.pop()

    return

# 深度搜索尋找路徑
def dfs(s):
    s = tuple(s)     # 字典的值是列表里面包含元组，原因是字典的key不能是列表
    if s in route:   # 已經在路徑裡面
        route.append(s)
        return

    # 到達終點，紀錄路徑
    if s == (0, 0, Sheeps, Wolfs, -1):
        route.append(s)
        if len(way) > 0:
            if ( len(route) < len(way[0]) ): 
                way.clear()   
                way.append(route[:])
            elif len(route) == len(way[0]) :
                way.append(route[:])

        else:
            way.append(route[:])
        return

    route.append(s)
    for i in range(len(Parent_Children[s])):
        dfs(Parent_Children[s][i])   # 遞迴搜索直到到達終點或者重複
        route.pop()

def Clear():
    Parent_Children.clear()  
    Status_list.clear() 
    route.clear() 
    way.clear()

if __name__ == '__main__':
    end = False
    print("\n***** River Crossing Problem *****")
    while not end :
        Clear()
        Wolfs, Sheeps = map(int, input("Input Wolfs and Sheeps: ").split())
        start_time = time.time() 
        if Wolfs==0 and Sheeps==0: 
            end = True      
        elif Wolfs > Sheeps:
            print("羊被吃了無法渡河!!!")    
        else:
            start = [Sheeps, Wolfs, 0, 0, 1]  # 西羊、西狼、東羊、東狼、船
            Status_list.append(start) # 目前狀態
            result(start)
            dfs(start)
            num = 0   # 統計次數
            lessRoad = 0 # 紀錄最短路徑長度/在最短路way的位置

            if ( len(way)!=0 ):
                '''
                for k in way:      
                    num += 1
                    print("(西狼,西羊,船)")   
                    for i in k:
                        print("({},{}".format(i[1], i[0]), end="")
                        if ( i[4] == 1 ):
                            print(",W)")
                        elif ( i[4] == -1 ):
                            print(",E)")   
                '''
                for i in way[0]:
                    print("({},{}".format(i[1], i[0]), end="")
                    if ( i[4] == 1 ):
                        print(",W)")
                    elif ( i[4] == -1 ):
                        print(",E)")           
                print("路徑長度: ", len(way[0]))                
                print("共有幾種最短路徑可能: ", len(way) ) 

            else: 
                print("無解!") 
            total_time = time.time() - start_time
            print("Total time: ", total_time)         
