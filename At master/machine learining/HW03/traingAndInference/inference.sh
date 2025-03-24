DATASET_NAME="Amandapu/RNN_Taiwan"
MODEL_NAME="exp/whisper-medium-taiwanese-asr-v2/checkpoint-600"
OUTPUT_PATH="submission/whisper-medium-tawiwanese-asr-600.csv"
INFERENCE_SPLIT="test"
SAMPLE_RATE=16000

python inference.py \
    --model_name $MODEL_NAME \
    --dataset_name $DATASET_NAME \
    --output_path $OUTPUT_PATH \
    --inference_split $INFERENCE_SPLIT \
    --sample_rate $SAMPLE_RATE