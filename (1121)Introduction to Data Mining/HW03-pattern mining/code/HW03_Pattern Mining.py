import pandas as pd
from mlxtend.preprocessing import TransactionEncoder
from mlxtend.frequent_patterns import fpgrowth
import time
import numpy as np
import matplotlib.pyplot as plt


def ReadFileDoc(path, D, W , NNZ) :
  f = open(path)
  text = []
  i = 0
  for line in f:
    if i == 0:
        D = int(line)
    elif i == 1:
        W = int(line)
    elif i == 2:
        NNZ = int(line)
        text = [[] for i in range(NNZ)] 
    else: 
      line = line.split(' ')
      text[i-3].append(int(line[0]))
      text[i-3].append(int(line[1]))
      text[i-3].append(int(line[2].strip()))  
    i = i + 1

  sorted(text, key=lambda x: (x[0], x[1])) 
  
  # print(text)
  return text, D, W , NNZ

def ReadFileWord(path) :
  f = open(path)
  text = []
  for line in f:
      text.append(line.strip())

  return text

def FP_fpgrowth(dataset, minSupport):
  dataset.sort()
  te = TransactionEncoder()
  te_ary = te.fit(dataset).transform(dataset)
  df = pd.DataFrame(te_ary, columns=te.columns_)
  return fpgrowth(df, min_support=minSupport,use_colnames=True) 

def do_all_freq(Dict, minSupport):
  all_freq = FP_fpgrowth(Dict, minSupport)
  paatterns = all_freq.shape[0]
  # count all frequent correct 
  # rint(all_freq)
  # print('Time to find frequent itemset')
  # print("all_freq_count: ", str(all_freq.shape[0]))  
  # print("--- %s seconds ---" % (time.time() - start_time))  

  return all_freq, paatterns
  

def do_Closed_frequent(all_freq, fredic):
#Find Closed frequent itemset
  start_time = time.time()
  closed_freq = []
  for index, row in all_freq.iterrows():
      isclose = True
      cli = row['itemsets']
      cls = row['support']
      checkset = fredic[cls]
      for i in checkset:
          if (cli!=i):
              if(frozenset.issubset(cli,i)):
                  isclose = False
                  break
      
      if(isclose):
          closed_freq.append(row['itemsets'])
          
  # print('Time to find Close frequent itemset')
  # print("closed_freq_count: ", len(closed_freq))  
  # print("--- %s seconds ---" % (times))  

  return closed_freq, len(closed_freq)


def do_Max_frequent(all_freq, fredic2):
  #Find Max frequent itemset
  max_freq = []
  for index, row in all_freq.iterrows():
      isclose = True
      cli = row['itemsets']
      cls = row['support']
      checkset = fredic2[cls]
      for i in checkset:
          if (cli!=i):
              if(frozenset.issubset(cli,i)):
                  isclose = False
                  break
      
      if(isclose):
          max_freq.append(row['itemsets'])

  # print('Time to find Max frequent itemset')
  # print("max_freq: ", len(max_freq))  
  # print("--- %s seconds ---" % (time.time() - start_time))

  return max_freq, len(max_freq)

def draw(minSupport_all, all_, closed_, max_, title, xlabel, ylabel):
  plt.style.use('bmh')
  fig = plt.figure()
  ax = plt.axes()

  plt.plot(minSupport_all, all_, 'b', label='all_freq')
  plt.plot(minSupport_all, closed_, 'r', label='closed_freq')
  plt.plot(minSupport_all, max_, 'g', label='max_freq')
  plt.xticks(minSupport_all) #因兩人賽季數相同，故任選
  plt.legend(loc = 'upper right')
  plt.title(title)
  plt.xlabel(xlabel)
  plt.ylabel(ylabel)
  #plt.show()

  fig.savefig( title )
  print("Already save " + title + ".png!" )

if __name__ == '__main__':
  D = 0
  W = 0
  NNZ = 0
  start_time = 0
  all_freq = None
  patterns = 0
  times = 0
  minSupport_all = []
  all_patterns = []
  closed_patterns = []
  max_patterns = []
  all_times = []
  closed_times = []
  max_times = []

  fileName = str(input('Input fileName(ex: nips, kos...)： '))
  minSupport = float(input('Input mininum of threshold(ex:0.2 (0, 1])： '))
  while True:
    increase = float(input('Input mininum of threshold each time increase(ex:0.1, 0.2 [0, 0.9])： '))  
    RepeatTimes = int(input('Input mininum of threshold RepeatTimes(ex:1,2 [1, N])： '))

    if (increase * RepeatTimes) <= 1:
       break
    else:
       print("Error! Final minSupport will beyond the range from zero to one")
       print("Please try again!")


  words = ReadFileWord('vocab.'+ fileName +'.txt')
  allIDCount, D, W , NNZ  = ReadFileDoc('docword.'+ fileName + '.txt', D, W , NNZ)

  # test read correct 
  # print(words[0])
  # print(allIDCount[0])

  Dict = [[] for i in range(D)] # 0-3429 共3430列

  for line in allIDCount: 
    docID = line[0]-1
    wordID = line[1]-1
    #check find correct word
    #if line[0] == allIDCount[0][0]:
      # print(line[0])
      # print(line[1])
      # print(words[wordID].strip())
    Dict[docID].append(words[wordID]) #.strip()去掉換行字符'\n'
 

  for i in range(RepeatTimes):  
    # print("all frequent...")
    start_time = time.time()
    all_freq, patterns = do_all_freq(Dict, minSupport)
    times = time.time() - start_time
    all_patterns.append(patterns)
    all_times.append(times)

    # do same change to find Closed frequent itemset and  Max frequent itemset
    su = all_freq.support.unique() # all unique support count
    fredic = {}
    for i in range(len(su)):
        inset = list(all_freq.loc[all_freq.support==su[i]]['itemsets'])
        fredic[su[i]] = inset
    fredic2 = {}
    for i in range(len(su)):
        inset2 = list(all_freq.loc[all_freq.support<=su[i]]['itemsets'])
        fredic2[su[i]] = inset2

    # print("Closed frequent...")
    start_time = time.time()
    closed_freq, patterns = do_Closed_frequent(all_freq, fredic)
    times = time.time() - start_time
    closed_patterns.append(patterns)
    closed_times.append(times)
    # print("Max frequent...")
    start_time = time.time()
    max_freq, patterns = do_Max_frequent(all_freq, fredic2)
    times = time.time() - start_time
    max_patterns.append(patterns)
    max_times.append(times)

    minSupport_all.append(minSupport)
    minSupport = round(minSupport + increase, 1) # round select one digit after decimal point

  # test n vaild patterns and average time correct
  print("patterns: ")
  print("all_patterns", all_patterns)
  print("closed_patterns", closed_patterns)
  print("max_patterns", max_patterns)
  print("average time: ")
  print("all_times", all_times)
  print("closed_times", closed_times)
  print("max_times", max_times)
  print("threshold_all: ")
  print("threshold", minSupport_all)

  draw(minSupport_all, all_patterns, closed_patterns, max_patterns, fileName + "_patterns", "threshold", "the number of patterns")
  draw(minSupport_all, all_times, closed_times, max_times, fileName + "_excution time", "threshold", "the average excution time")

