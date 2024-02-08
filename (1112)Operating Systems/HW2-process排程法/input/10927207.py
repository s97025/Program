import os
import queue
import copy

datas = []
outputALL = []
interface = []
GanttchartALL = []

def Turn(id):
    if id > 0 and id <= 9:
        return str(id)
    else:
        ch = 'A'
        for i in range(10,36):
            if int(id) == i :
                return ch
            else:
                ch = chr(ord(ch) + 1)
    return str(id)

def Read():
    global datas

    tmp = []
    timeSlice = ""
    cmd = ""
    name = ""
    name = input("請輸入檔案名稱(e.g input1, quit[0])：\n")

    if name == "0":
        cmd = "0"
    elif not os.path.exists(name + ".txt"):
        print("沒有此檔案!!!")    
    else:        
        with open(name + ".txt", "r") as file:
            cmd, timeSlice = file.readline().split() 
            tmp = file.readline()
            for line in file:
                if line != "" and line != "\n" and line != "\t":
                    tmp = line.split()
                    tmp = [int(x) for x in tmp]
                    datas.append(tmp)
        datas.sort(key=lambda x: (x[2], x[0])) # sort Arrival PID

    return name, cmd, timeSlice

def Writemethod(cmd):
    global interface
    if cmd == "1": 
        interface = ["FCFS\n", "==        FCFS==\n", "ID	FCFS\n"]
    elif cmd == "2":
        interface = ["RR\n", "==          RR==\n", "ID	RR\n"]
    elif cmd == "3":
        interface = ["SJF\n", "==         SJF==\n", "ID	SJF\n"]
    elif cmd == "4":
        interface = ["SRTF\n", "==        SRTF==\n", "ID	SRTF\n"]
    elif cmd == "5":
        interface = ["HRRN\n", "==        HRRN==\n", "ID	HRRN\n"]
    elif cmd == "6":
        interface = ["Priority RR\n", "==        PPRR==\n", "ID	PPRR\n"]
    elif cmd == "7":
        interface = ["All\n", "==        FCFS==\n", "==          RR==\n", "==         SJF==\n", "==        SRTF==\n", "==        HRRN==\n", "==        PPRR==\n", "ID	FCFS	RR	SJF	SRTF	HRRN	PPRR\n"]

def Write7(name, cmd):
    global  interface, outputALL, GanttchartALL
    Writemethod(cmd)

    nameOutput = "out_" + name + ".txt"
    i = 0 
    with open(nameOutput , "w") as file:
        file.write(interface.pop(0))
        while len(interface) != 0 and i != len(GanttchartALL) :
            file.write(interface.pop(0))
            file.write(GanttchartALL[i] + "\n")
            i = i + 1
        
        file.write("===========================================================\n")
        file.write("\n")
        file.write("Waiting Time\n")
        file.write(interface[0])
        file.write("===========================================================\n")
        i = 0 
        while i < len(outputALL[0]) and outputALL[0][i] != ["\n"] :
            j = 0 
            while j < len(outputALL[0][i]) :
                file.write( str(outputALL[0][i][j]) )
                j = j + 1 
                if j < len(outputALL[0][i]):
                    file.write( "\t" )
                else:
                    file.write( "\n" )
            i = i + 1

        file.write("===========================================================\n")
        file.write("\n")
        file.write("Turnaround Time\n")
        file.write(interface.pop(0))
        file.write("===========================================================\n")        
        i = i + 1
        while i < len(outputALL[0]):
            j = 0 
            while j <  len(outputALL[0][i]):
                file.write( str(outputALL[0][i][j]) )
                j = j + 1 
                if j < len(outputALL[0][i]):
                    file.write( "\t" )
                else:
                    file.write( "\n" )
            i = i + 1
        file.write("===========================================================\n")
        file.write("\n")

def Write(name, cmd):
    global  interface, outputALL, GanttchartALL
    Writemethod(cmd)

    nameOutput = "out_" + name + ".txt"
    with open(nameOutput , "w") as file:
        file.write(interface.pop(0))
        i = 0 
        while len(interface) != 0 and i != len(GanttchartALL) :
            file.write(interface.pop(0))
            file.write(GanttchartALL[i] + "\n")
            i = i + 1
        
        file.write("===========================================================\n")
        file.write("\n")
        file.write("Waiting Time\n")
        file.write(interface[0])
        file.write("===========================================================\n")       
        for wt in outputALL[0]:
            file.write( str(wt[0]) + "\t" + str(wt[2]) + "\n" )

        file.write("===========================================================\n")
        file.write("\n")
        file.write("Turnaround Time\n")
        file.write(interface.pop(0))
        file.write("===========================================================\n")        
        for tat in outputALL[0]:
            file.write( str(tat[0]) + "\t" + str(tat[1]) + "\n" )       
          
        file.write("===========================================================\n")
        file.write("\n")

class FCFS:
    global datas, outputALL, GanttchartALL, GanttchartALL
    Ganttchart = ""
    output = []

    def __init__(self):
        pass

    def Run(self):
        self.output.clear()
        self.Ganttchart = ""
        self.findExit()
        self.CalTime()
        outputALL.append(self.output)
        GanttchartALL.append(self.Ganttchart)

    def CalTime(self):
        for i in range(len(datas)):
            tat = (self.Ganttchart.rfind(Turn(datas[i][0]))+1) - datas[i][2]
            wt = tat - datas[i][1]
            tmp = [ datas[i][0], tat, wt ]
            self.output.append(tmp)

        self.output.sort(key=lambda x: (x[0])) # sort PID 

    def findExit(self):
        end = False
        dIndex = 0
        que = [] 
        cpu = []
        
        datas_d = copy.deepcopy(datas) 

        while end == False:
            while dIndex < len(datas_d) and len(self.Ganttchart) == datas_d[dIndex][2]:
                que.append(datas_d[dIndex])
                dIndex = dIndex + 1
            if len(cpu) == 0:
                if len(que) == 0:
                    self.Ganttchart = self.Ganttchart + "-"
                else:
                    cpu.append(que.pop(0))
            if len(cpu) != 0:       
                cpu[0][1] = cpu[0][1] - 1
                self.Ganttchart = self.Ganttchart + Turn(cpu[0][0])
                if cpu[0][1] == 0:
                    cpu.clear()
            if len(datas) == dIndex and len(cpu) == 0 and len(que) == 0:  
                end = True

class RR:
    global datas, outputALL, GanttchartALL, GanttchartALL
    Ganttchart = ""
    output = []

    def __init__(self, timeSlice):
        self.timeSlice = timeSlice 

    def Run(self):
        self.output.clear()
        self.Ganttchart = ""
        self.findExit()
        self.CalTime()
        outputALL.append(self.output)
        GanttchartALL.append(self.Ganttchart)

    def CalTime(self):
        for i in range(len(datas)):
            tat = (self.Ganttchart.rfind(Turn(datas[i][0]))+1) - datas[i][2]
            wt = tat - datas[i][1]
            tmp = [ datas[i][0], tat, wt ]
            self.output.append(tmp)

        self.output.sort(key=lambda x: (x[0])) # sort PID 

    def findExit(self):
        end = False
        dIndex = 0
        que = [] 
        cpu = []
        tS = int(self.timeSlice)
        datas_d = copy.deepcopy(datas) 

        while end == False:
            while dIndex < len(datas_d) and len(self.Ganttchart) == datas_d[dIndex][2]:
                que.append(datas_d[dIndex])
                dIndex = dIndex + 1
            if len(cpu) == 0:
                if len(que) == 0:
                    self.Ganttchart = self.Ganttchart + "-"
                else:
                    cpu.append(que.pop(0))
                    tS = int(self.timeSlice) 
            if len(cpu) != 0:     
                if len(que) != 0: 
                    if  tS == 0 : # 奪取(Preemptive)  
                        que.append(cpu.pop(0))
                        cpu.append(que.pop(0))
                        tS = int(self.timeSlice) 

                cpu[0][1] = cpu[0][1] - 1
                tS = tS - 1
                self.Ganttchart = self.Ganttchart + Turn(cpu[0][0])
                if cpu[0][1] == 0:
                    cpu.clear()

            if len(datas) == dIndex and len(cpu) == 0 and len(que) == 0:  
                end = True

class SJF:
    global datas, outputALL, GanttchartALL, GanttchartALL
    Ganttchart = ""
    output = []

    def __init__(self):
        pass

    def Run(self):
        self.output.clear()
        self.Ganttchart = ""
        self.findExit()
        self.CalTime()
        outputALL.append(self.output)
        GanttchartALL.append(self.Ganttchart)

    def CalTime(self):
        for i in range(len(datas)):
            tat = (self.Ganttchart.rfind(Turn(datas[i][0]))+1) - datas[i][2]
            wt = tat - datas[i][1]
            tmp = [ datas[i][0], tat, wt ]
            self.output.append(tmp)

        self.output.sort(key=lambda x: (x[0])) # sort PID 

    def findExit(self):
        end = False
        dIndex = 0
        que = [] 
        cpu = []
        
        datas_d = copy.deepcopy(datas) 

        while end == False:
            while dIndex < len(datas_d) and len(self.Ganttchart) == datas_d[dIndex][2]:
                que.append(datas_d[dIndex])
                que.sort(key=lambda x: (x[1], x[2],x[0])) # sort CPU Burst
                dIndex = dIndex + 1
            if len(cpu) == 0:
                if len(que) == 0:
                    self.Ganttchart = self.Ganttchart + "-"
                else:
                    cpu.append(que.pop(0))
            if len(cpu) != 0:       
                cpu[0][1] = cpu[0][1] - 1
                self.Ganttchart = self.Ganttchart + Turn(cpu[0][0])
                if cpu[0][1] == 0:
                    cpu.clear()
            if len(datas) == dIndex and len(cpu) == 0 and len(que) == 0:  
                end = True

class SRTF:
    global datas, outputALL, GanttchartALL, GanttchartALL
    Ganttchart = ""
    output = []

    def __init__(self):
        pass

    def Run(self):
        self.output.clear()
        self.Ganttchart = ""
        self.findExit()
        self.CalTime()
        outputALL.append(self.output)
        GanttchartALL.append(self.Ganttchart)

    def CalTime(self):
        for i in range(len(datas)):
            tat = (self.Ganttchart.rfind(Turn(datas[i][0]))+1) - datas[i][2]
            wt = tat - datas[i][1]
            tmp = [ datas[i][0], tat, wt ]
            self.output.append(tmp)

        self.output.sort(key=lambda x: (x[0])) # sort PID 

    def findExit(self):
        end = False
        dIndex = 0
        que = [] 
        cpu = []
        
        datas_d = copy.deepcopy(datas) 

        while end == False:
            while dIndex < len(datas_d) and len(self.Ganttchart) == datas_d[dIndex][2]:
                que.append(datas_d[dIndex])
                que.sort(key=lambda x: (x[1], x[2], x[0])) # sort CPU Burst
                dIndex = dIndex + 1
            if len(cpu) == 0:
                if len(que) == 0:
                    self.Ganttchart = self.Ganttchart + "-"
                else:
                    cpu.append(que.pop(0))
            if len(cpu) != 0:     
                if len(que) != 0:  
                    if que[0][1] < cpu[0][1]: #奪取(Preemptive)
                        que.append(cpu.pop(0))
                        cpu.append(que.pop(0))
                        que.sort(key=lambda x: (x[1], x[2], x[0])) # sort CPU Burst

                cpu[0][1] = cpu[0][1] - 1
                self.Ganttchart = self.Ganttchart + Turn(cpu[0][0])
                if cpu[0][1] == 0:
                    cpu.clear()

            if len(datas) == dIndex and len(cpu) == 0 and len(que) == 0:  
                end = True

class HRRN:
    global datas, outputALL, GanttchartALL, GanttchartALL
    Ganttchart = ""
    output = []

    def __init__(self):
        pass

    def Run(self):
        self.output.clear()
        self.Ganttchart = ""
        self.findExit()
        self.CalTime()
        outputALL.append(self.output)
        GanttchartALL.append(self.Ganttchart)

    def CalTime(self):
        for i in range(len(datas)):
            tat = (self.Ganttchart.rfind(Turn(datas[i][0]))+1) - datas[i][2]
            wt = tat - datas[i][1]
            tmp = [ datas[i][0], tat, wt ]
            self.output.append(tmp)

        self.output.sort(key=lambda x: (x[0])) # sort PID 

    def findExit(self):
        end = False
        dIndex = 0
        que = [] 
        cpu = []
        
        datas_d = copy.deepcopy(datas) 

        while end == False:
            while dIndex < len(datas_d) and len(self.Ganttchart) == datas_d[dIndex][2]:
                que.append(datas_d[dIndex])
                dIndex = dIndex + 1
            if len(cpu) == 0:
                if len(que) == 0:
                    self.Ganttchart = self.Ganttchart + "-"
                else:
                    que = self.SortResponseRatio(que, len(self.Ganttchart))
                    cpu.append(que.pop(0))
            if len(cpu) != 0:       
                cpu[0][1] = cpu[0][1] - 1
                self.Ganttchart = self.Ganttchart + Turn(cpu[0][0])
                if cpu[0][1] == 0:
                    cpu.clear()
            if len(datas) == dIndex and len(cpu) == 0 and len(que) == 0:  
                end = True

    def SortResponseRatio(self, que, time):
        for line in que:
            ResponseTime = ((time-line[2])+line[1])/float(line[1])
            if len(line) != 5:
                line.append(ResponseTime)
            else:
                line[4] = ResponseTime
        que.sort(key=lambda x: (-x[4], x[2], x[0])) #Sort ResponseRatio, Arrival

        return que

class PPRR:
    global datas, outputALL, GanttchartALL, GanttchartALL
    Ganttchart = ""
    output = []

    def __init__(self, timeSlice):
        self.timeSlice = timeSlice 

    def Run(self):
        self.output.clear()
        self.Ganttchart = ""
        self.findExit()
        self.CalTime()
        outputALL.append(self.output)
        GanttchartALL.append(self.Ganttchart)

    def CalTime(self):
        for i in range(len(datas)):
            tat = (self.Ganttchart.rfind(Turn(datas[i][0]))+1) - datas[i][2]
            wt = tat - datas[i][1]
            tmp = [ datas[i][0], tat, wt ]
            self.output.append(tmp)

        self.output.sort(key=lambda x: (x[0])) # sort PID 

    def findExit(self):
        end = False
        dIndex = 0
        que = [] 
        cpu = []
        ts = int(self.timeSlice)
        datas_d = copy.deepcopy(datas) 

        while end == False:
            while dIndex < len(datas_d) and len(self.Ganttchart) == datas_d[dIndex][2]:
                que.append(datas_d[dIndex])
                que.sort(key=lambda x: (x[3])) # sort Priority 
                dIndex = dIndex + 1
            
            if len(cpu) == 0:
                if len(que) == 0:
                    self.Ganttchart = self.Ganttchart + "-"
                else:
                    cpu.append(que.pop(0)) 
                ts = int(self.timeSlice)

            if len(cpu) != 0:     
                if len(que) != 0: 
                    if  cpu[0][3] > que[0][3] : # 奪取(Preemptive)  
                        que.append(cpu.pop(0))
                        cpu.append(que.pop(0))
                        que.sort(key=lambda x: (x[3])) # sort Priority 
                        ts = int(self.timeSlice)     
                    if cpu[0][3] == que[0][3] : # RR
                        if ts == 0:
                            que.append(cpu.pop(0))
                            cpu.append(que.pop(0))  
                            que.sort(key=lambda x: (x[3])) # sort Priority 
                            ts = int(self.timeSlice)     
                    else:
                        if ts == 0:          
                            ts = int(self.timeSlice)     
                else:
                    if ts == 0:       
                        ts = int(self.timeSlice)            

                cpu[0][1] = cpu[0][1] - 1
                ts = ts - 1 
                self.Ganttchart = self.Ganttchart + Turn(cpu[0][0])
                if cpu[0][1] == 0:
                    cpu.clear()

            if len(datas) == dIndex and len(cpu) == 0 and len(que) == 0:  
                end = True
        
class ALL():
    global interface, outputALL, GanttchartALL
    outputTmp = []
    def __init__(self, timeSlice):
        self.timeSlice = timeSlice    

    def tmpOuput(self):
        i = 0 
        for output in outputALL[0]:
            self.outputTmp[i].append(output[2])
            i = i + 1
        i = i + 1 
        for output in outputALL[0]:
            self.outputTmp[i].append(output[1])
            i = i + 1
        outputALL.clear()         

    def Run(self):
        self.outputTmp.clear()
        fcfs = FCFS()
        fcfs.Run() 
        for output in outputALL[0]:
            line = [output[0],output[2]] # WT
            self.outputTmp.append(line)
        
        self.outputTmp.append(["\n"])

        for output in outputALL[0]:
            line = [output[0],output[1] ] # TAT
            self.outputTmp.append(line)
        outputALL.clear()

        rr= RR(self.timeSlice)
        rr.Run()
        self.tmpOuput()

        sjf = SJF()
        sjf.Run()
        self.tmpOuput()

        srtf = SRTF()
        srtf.Run()
        self.tmpOuput()

        hrrn = HRRN()
        hrrn.Run()
        self.tmpOuput()

        pprr = PPRR(self.timeSlice)
        pprr.Run()
        self.tmpOuput()

        outputALL.append(self.outputTmp)


        
def main():

    global datas, outputALL, interface, GanttchartALL

    end = False 

    while end == False:
        datas.clear()
        outputALL.clear()
        interface.clear()
        GanttchartALL.clear()

        name, cmd, timeSlice = Read()

        if cmd == "0":
            end = True 

        elif cmd != "0" and os.path.exists(name + ".txt"):
            if cmd == "1": 
                fcfs = FCFS()
                fcfs.Run()
            elif cmd == "2":
                rr= RR(timeSlice)
                rr.Run()
            elif cmd == "3":
                sjf = SJF()
                sjf.Run()
            elif cmd == "4":
                srtf = SRTF()
                srtf.Run()
            elif cmd == "5":
                hrrn = HRRN()
                hrrn.Run()
            elif cmd == "6":
                pprr = PPRR(timeSlice)
                pprr.Run()
            elif cmd == "7":
                all = ALL(timeSlice)
                all.Run()

 
            if cmd != "7":
                Write(name, cmd)
            else:
                Write7(name, cmd)

if __name__ == '__main__':
    main()