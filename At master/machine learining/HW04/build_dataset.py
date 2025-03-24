import pandas as pd
import json

# trian
def clean_correct_answer(answer):
    if isinstance(answer, str) and answer.startswith('\u9078\u9805'):
        return int(answer[-1])  # Extract the numeric part from "選項X"
    try:
        return int(answer)  # Convert directly to integer if already numeric
    except ValueError:
        return None  # Return None for invalid entries

def train_excel_to_json(df):
    data_list = []
    for _, row in df.iterrows():
        instruction = row['文章']
        question = f"{row['問題']}\nOptions\n1) {row['選項1']}\n2) {row['選項2']}\n3) {row['選項3']}\n4) {row['選項4']}"
        correct_option = row['正確答案']
        if correct_option is not None:
            correct_answer = f"{correct_option}) {row[f'選項{correct_option}']}"
        else:
            correct_answer = ""

        json_entry = {
            "instruction": instruction,
            "question": question,
            "output": correct_answer
        }
        data_list.append(json_entry)

    return data_list

# Path to the input Excel file
file_name = 'train'
file_path = f'{file_name}.xlsx'
# Load the Excel file
df = pd.read_excel(file_path)

# Clean up the "正確答案" column
df['正確答案'] = df['正確答案'].apply(clean_correct_answer)

# Convert the data to JSON format
json_result = train_excel_to_json(df)

# Save the result to a JSON file
output_path = f'{file_name}.json'
with open(output_path, 'w', encoding='utf-8') as json_file:
    json.dump(json_result, json_file, ensure_ascii=False, indent=2)

print(f"JSON data has been saved to {output_path}")


# test
def test_excel_to_json(df):
    data_list = []
    for _, row in df.iterrows():
        #id = row['題號']
        instruction = row['文章']
        question = f"{row['問題']}\nOptions\n1) {row['選項1']}\n2) {row['選項2']}\n3) {row['選項3']}\n4) {row['選項4']}"

        json_entry = {
            #'id': id,
            "instruction": instruction,
            "question": question,
            "output": ""
        }
        data_list.append(json_entry)

    return data_list

# Path to the input Excel file
file_name = 'test'
file_path = f'{file_name}.xlsx'
# Load the Excel file
df = pd.read_excel(file_path)


# Convert the data to JSON format
json_result = test_excel_to_json(df)

# Save the result to a JSON file
output_path = f'{file_name}.json'
with open(output_path, 'w', encoding='utf-8') as json_file:
    json.dump(json_result, json_file, ensure_ascii=False, indent=2)

print(f"JSON data has been saved to {output_path}")
