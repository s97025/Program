import torch
import torch.nn as nn
import torch.nn.init as init
import torchvision.models as models
import math

class SimpleLinear(nn.Module):
        def __init__(self, input_dim, num_classes):
            super(SimpleLinear, self).__init__()
            self.fc = nn.Linear(input_dim, 3 * input_dim)
            self.relu = nn.ReLU()
            self.fc2 = nn.Linear(3 * input_dim, num_classes)
            init.kaiming_uniform_(self.fc.weight, nonlinearity='relu')
            
        def forward(self, x):
            return self.fc2(self.relu(self.fc(x)))

class SimpleCNN(nn.Module):
    def __init__(self, numClasses):
        super(SimpleCNN, self).__init__()
        self.features = nn.Sequential(
            nn.Conv2d(3, 32, kernel_size=3, stride=1, padding=1),
            nn.ReLU(inplace=True),
            nn.MaxPool2d(kernel_size=2, stride=2),
            nn.Conv2d(32, 64, kernel_size=3, padding=1),
            nn.ReLU(inplace=True),
            nn.MaxPool2d(kernel_size=2, stride=2),
            nn.Conv2d(64, 128, kernel_size=3, padding=1),
            nn.ReLU(inplace=True),
            nn.MaxPool2d(kernel_size=2, stride=2)
        )
        self.classifier = nn.Sequential(
            nn.Linear(128*28*28, 512),  # 28*28 from the image size (224/2/2/2 = 28) 
            nn.ReLU(inplace=True),
            nn.Linear(512, numClasses),
        )
        
    def forward(self, x):
        x = self.features(x)
        x = x.view(x.size(0), -1)
        x = self.classifier(x)
        return x
    
class ModelFactory:
    @staticmethod
    def GetModel(modelName, numClasses):
        if modelName == "simpleCNN":
            model = SimpleCNN(numClasses)
        elif modelName == "resnet50":
            model = models.resnet50(weights=models.ResNet50_Weights.DEFAULT)
            model.fc = nn.Linear(model.fc.in_features, numClasses)
        elif modelName == "convnext_tiny":
            model = models.convnext_tiny(weights=models.ConvNeXt_Tiny_Weights.DEFAULT)
            model.classifier[2] = nn.Linear(model.classifier[2].in_features, numClasses)
            init.trunc_normal_(model.classifier[2].weight, std=0.02)
        elif modelName == "convnext_base":
            model = models.convnext_base(weights=models.ConvNeXt_Base_Weights.DEFAULT)
            model.classifier[2] = nn.Linear(model.classifier[2].in_features, numClasses)
            init.trunc_normal_(model.classifier[2].weight, std=0.02)
        elif modelName == "efficientnet_v2_s":
            model = models.efficientnet_v2_s(weights=models.EfficientNet_V2_S_Weights.DEFAULT)
            model.classifier[1] = nn.Linear(model.classifier[1].in_features, numClasses)
            init_range = 1.0 / math.sqrt(model.classifier[1].out_features)
            init.uniform_(model.classifier[1].weight, -init_range, init_range)
        elif modelName == "efficientnet_v2_m":
            model = models.efficientnet_v2_m(weights=models.EfficientNet_V2_M_Weights.DEFAULT)
            model.classifier[1] = nn.Linear(model.classifier[1].in_features, numClasses)
            init_range = 1.0 / math.sqrt(model.classifier[1].out_features)
            init.uniform_(model.classifier[1].weight, -init_range, init_range)
        elif modelName == "swin_v2_s":
            model = models.swin_v2_s(weights=models.Swin_V2_S_Weights.DEFAULT)
            model.head = nn.Linear(model.head.in_features, numClasses)
            init.trunc_normal_(model.head.weight, std=0.02)
        elif modelName == "swin_v2_b":
            model = models.swin_v2_b(weights=models.Swin_V2_B_Weights.DEFAULT)
            model.head = nn.Linear(model.head.in_features, numClasses)
            init.trunc_normal_(model.head.weight, std=0.02)
        elif modelName == "maxvit_t":
            model = models.maxvit_t(weights=models.MaxVit_T_Weights.DEFAULT)
            model.classifier[5] = nn.Linear(model.classifier[5].in_features, numClasses, bias=False)
            init.normal_(model.classifier[5].weight, std=0.02)
        else:
            raise NotImplementedError('Not Implement this pretrained model!')
        
        return model

    @staticmethod
    def LoadModel(modelName, numClasses, path):
        # 調用 GetModel 來創建模型
        model = ModelFactory.GetModel(modelName, numClasses)
        
        # 載入模型參數
        model.load_state_dict(torch.load(path))
        return model
    
    
    @staticmethod
    def GetMetaModel(modelName, inputDim, numClasses):
        if modelName == "simpleLinear":
            model = SimpleLinear(inputDim, numClasses)
        else:
            raise NotImplementedError('Not Implement this meta model!')
        return model
    
    @staticmethod
    def LoadMetaModel(modelName, inputDim, numClasses, path):
        # 使用 GetMetaModel 來創建模型結構
        model = ModelFactory.GetMetaModel(modelName, inputDim, numClasses)
        
        # 載入模型權重
        model.load_state_dict(torch.load(path))
        return model
