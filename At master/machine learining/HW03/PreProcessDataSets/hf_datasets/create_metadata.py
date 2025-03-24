import os
import random
import pandas as pd
from dataclasses import dataclass

DATASETS = '../datasets'

ORIGINAL_TRAIN_DATA_FOLDER = f'{DATASETS}/transformer/train'
ORIGINAL_TEST_DATA_FOLDER = f'{DATASETS}/transformer/test'

# 切割後的結果
TRAIN_DATA_FOLDER = 'data/train'
VALID_DATA_FOLDER = 'data/valid'
TEST_DATA_FOLDER = 'data/test'


TRAIN_ANSWER_PATH = f'{DATASETS}/answer/train.csv'
TEST_ANSWER_PATH = f'{DATASETS}/answer/test.csv'

METADATA_PATH = 'metadata.csv'

TRAIN_VALID_SPLIT_RATE = 0.85
SEED=42

@dataclass
class Metadata:
    wav_id: int
    file_name: str
    transcription: str

    def __repr__(self) -> str:
        return f'{self.file_name} {self.transcription}'

class TranscriptionCollector:
    def __init__(self, transcription_path: str):
        self.transcription_pd = pd.read_csv(transcription_path)               # 讀取 CSV 檔案
        self.transcription_pd['id'] = self.transcription_pd['id'].astype(int) # id 列轉換為整數型別
        self.transcription_pd = self.transcription_pd.set_index('id')         # 設定 'id' 為 DataFrame 的索引

    def get_transcription_by_id(self, id: int) -> str:
        assert self.transcription_pd is not None                              # 確認 self.transcription_pd 不為空
        return self.transcription_pd.loc[id]['text']                          # 根據 id 查找並返回 'text' 

def create_metadata_csv(list_of_metadatas: list[list[Metadata]], metadata_path: str):
     # ensure the directory of metadata_path exists
    metadata_dir = os.path.dirname(metadata_path)
    if not os.path.exists(metadata_dir) and metadata_dir != '':
        os.makedirs(metadata_dir)

    with open(metadata_path, 'w') as f:
        f.write('file_name,transcription\n')
        for metadatas in list_of_metadatas:
            metadatas: list[Metadata]
            for metadata in metadatas:
                metadata: Metadata
                f.write(f'{metadata.file_name},{metadata.transcription}\n')

def create_valid_dataset_by_split_train(train_files: list[str], train_folder: str, valid_folder: str, seed: int):
    if os.path.exists(train_folder):
        # print(f'rm -rf {train_folder}')
        os.system(f'rm -rf {train_folder}')
    if os.path.exists(valid_folder):
        # print(f'rm -rf {valid_folder}')
        os.system(f'rm -rf {valid_folder}')

    os.makedirs(train_folder)
    os.makedirs(valid_folder)
    
    random.seed(seed)
    random.shuffle(train_files)
    split_index = int(len(train_files) * TRAIN_VALID_SPLIT_RATE)
    new_train_files = train_files[:split_index]
    valid_files = train_files[split_index:]

    for f in new_train_files:
        # print(f'cp {f} {train_folder}')
        os.system(f'cp {f} {train_folder}')

    for f in valid_files:
        # print(f'cp {f} {valid_folder}')
        os.system(f'cp {f} {valid_folder}')
        
def create_test_dataset_by_copy(test_files: list[str], test_folder: str):
    if os.path.exists(test_folder):
        # print(f'rm -rf {test_folder}')
        os.system(f'rm -rf {test_folder}')

    os.makedirs(test_folder)
    
    for f in test_files:
        # print(f'cp {f} {test_folder}')
        os.system(f'cp {f} {test_folder}')

if __name__ == '__main__':
    train_transcription_collector = TranscriptionCollector(TRAIN_ANSWER_PATH)
    test_transcription_collector = TranscriptionCollector(TEST_ANSWER_PATH)
    
    # open all original train_data_folder wav file
    train_files = os.listdir(ORIGINAL_TRAIN_DATA_FOLDER)
    train_files = [f for f in train_files if f.endswith('.wav')]
    train_files = [os.path.join(ORIGINAL_TRAIN_DATA_FOLDER, f) for f in train_files]

    # split original train data to train and valid
    create_valid_dataset_by_split_train(train_files, TRAIN_DATA_FOLDER, VALID_DATA_FOLDER, seed=SEED)

    # open all original test_data_folder wav file
    test_files = os.listdir(ORIGINAL_TEST_DATA_FOLDER)
    test_files = [f for f in test_files if f.endswith('.wav')]
    test_files = [os.path.join(ORIGINAL_TEST_DATA_FOLDER, f) for f in test_files]

    # create test data
    create_test_dataset_by_copy(test_files, TEST_DATA_FOLDER)

    # open all train_data_folder wav file
    train_files = os.listdir(TRAIN_DATA_FOLDER)
    train_files = [f for f in train_files if f.endswith('.wav')]
    train_files = [os.path.join(TRAIN_DATA_FOLDER, f) for f in train_files]

    # create metadata for train data
    train_metadata = []
    for f in train_files:
        wav_id = int(os.path.basename(f).split('.')[0])
        transcription = train_transcription_collector.get_transcription_by_id(wav_id)
        metadata = Metadata(wav_id, f, transcription)
        train_metadata.append(metadata)
    
    # open all valid data folder wav file
    valid_files = os.listdir(VALID_DATA_FOLDER)
    valid_files = [f for f in valid_files if f.endswith('.wav')]
    valid_files = [os.path.join(VALID_DATA_FOLDER, f) for f in valid_files]

    # create metadata for valid data
    valid_metadata = []
    for f in valid_files:
        wav_id = int(os.path.basename(f).split('.')[0])
        transcription = train_transcription_collector.get_transcription_by_id(wav_id)
        metadata = Metadata(wav_id, f, transcription)
        valid_metadata.append(metadata)
    
    # open all test_data_folder wav file
    test_files = os.listdir(TEST_DATA_FOLDER)
    test_files = [f for f in test_files if f.endswith('.wav')]
    test_files = [os.path.join(TEST_DATA_FOLDER, f) for f in test_files]

    # create metadata for test data
    test_metadata = []
    for f in test_files:
        wav_id = int(os.path.basename(f).split('.')[0])
        transcription = test_transcription_collector.get_transcription_by_id(wav_id)
        metadata = Metadata(wav_id, f, transcription)
        test_metadata.append(metadata)

    # create metadata csv
    create_metadata_csv([train_metadata, valid_metadata, test_metadata], METADATA_PATH)