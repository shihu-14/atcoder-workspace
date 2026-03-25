import argparse
import pandas as pd
import numpy as np
import os
import glob

def main():
    # 1. コマンドライン引数の設定
    ap = argparse.ArgumentParser(description="Generate a summary table for specific statistical metrics.")
    ap.add_argument("--input_dir", required=True, help="Path to the directory containing aggregated CSV files")
    ap.add_argument("--output_file", default="metrics_summary_table.csv", help="Output CSV file name for the table")
    args = ap.parse_args()

    input_dir = args.input_dir
    files = glob.glob(os.path.join(input_dir, "*.csv"))

    if not files:
        print(f"Error: No CSV files found in directory '{input_dir}'")
        return

    # 2. 抽出対象の指標リスト
    target_cols = [
        'passRate', 
        'longRate', 
        'tightRate', 
        'mean_minDistToFlag', 
        'mean_lastRiseSec', 
        'mean_p1Duration', 
        'mean_p1CountPerSection'
    ]

    results = []

    # 3. データの読み込みと被験者間平均の算出
    for file in files:
        basename = os.path.basename(file)
        # 命名規則から条件とフェーズを抽出 (例: aggregated_stats_A_post-test.csv)
        parts = basename.replace('.csv', '').split('_')
        if len(parts) >= 4:
            cond = parts[2]
            phase = parts[3]
            
            try:
                df = pd.read_csv(file)
                # 必要なカラムのみ抽出し、NaNを除外して平均を計算
                means = df[target_cols].mean(skipna=True).to_dict()
                means['Condition'] = cond
                means['Phase'] = phase
                results.append(means)
            except Exception as e:
                print(f"Error processing {file}: {e}")

    # 4. データフレーム化と順序の整理
    results_df = pd.DataFrame(results)

    if results_df.empty:
        print("No valid data could be extracted.")
        return

    # 条件の順序と、ご指示に基づいたフェーズの順序
    cond_order = ['A', 'B', 'C'] 
    phase_order = ['pre-test', 'post-test', 'training']

    results_df['Condition'] = pd.Categorical(results_df['Condition'], categories=cond_order, ordered=True)
    results_df['Phase'] = pd.Categorical(results_df['Phase'], categories=phase_order, ordered=True)
    
    # 条件、フェーズの順でソートし、見やすいようにインデックスを設定
    results_df = results_df.sort_values(['Condition', 'Phase'])
    
    # カラムの並び順を整える
    cols_order = ['Condition', 'Phase'] + target_cols
    results_df = results_df[cols_order]

    # 数値のフォーマット（小数点以下3桁に丸めるなど、視認性向上のための処理）
    for col in target_cols:
        results_df[col] = results_df[col].round(3)

    # 5. 結果の出力
    # CSVとして保存（論文等の表作成に利用しやすい形式）
    results_df.to_csv(args.output_file, index=False, encoding="utf-8-sig")
    print(f"\nSuccessfully saved the summary table to: {args.output_file}\n")

    # ターミナル上にも整形して表示
    print("=== Metrics Summary Table ===")
    
    # Pandasの表示設定を変更して省略されずに全て表示させる
    pd.set_option('display.max_columns', None)
    pd.set_option('display.width', 1000)
    
    # ConditionとPhaseをマルチインデックスにして綺麗な表組みで出力
    display_df = results_df.set_index(['Condition', 'Phase'])
    print(display_df)
    print("=============================")

if __name__ == "__main__":
    main()