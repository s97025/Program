import matplotlib.pyplot as plt
from dataclasses import dataclass 
import torch
import os
import numpy as np


class TrainConfig:
    testName:str
    modelName: str
    epoch: int
    lr: float
    
    def __init__(self, testName: str, modelName: str, epoch: int, lr: float):
        self.testName = testName
        self.modelName = modelName
        self.epoch = epoch
        self.lr = lr
        

def SaveModel(model, path):
    # 判斷資料及是否存在
    directory = os.path.dirname(path)
    if not os.path.exists(directory):
        os.makedirs(directory)
    
    # 儲存 model
    torch.save(model.state_dict(), path)


def SavePredictions(ids, predictedLabels, path):
    # 判斷資料及是否存在
    directory = os.path.dirname(path)
    if not os.path.exists(directory):
        os.makedirs(directory)
    
    # 寫入 predictions to CSV
    with open(path, "w") as f:
        f.write("id,character\n")
        for id_, label in zip(ids, predictedLabels):
            f.write(f"{id_},{label}\n")


def SaveReport(trainLosses, valLosses, valAccs, config, saveDir):
    # 判斷資料及是否存在
    if not os.path.exists(saveDir):  # 判斷資料及是否存在
        os.makedirs(saveDir)
    
    with open(os.path.join(saveDir, 'trainLosses.txt'), 'w') as file:
        for loss in trainLosses:
            file.write(f"{loss}\n")

    with open(os.path.join(saveDir, 'valLosses.txt'), 'w') as file:
        for loss in valLosses:
            file.write(f"{loss}\n")

    with open(os.path.join(saveDir, 'valAccs.txt'), 'w') as file:
        for acc in valAccs:
            file.write(f"{acc}\n")

    # 1. 儲存 config to hyperparameter.txt
    with open(os.path.join(saveDir, 'hyperparameters.txt'), 'w') as file:
        file.write(f"Test Name: {config.testName}\n")
        file.write(f"Model Name: {config.modelName}\n")
        file.write(f"Epoch: {config.epoch}\n")
        file.write(f"Learning Rate: {config.lr}\n")

    # 2. 繪製 a train loss, val loss figure as training.png
    plt.plot(trainLosses, label='Training Loss')
    plt.plot(valLosses, label='Validation Loss')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.legend()
    plt.title('Training and Validation Losses')
    plt.savefig(os.path.join(saveDir, 'training.png'), dpi=300)
    plt.close()
    

def PlotFilters(path, weights):
    # Get the number of filters (kernels)
    numFilters = weights.shape[0]

    # Determine the number of rows and columns for the grid of filters
    numCols = int(np.sqrt(numFilters))
    numRows = numFilters // numCols + int(numFilters % numCols != 0)
    
    figObj = plt.figure(figsize=(numCols * 2, numRows * 2))
    
    for i in range(numFilters):
        axObj = figObj.add_subplot(numRows, numCols, i+1)
        
        # For an RGB image, the filters will be 3D tensors.
        # We can visualize each channel separately or take the average to represent it in grayscale.
        if weights.shape[1] == 3:  # RGB image
            imgObj = weights[i].transpose(1, 2, 0)
            # Normalize between [0, 1]
            imgObj = (imgObj - imgObj.min()) / (imgObj.max() - imgObj.min())
            axObj.imshow(imgObj)
        else:  # Grayscale image
            axObj.imshow(weights[i, 0], cmap='gray')
        
        axObj.axis('off')
    
    plt.savefig(path, dpi=300)
    plt.close(figObj)