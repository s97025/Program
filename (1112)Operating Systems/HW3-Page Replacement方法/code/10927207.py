
import os

que = []
datas = ""
output = []
page_fualts = 0
page_replaces = 0 
frame = 0 

class Info():
  def __init__(self):
    process = ""
    count = 0 

def Read():
    global datas, frame
    
    cmd = ""
    name = input("請輸入檔案名稱(e.g input1, quit[0])：\n")

    if name == "0":
        cmd = "0"
    elif not os.path.exists(name + ".txt"):
        print("沒有此檔案!!!")    
    else:        
        with open(name + ".txt", "r") as file:
            cmd, frame = file.readline().split() 
            tmp = file.read().splitlines()
            datas = tmp[0] 

    return name, cmd

def Writemethod(cmd):
    global interface
    if cmd == "1": 
        interface = "--------------FIFO-----------------------\n"
    elif cmd == "2":
        interface = "--------------LRU-----------------------\n"
    elif cmd == "3":
        interface = "--------------Least Frequently Used Page Replacement-----------------------\n"
    elif cmd == "4":
        interface = "--------------Most Frequently Used Page Replacement -----------------------\n"
    elif cmd == "5":
        interface = "--------------Least Frequently Used LRU Page Replacement-----------------------\n"
    return interface 

def Write( cmd, name ):
  global output, page_fualts, page_replaces, frame 
  nameOutput = "out_" + name + ".txt"

  with open(nameOutput , "w") as file:  
    file.write(Writemethod(cmd)) 
    for i in range(len(output)): 
      file.write(output[i][0]) 
      file.write('\t') 

      file.write(output[i][1]) 

      if ( output[i][2] == 'F' ):
        file.write('\t')  
        file.write(output[i][2]) 
      file.write('\n')

    file.write("Page Fault = " + str(page_fualts) + "  Page Replaces = " + str(page_replaces) + "  Page Frames = " + frame + '\n' )
    
def WriteA( cmd, name ):
  global output, page_fualts, page_replaces, frame 
  nameOutput = "out_" + name + ".txt"

  with open(nameOutput , "a") as file:  
    file.write("\n")
    file.write(Writemethod(cmd)) 
    for i in range(len(output)): 
      file.write(output[i][0]) 
      file.write('\t') 

      file.write(output[i][1]) 

      if ( output[i][2] == 'F' ):
        file.write('\t')  
        file.write(output[i][2]) 
      file.write('\n')

    file.write("Page Fault = " + str(page_fualts) + "  Page Replaces = " + str(page_replaces) + "  Page Frames = " + str(frame) + '\n' )
        
def fault( process ):
  global que
  index = 0 
  while index < len(que) :
    if process == que[index].process :
      return False, index
    
    index += 1 

  return True, index-1

def Replace( info, index ):
  global que
  que.pop( index )
  que.insert(0 , info )

def SaveSingleOutput( process, IsFault ):
    global que
    quelist = ""
    tmp = []  
    tmp.append( process )  

    for i in range( len(que) ):
      quelist = quelist + que[i].process

    tmp.append( quelist )

    if not IsFault :
       tmp.append('')  
    else:
        tmp.append('F')  

    output.append( tmp )
    #print(tmp)
    
def FIFO(name):
  global datas, que, page_fualts, page_replaces, frame 

  IsFault = False

  for i in range(len(datas)): 
    info = Info()
    info.process = datas[i]
    info.count = 0

    if len(que) == 0:
      IsFault = True
      page_fualts += 1
      que.insert( 0 , info )
    else:
      IsFault, index = fault( datas[i] )
      if not IsFault :
        pass
        
      else:
        page_fualts += 1
        if len(que) < int(frame) :
            que.insert( 0 ,info )
        else:
            page_replaces += 1 
            Replace( info, index )


    SaveSingleOutput( datas[i], IsFault )
  
  #Write( "1", name )

def LRU(name):
  global datas, que, page_fualts, page_replaces, frame 

  IsFault = False

  for i in range(len(datas)): 
    info = Info()
    info.process = datas[i]
    info.count = 0  

    if len(que) == 0:
      IsFault = True
      page_fualts += 1
      que.insert( 0 , info )
    else:
      IsFault, index = fault( datas[i] )
      if not IsFault :
        que.insert( 0 , que.pop(index) ) 
        
      else:
        page_fualts += 1
        if len(que) < int(frame) :
            que.insert( 0 , info )
        else:
            page_replaces += 1 
            Replace( info, index )

    SaveSingleOutput( datas[i], IsFault )
  
  #Write( "2", name )  

def FindMin():
  global que
  index = 0 
  min = 0
  for i in range( len(que) ):
    if ( i == 0 ):
      index = 0
      min = que[i].count
    elif ( que[i].count <= min ):
      index = i
      min = que[i].count

  return index

def LFU_FIFO(name):
  global datas, que, page_fualts, page_replaces, frame 

  IsFault = False

  for i in range(len(datas)): 
    info = Info()
    info.process = datas[i]
    info.count = 1

    if len(que) == 0:
      IsFault = True
      page_fualts += 1
      que.insert( 0 , info )
    else:
      IsFault, index = fault( datas[i] )
      if not IsFault :
        que[index].count += 1 
        
      else:
        page_fualts += 1
        if len(que) < int(frame) :
            que.insert( 0 , info )
        else:
            page_replaces += 1 
            index = FindMin()
            Replace( info, index )

    SaveSingleOutput( datas[i], IsFault )

  #Write( "3", name ) 

def FindMax():
  global que
  index = 0 
  max = 0
  for i in range( len(que) ):
    if ( i == 0 ):
      index = 0
      max = que[i].count
    elif ( que[i].count >= max ):
      index = i
      max = que[i].count

  return index

def MFU_FIFO(name):
  global datas, que, page_fualts, page_replaces, frame 

  IsFault = False

  for i in range(len(datas)): 
    info = Info()
    info.process = datas[i]
    info.count = 1

    if len(que) == 0:
      IsFault = True
      page_fualts += 1
      que.insert( 0 , info )
    else:
      IsFault, index = fault( datas[i] )
      if not IsFault :
        que[index].count += 1 
        
      else:
        page_fualts += 1
        if len(que) < int(frame) :
            que.insert( 0 , info )
        else:
            page_replaces += 1 
            index = FindMax()
            Replace( info, index )

    SaveSingleOutput( datas[i], IsFault )

  #Write( "4", name ) 

def LFU_LRU(name):
  global datas, que, page_fualts, page_replaces, frame 

  IsFault = False

  for i in range(len(datas)): 
    info = Info()
    info.process = datas[i]
    info.count = 1

    if len(que) == 0:
      IsFault = True
      page_fualts += 1
      que.insert( 0 , info )
    else:
      IsFault, index = fault( datas[i] )
      if not IsFault :
        que[index].count += 1 
        que.insert( 0 , que.pop(index) )       
      else:
        page_fualts += 1
        if len(que) < int(frame) :
            que.insert( 0 , info )
        else:
            page_replaces += 1 
            index = FindMin()
            Replace( info, index )

    SaveSingleOutput( datas[i], IsFault )

  #Write( "5", name ) 

def ClearGlobalVar():
    global que, output, page_fualts, page_replaces
    que.clear()
    output.clear()
    page_fualts = 0
    page_replaces = 0 

def DoAll(name):
  FIFO(name)
  Write( "1", name )
  ClearGlobalVar()

  LRU(name)
  WriteA( "2", name )
  ClearGlobalVar()

  LFU_FIFO(name)
  WriteA( "3", name )
  ClearGlobalVar()

  MFU_FIFO(name)
  WriteA( "4", name )
  ClearGlobalVar()

  LFU_LRU(name)
  WriteA( "5", name )
  ClearGlobalVar()
   
def main():
  global datas, frame 

  end = False 
  while end == False:

    datas = ""
    frame = 0
    ClearGlobalVar()
    
    name, cmd = Read()

    if cmd == "0":
      end = True 
    elif cmd != "0" and os.path.exists(name + ".txt"):
      if cmd == "1": 
        FIFO(name)   
        Write( "1", name )  
      elif cmd == "2":
        LRU(name)       
        Write( "2", name )  
      elif cmd == "3":
        LFU_FIFO(name) 
        Write( "3", name )      
      elif cmd == "4":
        MFU_FIFO(name)  
        Write( "4", name )       
      elif cmd == "5":
        LFU_LRU(name)  
        Write( "5", name )       
      elif cmd == "6":
        DoAll(name)


if __name__  == '__main__':
  main()

