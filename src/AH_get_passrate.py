import os
import csv
import sys
from collections import defaultdict

def process_csv_files(input_directory):
    # 条件のマッピング (0->C, 1->A, 3->B)
    condition_map = {
        '1': 'A',
        '3': 'B',
        '0': 'C'
    }
    
    valid_seeds = {'before', 'after'}
    grouped_data = defaultdict(list)
    headers_map = defaultdict(set)

    print(f"Scanning directory: {input_directory}") # ログ追加

    for root, dirs, files in os.walk(input_directory):
        for filename in files:
            if not filename.startswith('gazepos_') or not filename.endswith('.csv'):
                continue
            
            name_parts = filename.replace('.csv', '').split('_')
            
            if len(name_parts) != 4:
                continue
            
            _, _, cond_val, seed_val = name_parts
            
            if seed_val not in valid_seeds:
                continue
            
            if cond_val not in condition_map:
                continue
            
            condition_label = condition_map[cond_val]
            
            file_path = os.path.join(root, filename)
            row_data = {}
            
            try:
                with open(file_path, 'r', encoding='utf-8', errors='replace') as f:
                    reader = csv.reader(f)
                    for row in reader:
                        if len(row) < 3:
                            continue
                        if row[0] == 'RESULT':
                            row_data[row[1]] = row[2]
            except Exception as e:
                print(f"Error reading {filename}: {e}")
                continue

            if row_data:
                key = (condition_label, seed_val)
                grouped_data[key].append(row_data)
                headers_map[key].update(row_data.keys())

    # --- 集計結果の書き込み ---
    if not grouped_data:
        print("No matching files found. Check your filenames.")
        return

    for (cond, seed), data_list in grouped_data.items():
        output_filename = f"passrate_{cond}_{seed}.csv"
        
        # ★変更点: 入力ディレクトリの中に出力するようにパスを結合
        output_path = os.path.join(input_directory, output_filename)
        
        fieldnames = sorted(list(headers_map[(cond, seed)]))
        
        try:
            with open(output_path, 'w', encoding='utf-8', newline='') as f:
                writer = csv.DictWriter(f, fieldnames=fieldnames)
                writer.writeheader()
                writer.writerows(data_list)
            print(f"Created: {output_path} ({len(data_list)} records)")
        except Exception as e:
            print(f"Error writing {output_filename}: {e}")

if __name__ == "__main__":
    if len(sys.argv) > 1:
        target_dir = sys.argv[1]
    else:
        target_dir = "."
        
    process_csv_files(target_dir)