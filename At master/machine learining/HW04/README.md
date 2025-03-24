# ZH-TW Reading Comprehension Test for LLMs - 機器學習作業四

智能系統研究所 蒲品憶 313581038

## 作業連結:
傳送門: https://www.kaggle.com/competitions/zh-tw-reading-comprehension-test-for-ll-ms/overview

# report

ZH-TW Rading Comprehension_report.pdf

## Installation

Install the required packages.

    ```
    python3 -m venv venv
    cd venv/Scripts
    activate.bat
    Follow requirements.txt to install packages
    ```
    
## Create DataSets

1. Put your dataset in to the place you want

2. python `build_dataset.py`
    - train.json
    - test.json (plus id: "" )

    json format:
    ```
    {
        "instruction": "男：請問，苗栗的縣立圖書館在哪裡？\n女：就沿著門口那條路走到底，再向左轉就到了。\n男：會很難找嗎？\n女：不會。而且那裏有U-BIKE，你可以騎著繞一繞市區。可以到鐵道博物館走一走。\n男：好的，謝謝你告訴我。\n女：其實苗栗市區不大。\n男：市區是不大，但房子都蠻有歷史感的，我很喜歡。",
        "question": "這位先生最欣賞苗栗什麼？\nOptions\n1) 有歷史感的房子。\n2) 方便的U-BIKE。\n3) 圖書館的藏書。\n4) 鐵道博物館。",
        "output": ""
    },
    ```

## Training and inference

1. Training
    ```
    jupyter nbconvert --to script Alpaca_training.ipynb
    python Alpaca_training.py
    ```


2. inference
    ```
    jupyter nbconvert --to script Alpaca_inference.ipynb
    python Alpaca_inference.py
    ```

## Reference

1. [Taiwan-LLM: Language Models for Taiwanese Culture](https://github.com/MiuLab/Taiwan-LLM)
2. [Fine-Tune_LLaMA_3](https://www.confident-ai.com/blog/the-ultimate-guide-to-fine-tune-llama-2-with-llm-evaluations)
3. [LLaMA3_Unsloth_ipynb](https://colab.research.google.com/drive/135ced7oHytdxu3N2DNe1Z0kqjyYIkDXp?usp=sharing#scrollTo=e2pEuRb1r2Vg)