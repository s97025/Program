import os
import time
import threading
import concurrent.futures
from datetime import datetime,timezone,timedelta
from multiprocessing import Process, Manager



bubbleResult = []
output = []

def BubbleSort(lst, result):
    length = len(lst)
    while length > 1:
        length-=1 
        for i in range(length):
            if lst[i] > lst[i+1]:
                lst[i], lst[i+1] = lst[i+1], lst[i]       

    result.append(lst)

def MergeSort(left, right, lst):
    result = []
    while len(left) != 0 and len(right) != 0:
        if left[0] < right[0]:
            result.append(left.pop(0))
        else:
            result.append(right.pop(0))

    if len(left) != 0 :
        result = result+left 
    elif len(right) != 0  :
        result = result+right

    lst.append( result )   

def DoTwoSort(lst, result):
    global bubbleResult
    for i in range(len(lst)):
        BubbleSort(lst[i], bubbleResult)       
    while len(bubbleResult) > 1 :
        MergeSort(bubbleResult.pop(0), bubbleResult.pop(0), bubbleResult)

    result.append(bubbleResult[0])
    bubbleResult.clear()

def DoMethod1(lst):
    global bubbleResult, output
    BubbleSort(lst, bubbleResult)
    output = bubbleResult[0]
    bubbleResult.clear()

def DoMethod2(lst):
    global output
    lsttmp = Manager().list()
    p = Process(target=DoTwoSort, args=(lst, lsttmp))        
    p.start()
    p.join()

    output = lsttmp[0]

def DoMethod3(lst, split):    
    global output
    lsttmp = Manager().list()
    processes1 = []  
    processes2 = [] 

    for i in range(split) : # BubbleSort 
        p = Process(target=BubbleSort, args=(lst[i], lsttmp))     
        processes1.append(p)   
        processes1[i].start()    
    for i in range(len(processes1)) : # wait processes end
        processes1[i].join() 
    processes1.clear()

 
    while len(lsttmp) > 1: # merge
        size = len(lsttmp) // 2        
        for i in range(size): 
            p = Process(target=MergeSort, args=(lsttmp.pop(0), lsttmp.pop(0), lsttmp) )
            processes2.append(p)
            processes2[i].start()
        for i in range(len(processes2)): # wait processes end
            processes2[i].join()         
        processes2.clear()

    output = lsttmp[0]  

def DoMethod4(lst, split):
    global output
    lsttmp = []
    threads1 = []  
    threads2 = [] 

    for i in range(split) : # BubbleSort       
        p = threading.Thread(target=BubbleSort, args=(lst[i], lsttmp))     
        threads1.append(p)   
        threads1[i].start()
    for i in range(len(threads1)): # wait threads end
        threads1[i].join() 
    threads1.clear()

    while len(lsttmp) > 1: # merge  
        size = len(lsttmp) // 2        
        for i in range(size): 
            p = threading.Thread(target=MergeSort, args=(lsttmp.pop(0), lsttmp.pop(0), lsttmp) )
            threads2.append(p)
            threads2[i].start()
        for i in range(len(threads2)): # wait threads end
            threads2[i].join()
        threads2.clear()

    output = lsttmp[0]  
    lsttmp.clear()

def Spilt(lst):

    split = ""
    datas = []
    remainList = []
    correct = False
    while not correct:
        split = input("請輸入要切成幾份[1~陣列長度]:\n")
        if split.isdigit():
            split = int(split)
            if len(lst) < split or 0 == split : 
                print("輸入錯誤，請重新輸入!!!")    
            elif len(lst) >= split: 
                correct = True

        else:
            print("輸入錯誤，請重新輸入!!!")
    

    quotient = len(lst) // split # 商數
    remainder = len(lst) % split # 餘數

    if len(lst) != 0 : 
        tmp = lst[:remainder]
        lst = lst[remainder:] 
        remainList = list(tmp) # 一維陣列

    while len(lst) > 0:
        tmp = lst[:quotient]
        lst = lst[quotient:]
        datas.append(tmp)

        if (len(remainList) != 0 and len(datas) >= (split-remainder) ):
            datas[len(datas)-1].append(remainList.pop(0))


    remainList.clear()
    return datas, split  # 二維

def Cmd():
    cmd = ""
    correct = False
    while not correct:
        cmd = input("請輸入方法名稱(0[quit], 1, 2, 3, 4):\n")
        if cmd in ["0", "1", "2", "3", "4"]:
            correct = True
        else:
            print("輸入錯誤，請重新輸入!!!")
    return cmd

def Read():
    end = False
    name = ""
    lst = []
    while not end:
        name = input("請輸入檔案名稱：\n")
        if not os.path.exists(name + ".txt"):
            print("沒有此檔案!!!")
        else:
            with open(name + ".txt", "r") as file:
                lst = [int(line) for line in file if line!= "\n"]
                end = True
    return name, lst

def write(result, cmd, name, time):
    with open(f"{name}_output{cmd}.txt" , "w") as file:
        file.write("Sort : \n")
        for i in range(len(result)) :
            file.write(str(result[i]) + "\n")

        file.write("CPU Time : " + str(time) + "\n" )
        dt1 = datetime.utcnow().replace(tzinfo=timezone.utc) # transfer UTC 
        dt2 = dt1.astimezone(timezone(timedelta(hours=8)))  
        file.write("Output Time : %s"%(dt2) )


def main():

  global bubbleResult, output
  end = False  
  lst = [] 
  output = []
  startTime = 0.0
  endTime = 0.0
  split = 0 
  name = '' 

  while not end:
    cmd = Cmd()
    if cmd == '0':
        end = True
    else:    
        name, lst = Read() 
       
        if cmd == '1':
            startTime = time.time() 
            DoMethod1(lst)
            endTime = time.time()
        
        elif cmd == '2':     
            lst, split = Spilt(lst)  
            startTime = time.time()
            DoMethod2(lst)
            endTime = time.time()     
        
        elif cmd == '3':
            lst, split = Spilt(lst)  
            startTime = time.time()
            DoMethod3(lst, split)
            endTime = time.time() 
        
        elif cmd == '4':
            lst, split = Spilt(lst)        
            startTime = time.time()
            DoMethod4(lst, split)
            endTime = time.time()
            pass

        write(output, cmd, name, endTime - startTime) 

    lst.clear()
    output.clear()
    bubbleResult.clear()
    name = ''
    startTime = 0.0
    endTime = 0.0
    split = 0

if __name__ == '__main__':
    main()