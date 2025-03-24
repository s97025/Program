# Taiwan_RNN - 機器學習作業三

**智能系統研究所 蒲品憶 313581038**

## 作業連結:
* 傳送門: https://www.kaggle.com/competitions/zh-tw-reading-comprehension-test-for-ll-ms/data

## Installation

1. ffmpeg
    ```bash
    sudo apt-get install ffmpeg
    ```

2. sox
    ```bash
    sudo apt-get install sox
    ```


3. Install the required packages and set access token
    ```bash
    python3 -m venv venv
    source venv/bin/activate
    pip install -r requirements.txt
    pip install -U "huggingface_hub[cli]"
    huggingface-cli login # use your huggingface account WRITE token
    ```


## PreProcess DataSets

1. Put your dataset in to the place you want

2. Transform your dataset into 16K sampling rate if you want

    - Modify `transform_wav_to_16k.sh` to your own path
    - Run `bash transform_wav_to_16k.sh`

3. Noise your train dataset if you want
    
    - Modify `add_noise.py` to your own path
    - Run `python add_noise.py`

4. Create Huggingface metadata and copy your dataset into the Huggingface dataset folder

    - Go to `hf_datasets` folder

    - Modify `create_metadata.py` to your own path

    ```bash
    python create_metadata.py
    ```

5. After creating metadata, you can upload your dataset to Huggingface

    - Modify `create_datasets.py` to your own path and your own dataset name

    ```bash
    python create_datasets.py
    ```


## Training and inference

1. Training
    1. Modify config of `training.sh`

        - `MODEL_NAME_OR_PATH` : The pretrained model name you want to finetune.
        - `DATASET_NAME` : The dataset name you want to finetune.
        - `MAX_STEPS` : The maximum steps of training.
        - `PER_DEVICE_TRAIN_BATCH_SIZE` : The batch size of training.
        - `PER_DEVICE_EVAL_BATCH_SIZE` : The batch size of evaluation.
        - `LEARNING_RATE` : The learning rate of training.
        - `WARMUP_STEPS` : The warmup steps of training.
        - `EVAL_STEPS` : The evaluation steps of training.
        - `SAVE_STEPS` : The model saving steps of training.
        - `GENERATION_MAX_LENGTH` : The maximum length of text generation.
        - `MODEL_INDEX_NAME` : The model name you want to push to huggingface hub.

    2. Run the following command to train the model
        ```bash
        bash training.sh
        ```
2. inference

    1. Modify config of `inference.sh`

        - `MODEL_NAME` : The model name you want to decode, which is the same as the model you push to huggingface hub.
        - `OUTPUT_PATH` : The output path of the decoded text.

    2. Run the following command to decode the model
        ```bash
        bash inference.sh
        ```

## Reference

1. [Huggingface Doc](https://huggingface.co/docs/datasets/audio_dataset)
2. [openai whisper](https://github.com/openai/whisper)
3. [Whisper Finetuning event](https://github.com/huggingface/community-events/tree/main/whisper-fine-tuning-event)