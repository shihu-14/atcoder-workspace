import os
import csv
import sys
from collections import defaultdict

def process_csv_files(input_directory):
    # 条件のマッピング
    condition_map = {
        '1': 'A',
        '3': 'B',
        '0': 'C'
    }
    
    valid_seeds = {'before', 'after'}
    
    # データを整理するための辞書
    # 構造: data_store[条件][参加者ID][before/after] = {項目名: 値, ...}
    data_store = defaultdict(lambda: defaultdict(dict))
    
    # 全ての項目名を把握するためのセット（条件ごとに管理）
    headers_map = defaultdict(set)

    print(f"Scanning directory: {input_directory}")

    # --- 1. データの読み込み ---
    for root, dirs, files in os.walk(input_directory):
        for filename in files:
            if not filename.startswith('gazepos_') or not filename.endswith('.csv'):
                continue
            
            name_parts = filename.replace('.csv', '').split('_')
            
            if len(name_parts) != 4:
                continue
            
            # ファイル名から ID を取得するように変更 (index 1)
            _, p_id, cond_val, seed_val = name_parts
            
            if seed_val not in valid_seeds:
                continue
            
            if cond_val not in condition_map:
                continue
            
            condition_label = condition_map[cond_val]
            file_path = os.path.join(root, filename)
            
            # 1ファイルのデータを辞書として読み込む
            row_data = {}
            try:
                with open(file_path, 'r', encoding='utf-8', errors='replace') as f:
                    reader = csv.reader(f)
                    for row in reader:
                        if len(row) < 3:
                            continue
                        # "RESULT" 行のデータを取得
                        if row[0] == 'RESULT':
                            # key: 項目名, value: 数値
                            row_data[row[1]] = row[2]
            except Exception as e:
                print(f"Error reading {filename}: {e}")
                continue

            if row_data:
                # 条件 > ID > seed(before/after) の階層で保存
                data_store[condition_label][p_id][seed_val] = row_data
                # 項目名を記録（CSVヘッダー用）
                headers_map[condition_label].update(row_data.keys())

    # --- 2. 改善量 (After - Before) の計算と書き出し ---
    if not data_store:
        print("No matching files found. Check your filenames.")
        return

    for condition, participants in data_store.items():
        calculated_rows = []
        
        # この条件における全項目リスト
        field_keys = sorted(list(headers_map[condition]))
        
        for p_id, seeds in participants.items():
            # BeforeとAfterの両方が揃っている場合のみ計算
            if 'before' in seeds and 'after' in seeds:
                before_data = seeds['before']
                after_data = seeds['after']
                
                # 結果行の作成（最初はIDを入れる）
                diff_row = {'ParticipantID': p_id}
                
                for key in field_keys:
                    val_before = before_data.get(key)
                    val_after = after_data.get(key)
                    
                    # 両方の値が存在し、かつ数値変換できる場合のみ計算
                    if val_before is not None and val_after is not None:
                        try:
                            # 改善量 = After - Before
                            diff = float(val_after) - float(val_before)
                            diff_row[key] = diff
                        except ValueError:
                            # 数値でない場合は計算スキップ（空欄またはそのまま）
                            diff_row[key] = ''
                    else:
                        diff_row[key] = ''
                
                calculated_rows.append(diff_row)
            else:
                print(f"Skipping ID {p_id} in Condition {condition}: Missing 'before' or 'after' data.")

        # --- CSV出力 ---
        if calculated_rows:
            # ID順にソート（オプション）
            calculated_rows.sort(key=lambda x: x['ParticipantID'])
            
            output_filename = f"improvement_diff_{condition}.csv"
            output_path = os.path.join(input_directory, output_filename)
            
            # ヘッダーの先頭にIDを追加
            output_fieldnames = ['ParticipantID'] + field_keys
            
            try:
                with open(output_path, 'w', encoding='utf-8', newline='') as f:
                    writer = csv.DictWriter(f, fieldnames=output_fieldnames)
                    writer.writeheader()
                    writer.writerows(calculated_rows)
                print(f"Created: {output_path} ({len(calculated_rows)} records)")
            except Exception as e:
                print(f"Error writing {output_filename}: {e}")
        else:
            print(f"No complete pairs (before/after) found for Condition {condition}.")

if __name__ == "__main__":
    if len(sys.argv) > 1:
        target_dir = sys.argv[1]
    else:
        target_dir = "."
        
    process_csv_files(target_dir)