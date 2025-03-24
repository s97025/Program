import numpy as np 
import pandas as pd
import csv
from sklearn.tree import DecisionTreeClassifier, export_graphviz
from sklearn.ensemble import RandomForestClassifier
import matplotlib.pyplot as plt
import graphviz
import pydotplus


classes = ['Grass', 'Fire', 'Water', 'Bug', 'Normal', 'Poison', 'Electric', \
           'Ground', 'Fairy', 'Fighting', 'Psychic', 'Rock', 'Ghost', 'Ice', \
           'Dragon', 'Dark', 'Steel' , 'Flying']
features = ['HP', 'Attack', 'Defense', 'Sp..Atk', 'Sp..Def', 'Speed', 'Legendary']
num = 0

def tree_mdl(x_train, y_train, x_test, y_test, criterion, max_depth):
    global classes, features

    tree = DecisionTreeClassifier(random_state=1, criterion=criterion, max_depth=max_depth)
    tree.fit(x_train, y_train)

    acu_train = tree.score(x_train,y_train)
    acu_test = tree.score(x_test,y_test)

    return acu_train, acu_test, tree

def run_tree(train_data, valid_data):
    # y_train 只有'Type.1'欄  
    # X_train有'HP'	'Attack'	'Defense'	'Sp..Atk'	'Sp..Def'	'Speed' 'Legendary'欄    
    y_train = train_data['Type.1']
    x_train = train_data.drop(['Type.1'], axis=1)

    y_valid = valid_data['Type.1']
    x_valid = valid_data.drop(['Type.1'], axis=1)

    accuracy_tree = 0
    accuracy_value = 0
    criterion_value = ""
    max_depth_value = 0

    result = {"criterion":[],
              "max_depth":[],
              "acu_train":[],
              "acu_valid":[]
             }

    for criterion in ["gini", 'entropy']:
      acu_tr_lis = []
      acu_va_lis = []

      for max_depth in range(1, 11):
        acu_train, acu_valid, tree = tree_mdl(x_train, y_train, x_valid, y_valid, criterion, max_depth)
        acu_tr_lis.append(acu_train)
        acu_va_lis.append(acu_valid)
        result["criterion"].append(criterion)
        result["max_depth"].append(max_depth)
        result["acu_train"].append(acu_train)
        result["acu_valid"].append(acu_valid)


        if acu_valid > accuracy_value:
            accuracy_tree = tree
            accuracy_value = acu_valid
            criterion_value = criterion
            max_depth_value = max_depth

      # 繪製折線圖 比較 train valid max_depth增加後的的精確度
      plt.plot(range(1,11), acu_tr_lis, "o-", label="acu-train")
      plt.plot(range(1,11), acu_va_lis, "*-", label="acu-valid")
      plt.xlabel("max_depath")
      plt.ylabel("accuracy")
      plt.title("Criterion = "+str(criterion))   
      plt.legend(["acu-train", "acu-valid"])
      plt.savefig( num + "_" + criterion + '.png')
      plt.show()
      
    acu_train = tree.score(x_train,y_train)  
    acu_valid = tree.score(x_valid,y_valid) 
    print("儲存entropy.png")
    print("儲存gini.png")    
    print("使用演算法: " + criterion_value + "  " + "max_path深度: " + str(max_depth_value) ) 
    print("acu_train:", acu_train)
    print("acu_valid:", acu_valid)

    return tree

def TestTree(test_data, result):
  y_test = test_data['Type.1']
  x_test = test_data.drop(['Type.1'], axis=1)
  acu_test = result.score(x_test,y_test)
  print("acu_test:", acu_test)


def Rd_tree_mdl(x_train, y_train, x_test, y_test, criterion, max_depth):
  Rd_tree = RandomForestClassifier(n_estimators=100, criterion = criterion, max_depth=max_depth)
  Rd_tree.fit(x_train, y_train)
  acu_train = Rd_tree.score(x_train,y_train)
  acu_test = Rd_tree.score(x_test,y_test)
  # print('訓練集: ',acu_train)
  # print('測試集: ',acu_test)
  return acu_train, acu_test, Rd_tree

def Rd_run_tree(train_data, valid_data):
    # y_train 只有'Type.1'欄  
    # X_train有'HP'	'Attack'	'Defense'	'Sp..Atk'	'Sp..Def'	'Speed' 'Legendary'欄    
    y_train = train_data['Type.1']
    x_train = train_data.drop(['Type.1'], axis=1)

    y_valid = valid_data['Type.1']
    x_valid = valid_data.drop(['Type.1'], axis=1)

    accuracy_tree = 0
    accuracy_value = 0
    criterion_value = ""
    max_depth_value = 0

    result = {"criterion":[],
              "max_depth":[],
              "acu_train":[],
              "acu_valid":[]
             }

    for criterion in ["gini", 'entropy']:
      acu_tr_lis = []
      acu_va_lis = []

      for max_depth in range(1, 11):
        acu_train, acu_valid, tree = Rd_tree_mdl(x_train, y_train, x_valid, y_valid, criterion, max_depth)
        acu_tr_lis.append(acu_train)
        acu_va_lis.append(acu_valid)
        result["criterion"].append(criterion)
        result["max_depth"].append(max_depth)
        result["acu_train"].append(acu_train)
        result["acu_valid"].append(acu_valid)


        if acu_valid > accuracy_value:
            accuracy_tree = tree
            accuracy_value = acu_valid
            criterion_value = criterion
            max_depth_value = max_depth

      # 繪製折線圖 比較 train valid max_depth增加後的的精確度
      plt.plot(range(1,11), acu_tr_lis, "o-", label="acu-train")
      plt.plot(range(1,11), acu_va_lis, "*-", label="acu-valid")
      plt.xlabel("max_depath")
      plt.ylabel("accuracy")
      plt.title("Criterion = "+str(criterion))   
      plt.legend(["acu-train", "acu-valid"])
      plt.savefig( num + "_" + criterion + '.png')
      plt.show()
      
    acu_train = tree.score(x_train,y_train)  
    acu_valid = tree.score(x_valid,y_valid) 
    print("儲存entropy.png")
    print("儲存gini.png")   
    print("使用演算法: " + criterion_value + "  " + "max_path深度: " + str(max_depth_value) ) 
    print("acu_train:", acu_train)
    print("acu_valid:", acu_valid)

    return tree

if __name__=="__main__":
    train_data = pd.read_csv('pokemon_train.csv', encoding = 'utf-8', index_col = 0 )    
    train_data.drop(columns = ['Type.2', 'Total','Generation'], inplace=True, axis=1)    
    
    valid_data = pd.read_csv('pokemon_valid.csv', encoding = 'utf-8', index_col = 0 )    
    valid_data.drop(columns = ['Type.2', 'Total','Generation'], inplace=True, axis=1)

    test_data = pd.read_csv('pokemon_test.csv', encoding = 'utf-8', index_col = 0 )    
    test_data.drop(columns = ['Type.2', 'Total','Generation'], inplace=True, axis=1)

    while(True):
      print("\n")
      print("0 or others. exit")
      print("1.Decision Tree")
      print("2.randomForest")
      print("=============================")
      num = input("Input num (0,1,2): ")

      if num == '1' :
          result = run_tree(train_data, valid_data)  
          TestTree(test_data, result)
      elif num == '2':
          result = Rd_run_tree(train_data, valid_data)  
          TestTree(test_data, result)
      else: 
        exit()


