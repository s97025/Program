from datasets import load_dataset

HF_DATASETS_FOLDER="hf_datasets"
HF_HUB_NAME="Amandapu/RNN_Taiwan"

if __name__ == "__main__":
    dataset = load_dataset("audiofolder", data_dir=HF_DATASETS_FOLDER)
    dataset.push_to_hub(HF_HUB_NAME)