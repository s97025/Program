import torch
import torchvision
torchvision.disable_beta_transforms_warning()
from torchvision import transforms
import torchvision.transforms.v2 as T

# 自訂 transform to add Gaussian noise
class AddGaussianNoise(object):
    def __init__(self, mean=0., std=1.):
        self.std = std
        self.mean = mean

    def __call__(self, tensor):
        return tensor + torch.randn(tensor.size()) * self.std + self.mean

# 自訂 transform to add Speckle noise
class AddSpeckleNoise(object):
    def __init__(self, noise_level=0.1):
        self.noise_level = noise_level

    def __call__(self, tensor):

        # 生成 Speckle noise
        noise = torch.randn_like(tensor) * self.noise_level

        # 在圖像中加入 Speckle noise
        noisy_tensor = tensor * (1 + noise)

        # 裁切將數值限制在[0,1]
        noisy_tensor = torch.clamp(noisy_tensor, 0, 1)

        return noisy_tensor
    
# 自訂 transform to add Poisson noise
class AddPoissonNoise(object):
    def __init__(self, lam=1.0):
        self.lam = lam

    def __call__(self, tensor):
        # 生成 Poisson noise
        noise = torch.poisson(self.lam * torch.ones(tensor.shape))

        # 在圖像中加入 Poisson noise 
        noisy_tensor = tensor + noise / 255.0  # Assuming the image is scaled between 0 and 1

        # 裁切將數值限制在[0,1]
        noisy_tensor = torch.clamp(noisy_tensor, 0, 1)

        return noisy_tensor
    
# 自訂 transform to add Salt and Pepper noise
class AddSaltPepperNoise(object):
    def __init__(self, salt_prob=0.05, pepper_prob=0.05):
        self.salt_prob = salt_prob
        self.pepper_prob = pepper_prob

    def __call__(self, tensor):
        noise = torch.rand(tensor.size())
        # 複製 tensor 避免 deprecated behavior (重要!!)
        tensor = tensor.clone()

        tensor[(noise < self.salt_prob)] = 1  # Salt noise: 將部分像素設為 1 ( 鹽噪聲 - 白色點 )
        tensor[(noise > 1 - self.pepper_prob)] = 0  # Pepper noise: 將部分像素設為 0 ( 鹽噪聲 - 黑色點 )
        return tensor
    
def GetTransform():
    transform = transforms.Compose([
        # 尺寸調整 
        transforms.Resize((224, 224)),

        # 將圖像轉為 Tensor 
        transforms.ToTensor(),

        # 空間操作
        T.RandomApply([T.RandomHorizontalFlip()], p=0.1),
        T.RandomApply([T.RandomVerticalFlip()], p=0.1),
        T.RandomApply([T.RandomRotation(10)], p=0.1),
        T.RandomApply([T.RandomPerspective(distortion_scale=0.6, p=1.0)], p=0.1),
        T.RandomApply([T.RandomAffine(degrees=(30, 70), translate=(0.1, 0.3), scale=(0.5, 0.75))], p=0.1),
        T.RandomApply([T.ElasticTransform(alpha=250.0)], p=0.1),

        # 顏色操作
        T.RandomApply([T.ColorJitter(brightness=0.4, contrast=0.4, saturation=0.4, hue=0.1)], p=0.1),
        T.RandomGrayscale(p=0.1),
        T.RandomInvert(p=0.1),
        T.RandomPosterize(bits=2, p=0.1),
        T.RandomApply([T.RandomSolarize(threshold=1.0)], p=0.05),
        T.RandomApply([T.RandomAdjustSharpness(sharpness_factor=2)], p=0.1),

        # 噪聲操作
        T.RandomApply([AddGaussianNoise(mean=0.0, std=0.05)], p=0.1), 
        T.RandomApply([AddPoissonNoise(lam=0.1)], p=0.1),  # lam: 期望值
        T.RandomApply([AddSpeckleNoise(noise_level=0.1)], p=0.1),
        T.RandomApply([AddSaltPepperNoise(salt_prob=0.05, pepper_prob=0.05)], p=0.1),

        # 高斯模糊
        T.RandomApply([T.GaussianBlur(kernel_size=(5, 9), sigma=(0.1, 5.))], p=0.1),
        
        # 標準化 Normalize [-1,1]  
        transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])  
    ])

    return transform

def GetTestTransform():
    test_transform = transforms.Compose([
        # 尺寸調整 
        transforms.Resize((224, 224)),
        
        # 將圖像轉為 Tensor 
        transforms.ToTensor(),
        
        # 標準化 Normalize [-1,1]  
        transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225]) 
    ])

    return test_transform