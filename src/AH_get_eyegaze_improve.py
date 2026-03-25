import os
import csv
import sys
from collections import defaultdict

def process_gaze_analysis_files(input_directory):
    # 条件のマッピング
    # 1 -> T (Task-based), 3 -> A (Adaptive), 0 -> B (Baseline)
    condition_map = {
        '1': 'T',
        '3': 'A',
        '0': 'B'
    }
    
    valid_seeds = {'before', 'after'}
    
    # データを格納する辞書
    # data_store[Condition][ParticipantID][seed] = {MetricName: Value, ...}
    data_store = defaultdict(lambda: defaultdict(dict))
    
    # 抽出・計算対象の項目定義
    # ここではキーを内部識別用に定義
    METRIC_COUNT_RATE = 'two ahead flag count rate'
    METRIC_ONE_TIME = 'one ahead flag time rate'
    METRIC_TWO_TIME = 'two ahead flag time rate'
    METRIC_GROUND_TIME = 'ahead ground time rate'
    
    # 出力するヘッダー順序
    output_headers = [
        'ParticipantID',
        METRIC_COUNT_RATE,
        METRIC_ONE_TIME,
        METRIC_TWO_TIME,
        METRIC_GROUND_TIME
    ]

    print(f"Scanning directory: {input_directory}")

    # --- 1. ファイル走査とデータ読み込み ---
    for root, dirs, files in os.walk(input_directory):
        for filename in files:
            # ファイル名の基本チェック
            if not filename.startswith('analysis_') or not filename.endswith('.csv'):
                continue
            
            # ファイル名解析: analysis_{id}_{condition}_{seed}.csv
            name_parts = filename.replace('.csv', '').split('_')
            
            if len(name_parts) != 4:
                continue
            
            _, p_id, cond_val, seed_val = name_parts
            
            # 条件とSeedのフィルタリング
            if cond_val not in condition_map:
                continue
            if seed_val not in valid_seeds:
                continue
            
            condition_label = condition_map[cond_val]
            file_path = os.path.join(root, filename)
            
            # CSV読み込み
            raw_data = {}
            try:
                with open(file_path, 'r', encoding='utf-8', errors='replace') as f:
                    # 区切り文字がカンマかタブか不明ですが、提示例に従いcsv.reader(デフォルトカンマ)を使用
                    # もしタブ区切りの場合は delimiter='\t' を追加してください
                    reader = csv.reader(f)
                    for row in reader:
                        # "key, value" の形式を想定 (len >= 2)
                        if len(row) >= 2:
                            # キーの空白除去などを念のため行う
                            key = row[0].strip()
                            val = row[1].strip()
                            raw_data[key] = val
            except Exception as e:
                print(f"Error reading {filename}: {e}")
                continue

            # 必要なデータの抽出と計算
            try:
                # 1. Two ahead flag count rate の計算
                total_flags = float(raw_data.get('total flags', 0))
                two_ahead_count = float(raw_data.get('two ahead flag count', 0))
                
                if total_flags > 0:
                    calc_count_rate = two_ahead_count / total_flags
                else:
                    calc_count_rate = 0.0

                # 2. その他のRateの抽出
                one_ahead_time_rate = float(raw_data.get('one ahead flag time rate', 0))
                two_ahead_time_rate = float(raw_data.get('two ahead flag time rate', 0))
                ahead_ground_time_rate = float(raw_data.get('ahead ground time rate', 0))

                # データストアに保存
                data_store[condition_label][p_id][seed_val] = {
                    METRIC_COUNT_RATE: calc_count_rate,
                    METRIC_ONE_TIME: one_ahead_time_rate,
                    METRIC_TWO_TIME: two_ahead_time_rate,
                    METRIC_GROUND_TIME: ahead_ground_time_rate
                }
                
            except ValueError as e:
                # 数値変換エラーなど
                print(f"Skipping {filename}: Data format error ({e})")
                continue

    # --- 2. 改善量の計算とCSV出力 ---
    if not data_store:
        print("No matching files found.")
        return

    # 計算対象のメトリクスリスト（IDを除く）
    target_metrics = output_headers[1:]

    for condition, participants in data_store.items():
        output_rows = []
        
        # 参加者IDでソートして処理
        sorted_p_ids = sorted(participants.keys())
        
        for p_id in sorted_p_ids:
            seeds = participants[p_id]
            
            # BeforeとAfterが揃っている場合のみ計算
            if 'before' in seeds and 'after' in seeds:
                before_vals = seeds['before']
                after_vals = seeds['after']
                
                row = {'ParticipantID': p_id}
                
                for metric in target_metrics:
                    val_b = before_vals[metric]
                    val_a = after_vals[metric]
                    
                    # 向上率 = After - Before
                    diff = val_a - val_b
                    row[metric] = diff
                
                output_rows.append(row)
            else:
                # 片方しかデータがない場合はスキップ（ログ出力等は任意）
                pass
        
        # 条件ごとのファイル出力
        if output_rows:
            output_filename = f"gaze_improvement_{condition}.csv"
            output_path = os.path.join(input_directory, output_filename)
            
            try:
                with open(output_path, 'w', encoding='utf-8', newline='') as f:
                    writer = csv.DictWriter(f, fieldnames=output_headers)
                    writer.writeheader()
                    writer.writerows(output_rows)
                print(f"Created: {output_path} ({len(output_rows)} records)")
            except Exception as e:
                print(f"Error writing {output_filename}: {e}")
        else:
            print(f"No complete data pairs found for Condition {condition}")

if __name__ == "__main__":
    if len(sys.argv) > 1:
        target_dir = sys.argv[1]
    else:
        # 引数がない場合はカレントディレクトリ
        target_dir = "."
        
    process_gaze_analysis_files(target_dir)