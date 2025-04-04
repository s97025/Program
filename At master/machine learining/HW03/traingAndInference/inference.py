from argparse import ArgumentParser, Namespace
from datasets import load_dataset, Audio
from dataclasses import dataclass
from transformers import pipeline
from tqdm import tqdm
import os

@dataclass 
class WavAndTranscription:
    index: str
    audio: dict
    transcription: str

def inference(args: Namespace):
    datas: list[WavAndTranscription] = []

    dataset = load_dataset(args.dataset_name, split=args.inference_split)
    dataset = dataset.cast_column("audio", Audio(sampling_rate=args.sample_rate))

    for data in dataset:
        index = data["audio"]["path"].split('.')[0]
        audio = data["audio"]
        transcription = None 
        datas.append(WavAndTranscription(index, audio, transcription))
    
    transcriber = pipeline("automatic-speech-recognition", model=args.model_name)

    for i in tqdm(range(len(datas))):
        audio = datas[i].audio
        transcription = transcriber(audio)
        datas[i].transcription = transcription["text"]

    # **新增：檢查目錄是否存在，若不存在則創建目錄**
    output_dir = os.path.dirname(args.output_path)
    if output_dir and not os.path.exists(output_dir):
        os.makedirs(output_dir)

    with open(args.output_path, "w") as f:
        f.write("id,text\n")
        for data in datas:
            f.write(f"{data.index},{data.transcription}\n")

if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("--dataset_name", type=str)
    parser.add_argument("--model_name", type=str)
    parser.add_argument("--output_path", type=str)
    parser.add_argument("--inference_split", type=str)
    parser.add_argument("--sample_rate", type=int)
    args = parser.parse_args()
    inference(args)