import torch 
from torch.utils.data import DataLoader, Dataset
from PIL import Image
from torchvision import datasets
from dataPreprocess import GetTransform, GetTestTransform
import os 
from dotenv import load_dotenv
load_dotenv()


TRAIN_DATA_DIR = os.getenv('TRAIN_DATA_DIR')
TEST_DATA_DIR = os.getenv('TEST_DATA_DIR')
TRAIN_VAL_RATIO = float(os.getenv('TRAIN_VAL_RATIO'))
BATCH_SIZE = int(os.getenv('BATCH_SIZE'))


# test dataset
class TestDataset(Dataset):
    def __init__(self, rootDir, transform=None):
        self.rootDir = rootDir
        self.transform = transform
        self.imageFiles = [f for f in os.listdir(rootDir) if os.path.isfile(os.path.join(rootDir, f))]


    def __len__(self):
        return len(self.imageFiles)

    def __getitem__(self, idx):
        img_path = os.path.join(self.rootDir, self.imageFiles[idx])
        image = Image.open(img_path).convert('RGB')
        if self.transform:
            image = self.transform(image)
        return image, self.imageFiles[idx]
    


class DataHandler:
    def __init__(self):
        originImageDataset = datasets.ImageFolder(root=TRAIN_DATA_DIR, transform = GetTransform())
        self.classes = originImageDataset.classes.copy()
        self.numOfClasses = len(self.classes)

        # split originImageDataset into train and valid dataset
        trainSize = int(TRAIN_VAL_RATIO * len(originImageDataset))
        validSize = len(originImageDataset) - trainSize
        trainDataset, validDataset = torch.utils.data.random_split(originImageDataset, [trainSize, validSize])
        self.trainLoader = DataLoader(trainDataset, batch_size=BATCH_SIZE, shuffle=True)
        self.validLoader = DataLoader(validDataset, batch_size=BATCH_SIZE, shuffle=False)
        

        # Loader test
        testDataset = TestDataset(rootDir=TEST_DATA_DIR, transform=GetTestTransform())
        self.testLoader = DataLoader(testDataset, batch_size=BATCH_SIZE, shuffle=False)


