import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import os

# ==========================================
# 設定箇所：ここを変更するだけで他の項目も出力可能です
# ==========================================
# 解析対象のディレクトリ（CSVファイルがある場所）
# 必要に応じてパスを変更してください（例: "./data" や "."）
TARGET_DIR = "/Users/eiichi/Desktop/AHdata/passrate"

# 箱ひげ図を作成したい項目名（CSVのヘッダーと完全一致させる必要があります）
# 変更例: "long turn rate", "tight turn rate", "total turn rate"
TARGET_METRIC = "shortRate"

# ==========================================
# データ読み込みと統合
# ==========================================
def load_and_process_data():
    conditions = ['A', 'B', 'C']
    times = ['before', 'after']
    
    all_data = []

    for condition in conditions:
        for time in times:
            filename = f"passrate_{condition}_{time}.csv"
            filepath = os.path.join(TARGET_DIR, filename)
            
            if not os.path.exists(filepath):
                print(f"Warning: File not found: {filename}")
                continue
            
            try:
                # CSVを読み込む
                df = pd.read_csv(filepath)
                
                # 指定した項目(TARGET_METRIC)が含まれているか確認
                if TARGET_METRIC in df.columns:
                    # 解析用にデータを抽出して整形
                    # 値だけのSeriesを取り出し、ラベル（条件と時期）を付与してDataFrame化
                    temp_df = pd.DataFrame({
                        "Value": df[TARGET_METRIC],
                        "Condition": condition,
                        "Time": time
                    })
                    all_data.append(temp_df)
                else:
                    print(f"Warning: Column '{TARGET_METRIC}' not found in {filename}")
                    
            except Exception as e:
                print(f"Error reading {filename}: {e}")

    # 全データを1つのDataFrameに結合
    if all_data:
        return pd.concat(all_data, ignore_index=True)
    else:
        return pd.DataFrame()

# ==========================================
# 箱ひげ図の描画
# ==========================================
def plot_boxplot(df):
    if df.empty:
        print("No data available to plot.")
        return

    # グラフのスタイル設定（論文調のシンプルなスタイル）
    sns.set_style("whitegrid")
    plt.figure(figsize=(10, 6))

    # 箱ひげ図の描画
    # x軸に条件(Condition)、y軸に値(Value)、色分け(hue)で時期(Time)を指定
    ax = sns.boxplot(
        x="Condition", 
        y="Value", 
        hue="Time", 
        data=df, 
        palette="Set2",     # 色のセット（好みで変更可）
        width=0.6,          # 箱の幅
        order=['A', 'B', 'C'],       # X軸の並び順
        hue_order=['before', 'after'] # 色分けの並び順
    )

    # タイトルとラベルの設定
    plt.title(f"Boxplot of {TARGET_METRIC}", fontsize=15)
    plt.ylabel(TARGET_METRIC, fontsize=12)
    plt.xlabel("Condition", fontsize=12)
    
    # 凡例のタイトルなどを調整
    plt.legend(title="Time")

    # グリッド線を背面に
    ax.set_axisbelow(True)

    # 保存と表示
    output_filename = f"boxplot_{TARGET_METRIC.replace(' ', '_')}.png"
    plt.savefig(output_filename, dpi=300, bbox_inches='tight')
    print(f"Graph saved as: {output_filename}")
    plt.show()

# ==========================================
# メイン実行処理
# ==========================================
if __name__ == "__main__":
    # 1. データを読み込む
    merged_df = load_and_process_data()
    
    # 2. グラフを描画する
    plot_boxplot(merged_df)