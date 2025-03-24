import torch
import torch.nn as nn
from tqdm import tqdm
from dataHandler import DataHandler
from model import ModelFactory
from utils import PlotFilters
import os 
from dotenv import load_dotenv
load_dotenv()

DEVICE = 'cuda' if torch.cuda.is_available() else 'cpu'

def VisualizeLayer(path):
    directory = os.path.dirname(path)
    if not os.path.exists(directory):
        os.makedirs(directory)

    dataHandler = DataHandler()
    numClasses = dataHandler.numOfClasses

    TEST_NAME = os.getenv('TEST_NAME')
    MODEL_NAME = os.getenv(f'MODEL_NAME_{3}')
    convNext = ModelFactory.LoadModel(MODEL_NAME, numClasses, f"model/{TEST_NAME}/{MODEL_NAME}/best.pth")
    convNext.eval()
    
    for name, module in convNext.named_modules():
        if isinstance(module, nn.Conv2d):
            convWeights = module.weight.detach().numpy()
            imagePath = os.path.join(path, f"{name}_filters.png")
            PlotFilters(imagePath, convWeights)

def AnalyzePretrainedModel(path):
    import torchvision.models
    from torchvision.models import list_models, get_model_weights
    from collections import namedtuple
    import matplotlib.pyplot as plt

    directory = os.path.dirname(path)
    if not os.path.exists(directory):
        os.makedirs(directory)

    ModelConfig = namedtuple('ModelConfig', ['name', 'num_params', 'GFLOPS', 'acc1', 'acc5'])
    classification_models = list_models(module=torchvision.models)

    model_configs = []
    for model_name in classification_models:
        try:
            weights = get_model_weights(model_name)
            num_params = weights.DEFAULT.meta["num_params"]
            GFLOPS = weights.DEFAULT.meta["_ops"]
            acc1 = weights.DEFAULT.meta["_metrics"]["ImageNet-1K"]["acc@1"]
            acc5 = weights.DEFAULT.meta["_metrics"]["ImageNet-1K"]["acc@5"]
            model_config = ModelConfig(model_name, num_params, GFLOPS, acc1, acc5)
            model_configs.append(model_config)
        except:
            pass
        
    acc1_above_80 = [config for config in model_configs if config.acc1 > 80]

    num_params = [config.num_params for config in acc1_above_80]
    gflops = [config.GFLOPS for config in acc1_above_80]
    acc1 = [config.acc1 for config in acc1_above_80]
    model_names = [config.name for config in acc1_above_80]

    # Highlight specific model names
    highlighted_models = ['maxvit_t', 'efficientnet_v2_m', 'swin_v2_b', 'convnext_base']
    highlighted_indices = [model_names.index(name) for name in highlighted_models if name in model_names]

    # Create a list of colors for the scatter plot
    colors = ['blue'] * len(model_names)
    for index in highlighted_indices:
        colors[index] = 'red'

    # Create the first scatter plot
    fig1, ax1 = plt.subplots(figsize=(15, 8))
    ax1.scatter(num_params, acc1, c=colors)
    for i, name in enumerate(model_names):
        if name in highlighted_models:
            ax1.annotate(name, (num_params[i], acc1[i]), xytext=(5, 5), textcoords='offset points', fontsize=12, color='red', fontweight='bold')
        else:
            ax1.annotate(name, (num_params[i], acc1[i]), xytext=(5, 5), textcoords='offset points', fontsize=8)
    ax1.set_xlabel('Number of Parameters')
    ax1.set_ylabel('Top-1 Accuracy')
    ax1.set_title('Number of Parameters vs Top-1 Accuracy')

    # Save the first scatter plot as an image
    plt.savefig(os.path.join(path, 'num_params_vs_top1_acc.png'), dpi=300)
    plt.close()

    # Create the second scatter plot
    fig2, ax2 = plt.subplots(figsize=(15, 8))
    ax2.scatter(gflops, acc1, c=colors)
    for i, name in enumerate(model_names):
        if name in highlighted_models:
            ax2.annotate(name, (gflops[i], acc1[i]), xytext=(5, 5), textcoords='offset points', fontsize=12, color='red', fontweight='bold')
        else:
            ax2.annotate(name, (gflops[i], acc1[i]), xytext=(5, 5), textcoords='offset points', fontsize=8)
    ax2.set_xlabel('GFLOPS')
    ax2.set_ylabel('Top-1 Accuracy')
    ax2.set_title('GFLOPS vs Top-1 Accuracy')

    # Save the second scatter plot as an image
    plt.savefig(os.path.join(path, 'gflops_vs_top1_acc.png'), dpi=300)
    plt.close()

if __name__ == "__main__":
    VisualizeLayer('report/visualizeLayer/')
    AnalyzePretrainedModel('report/selectModel/')