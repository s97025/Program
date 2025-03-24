import torch
import torch.nn as nn
import torch.optim as optim
from tqdm import tqdm
from model import ModelFactory
from dataHandler import DataHandler
from utils import SaveModel, SavePredictions, SaveReport, TrainConfig
import os 
from dotenv import load_dotenv
load_dotenv()

SEED = int(os.getenv('SEED'))
DEVICE = 'cuda' if torch.cuda.is_available() else 'cpu'
INFINITE = float('inf')


# 啟用 cuDNN 加速
torch.backends.cudnn.enabled = True
# 啟用 cuDNN 的 benchmark 模式，如果你的數據大小固定，可以提高性能
torch.backends.cudnn.benchmark = True



def ConfusionMatrix():
    import matplotlib.pyplot as plt
    import numpy as np
    from sklearn.metrics import confusion_matrix   
    
    def plotCM(path, cm, classes, normalize=False, title='Confusion matrix', cmap=plt.cm.Blues):
        if normalize:
            cm = cm.astype('float') / cm.sum(axis=1)[:, np.newaxis]
            print("Normalized confusion matrix")
        else:
            print('Confusion matrix, without normalization')

        plt.figure(figsize=(15,15))  
        plt.imshow(cm, interpolation='nearest', cmap=cmap) # 矩陣的顏色不做插值
        plt.title(title)
        plt.colorbar()
        
        tickMarks = np.arange(len(classes))

        plt.xticks(tickMarks, classes, rotation=90)  # 標記為類別名稱，並旋轉 90 度便於閱讀
        plt.yticks(tickMarks, classes)
        plt.ylabel('True label')
        plt.xlabel('Predicted label')
        plt.tight_layout()
        plt.savefig(path, dpi=300)

    dataHandler = DataHandler()
    numClasses = dataHandler.numOfClasses

    testName = os.getenv('TEST_NAME')
    numBaseModel = int(os.getenv('NUM_BASE_MODEL'))
    baseModels = []
    for modelIndex in range(numBaseModel):
        modelName = os.getenv(f'MODEL_NAME_{modelIndex}')
        baseModels.append(ModelFactory.LoadModel(modelName, numClasses, f"model/{testName}/{modelName}/best.pth").to(DEVICE))
    for model in baseModels:
        model.eval()

    modelNameMeta = os.getenv('MODEL_NAME_META')
    metaModel = ModelFactory.LoadMetaModel(modelNameMeta, len(baseModels) * numClasses, numClasses, f"model/{testName}/meta_{modelNameMeta}/best.pth").to(DEVICE)
    metaModel.eval() # 設置為評估模式

    criterion = nn.CrossEntropyLoss()


    # In your validation loop
    allLabels = []
    allPredictions = []

    with torch.no_grad():
        for inputs, labels in tqdm(dataHandler.validLoader, desc=f"MetaModel: {modelNameMeta} Validating", total=len(dataHandler.validLoader)):
            metaFeatures = []
            inputs, labels = inputs.to(DEVICE), labels.to(DEVICE)
            
            for model in baseModels:
                outputs = model(inputs)
                probs = nn.functional.softmax(outputs, dim=1)
                metaFeatures.append(probs)

            metaInputs = torch.cat(metaFeatures, dim=1)
            metaOutputs = metaModel(metaInputs)
            loss = criterion(metaOutputs, labels)

            _, predicted = metaOutputs.max(1)

            # Append labels and predictions to list
            allLabels.extend(labels.cpu().numpy())
            allPredictions.extend(predicted.cpu().numpy())

    # Compute the confusion matrix
    cm = confusion_matrix(allLabels, allPredictions)
    plotCM(f'report/{testName}/confusion_Matrix.png', cm, dataHandler.classes)


def BaseModelTrain(dataHandler: DataHandler, config: TrainConfig):
    model = ModelFactory.GetModel(config.modelName, dataHandler.numOfClasses)
    model = model.to(DEVICE)
    
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.Adam(model.parameters(), lr=config.lr)


    bestValid =  INFINITE
    trainLosses = []
    validLosses = []
    validAccs = []
    
    for epoch in range(config.epoch):
        model.train()
        avgTrainLoss = 0.0
        trainIter = tqdm(dataHandler.trainLoader, desc=f"BaseModel: {config.modelName} Training On Epoch {epoch}", total=len(dataHandler.trainLoader))  # 視覺化當前訓練進度

        for inputs, labels in trainIter:
            inputs = inputs.to(DEVICE)
            labels = labels.to(DEVICE)
            optimizer.zero_grad()
            outputs = model(inputs)
            # # 防止模型輸出過大或過小的值進入損失函數
            # outputs = torch.clamp(outputs, min=-10, max=10)
            loss = criterion(outputs, labels)

            loss.backward()
            # # 對梯度進行裁剪，限制最大梯度范數為 5.0 (防止梯度爆炸)
            # torch.nn.utils.clip_grad_norm_(model.parameters(), max_norm=5.0)            
            optimizer.step()
            avgTrainLoss += loss.item()
            trainIter.set_postfix_str(f"Loss: {loss.item():.2f}")

            
        model.eval()
        avgValidLoss = 0.0
        correct = 0
        total = 0
        with torch.no_grad():
            for inputs, labels in dataHandler.validLoader:
                inputs = inputs.to(DEVICE)
                labels = labels.to(DEVICE)
                outputs = model(inputs)
                avgValidLoss += criterion(outputs, labels).item()
                _, predicted = outputs.max(1)
                total += labels.size(0)
                correct += predicted.eq(labels).sum().item() # predicted 和 labels 進行逐元素比較，返回布林值張量，表示預測是否正確，返回正確數量
                
                
        avgTrainLoss /= len(dataHandler.trainLoader)
        avgValidLoss /= len(dataHandler.validLoader)
        validAcc = 100. * correct / total
        print(f" Epoch {epoch+1}/{config.epoch}, Train Loss: {avgTrainLoss: .4f}, Vaild Loss: {avgValidLoss: .4f}, Valid Acc: {validAcc: .2f}%")
        SaveModel(model=model, path=f"model/{config.testName}/{config.modelName}/{epoch}.pth")
        
        if avgValidLoss < bestValid:
            bestValid = avgValidLoss 
            SaveModel(model=model, path = f"model/{config.testName}/{config.modelName}/best.pth")
        
        trainLosses.append(avgTrainLoss)
        validLosses.append(avgValidLoss)
        validAccs.append(validAcc)
        
    SaveReport(trainLosses, validLosses, validAccs, config, saveDir = f'report/{config.testName}/{config.modelName}')

def pipelineTrain():
    TEST_NAME = os.getenv('TEST_NAME')
    dataHandler = DataHandler()
    numClasses = dataHandler.numOfClasses

    NUM_BASE_MODEL = int(os.getenv('NUM_BASE_MODEL'))
    for modeIndex in range(NUM_BASE_MODEL):
        MODEL_NAME =  os.getenv(f'MODEL_NAME_{modeIndex}')
        EPOCH = int(os.getenv(f'EPOCH_{modeIndex}'))
        LR = float(os.getenv(f'LR_{modeIndex}'))
        BaseModelTrain(dataHandler, TrainConfig(TEST_NAME, MODEL_NAME, EPOCH, LR))
    
    
    baseModels = []
    for modeIndex in range(NUM_BASE_MODEL):
        MODEL_NAME = os.getenv(f'MODEL_NAME_{modeIndex}')
        baseModels.append(ModelFactory.LoadModel(MODEL_NAME, numClasses, f"model/{TEST_NAME}/{MODEL_NAME}/best.pth").to(DEVICE))
        
    MODEL_NAME_META = os.getenv('MODEL_NAME_META')
    EPOCH_META = int(os.getenv('EPOCH_META'))
    LR_META = float(os.getenv('LR_META'))
    
    metaModel = ModelFactory.GetMetaModel(MODEL_NAME_META, len(baseModels) * numClasses, numClasses).to(DEVICE)
    
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.Adam(metaModel.parameters(), lr=LR_META)
    
    bestValid = INFINITE
    trainLosses = []
    validLosses = []
    validAccs = []
    
    for epoch in range(EPOCH_META):
        metaModel.train()
        avgTrainLoss = 0.0
        trainIter = tqdm(dataHandler.trainLoader, desc=f"MetaModel: {MODEL_NAME_META} Training On Epoch {epoch}", total=len(dataHandler.trainLoader))
        for inputs, labels in trainIter:
            meta_features = []
            inputs = inputs.to(DEVICE)
            labels = labels.to(DEVICE)
            
            for model in baseModels:
                with torch.no_grad():
                    outputs = model(inputs)
                    probs = nn.functional.softmax(outputs, dim=1) # softmax 函數將模型的輸出轉換為概率分布 probs，dim=1 表示對每一行（即每個樣本）進行 softmax 計算
                    meta_features.append(probs)
            
            meta_inputs = torch.cat(meta_features, dim=1) # 將所有基礎模型的預測概率沿維度 1（特徵維度）串接起來
            optimizer.zero_grad()
            meta_outputs = metaModel(meta_inputs)
            loss = criterion(meta_outputs, labels)
            loss.backward()
            optimizer.step()
            avgTrainLoss += loss.item()
            trainIter.set_postfix_str(f"Loss: {loss.item(): .2f}")
    
        metaModel.eval()
        avgValidLoss = 0.0
        correct = 0
        total = 0
        
        with torch.no_grad():
            for inputs, labels in tqdm(dataHandler.validLoader, desc=f"MetaModel: {MODEL_NAME_META} Validing On Epoch {epoch}", total=len(dataHandler.validLoader)):
                meta_features = []
                inputs = inputs.to(DEVICE)
                labels = labels.to(DEVICE)
            
                for model in baseModels:
                    outputs = model(inputs)
                    probs = nn.functional.softmax(outputs, dim=1)
                    meta_features.append(probs)
                
                meta_inputs = torch.cat(meta_features, dim=1)
                meta_outputs = metaModel(meta_inputs)
                avgValidLoss += criterion(meta_outputs, labels).item()
                _, predicted = meta_outputs.max(1)
                total += labels.size(0)
                correct += predicted.eq(labels).sum().item()
        
        avgTrainLoss /= len(dataHandler.trainLoader)
        avgValidLoss /= len(dataHandler.validLoader)
        validAcc = 100. * correct / total
        
        print(f"Epoch {epoch+1}/{EPOCH_META}, Train Loss: {avgTrainLoss: .4f}, Valid Loss {avgValidLoss: .4f}, valid Acc: {validAcc: .2f}%")
        SaveModel(model=metaModel, path=f'model/{TEST_NAME}/meta_{MODEL_NAME_META}/{epoch}.pth')

        
        if avgValidLoss < bestValid:
            bestValid = avgValidLoss
            SaveModel(model=metaModel, path=f'model/{TEST_NAME}/meta_{MODEL_NAME_META}/best.pth')


        trainLosses.append(avgTrainLoss)
        validLosses.append(avgValidLoss)
        validAccs.append(validAcc)
        
    config = TrainConfig(TEST_NAME, MODEL_NAME_META, EPOCH_META, LR_META)
    SaveReport(trainLosses, validLosses, validAccs, config, saveDir = f'report/{config.testName}/meta_{config.modelName}')
    ConfusionMatrix()
    
    
def pipelineTest():
    dataHandler = DataHandler()
    numClasses = dataHandler.numOfClasses
    TEST_NAME = os.getenv('TEST_NAME')
    NUM_BASE_MODEL = int(os.getenv('NUM_BASE_MODEL'))
    
    baseModels = []
    for modelIndex in range(NUM_BASE_MODEL):
        MODEL_NAME = os.getenv(f'MODEL_NAME_{modelIndex}')
        baseModels.append(ModelFactory.LoadModel(MODEL_NAME, numClasses, f"model/{TEST_NAME}/{MODEL_NAME}/best.pth").to(DEVICE))
    for model in baseModels:
        model.eval()
    
    MODEL_NAME_META = os.getenv('MODEL_NAME_META')
    metaModel = ModelFactory.LoadMetaModel(MODEL_NAME_META, len(baseModels) * numClasses, numClasses, f"model/{TEST_NAME}/meta_{MODEL_NAME_META}/best.pth").to(DEVICE)
    metaModel.eval()
    
    predictions = []
    fileNameLists = []
    
    with torch.no_grad():
        for inputs, fileNames in dataHandler.testLoader:
            meta_features = []
            inputs = inputs.to(DEVICE)
            
            for model in baseModels:
                outputs = model(inputs)
                probs = nn.functional.softmax(outputs, dim=1)
                meta_features.append(probs)
            
            meta_inputs = torch.cat(meta_features, dim=1)
            meta_outputs = metaModel(meta_inputs)
            _, predictedIndices = torch.max(meta_outputs, 1)
            
            predictions.extend(predictedIndices.tolist())
            fileNameLists.extend(fileNames)
            
    # 將Indices預測轉換回labels 
    predictedLabels = [dataHandler.classes[i] for i in predictions]
    ids = [name.split('.')[0] for name in fileNameLists]
    SavePredictions(ids, predictedLabels, f"predict/{TEST_NAME}.csv")
    
    
    
if __name__ == "__main__":
    torch.manual_seed(SEED)    
    pipelineTrain()
    pipelineTest()
    print("Finish All!!!")
