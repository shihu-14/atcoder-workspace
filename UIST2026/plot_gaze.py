import argparse
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os
import glob

def main():
    # 1. コマンドライン引数の設定
    ap = argparse.ArgumentParser(description="Gaze time distribution visualizer")
    ap.add_argument("--input_dir", required=True, help="Path to the directory containing aggregated CSV files")
    ap.add_argument("--output_file", default="gaze_distribution_plot.png", help="Output image file name")
    args = ap.parse_args()

    input_dir = args.input_dir
    files = glob.glob(os.path.join(input_dir, "*.csv"))

    if not files:
        print(f"Error: No CSV files found in directory '{input_dir}'")
        return

    # 2. ターゲット列の定義（積み上げる順番: S0, P0, S1, P1, Other）
    target_cols = ['rateTimeS0', 'rateTimeP0', 'rateTimeS1', 'rateTimeP1', 'rateTimeOther']
    display_labels = ['S0', 'P0', 'S1', 'P1', 'Other']
    
    # 色の設定 (S0:薄青, P0:濃青, S1:薄オレンジ, P1:濃オレンジ, Other:グレー)
    colors = ['#aec7e8', '#1f77b4', '#ffbb78', '#ff7f0e', '#7f7f7f']

    results = []

    # 3. データの読み込みと平均値の算出
    for file in files:
        basename = os.path.basename(file)
        # 命名規則から条件とフェーズを抽出 (例: aggregated_stats_A_post-test.csv)
        parts = basename.replace('.csv', '').split('_')
        if len(parts) >= 4:
            cond = parts[2]
            phase = parts[3]
            
            df = pd.read_csv(file)
            
            # 各指標の被験者間平均を算出
            means = df[target_cols].mean().to_dict()
            means['Condition'] = cond
            means['Phase'] = phase
            results.append(means)

    results_df = pd.DataFrame(results)

    # 4. 表示順序の指定
    # 条件の順序と、フェーズの順序（上から pre-test -> post-test -> training となるようにする）
    cond_order = ['A', 'B', 'C'] 
    phase_order = ['pre-test', 'post-test', 'training']

    # 5. グラフの描画設定
    plt.rcParams['font.family'] = 'serif'
    plt.rcParams['mathtext.fontset'] = 'stix'
    plt.rcParams['font.size'] = 12

    fig, ax = plt.subplots(figsize=(10, 8))
    
    y_pos = []
    y_labels = []
    current_y = 0

    # 棒グラフは下から上へ描画されるため、上から表示したい順序の「逆順」でループを回す
    for c in reversed(cond_order):
        for p in reversed(phase_order):
            row = results_df[(results_df['Condition'] == c) & (results_df['Phase'] == p)]
            if not row.empty:
                vals = row[target_cols].values[0]
                
                # 合計を100%に正規化 (NaNが含まれる場合は0として計算)
                total = np.nansum(vals)
                if total > 0:
                    vals_percent = np.nan_to_num(vals) / total * 100
                else:
                    vals_percent = np.zeros(len(vals))
                
                y_pos.append(current_y)
                y_labels.append(f"Cond {c}\n{p}")
                
                # 積み上げ棒グラフの描画
                left = 0
                for i, val in enumerate(vals_percent):
                    if val > 0:
                        ax.barh(current_y, val, left=left, height=0.7, color=colors[i], edgecolor='white', linewidth=0.5)
                        left += val
                current_y += 1
        
        # 異なる条件のグループ間に隙間を空ける
        current_y += 0.8  

    # 6. 軸とラベルの設定
    ax.set_yticks(y_pos)
    ax.set_yticklabels(y_labels)
    ax.set_xlabel('Percentage of Gaze Time (%)')
    ax.set_title('Gaze Time Distribution by Condition and Phase')
    
    # 枠線を非表示にしてスッキリさせる
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.spines['left'].set_visible(False)
    
    # 凡例の設定 (グラフ下部に配置)
    dummy_lines = [plt.Rectangle((0,0),1,1, fc=c, ec='white') for c in colors]
    ax.legend(dummy_lines, display_labels, loc='upper center', bbox_to_anchor=(0.5, -0.1), ncol=3, frameon=False)

    # 7. レイアウト調整と保存
    plt.tight_layout()
    plt.savefig(args.output_file, dpi=300)
    print(f"Saved plot to {args.output_file}")

if __name__ == "__main__":
    main()