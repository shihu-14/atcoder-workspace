import pandas as pd
import os
import matplotlib.pyplot as plt

# ==========================================
# 設定箇所
# ==========================================
# CSVファイルがあるディレクトリ
TARGET_DIR = os.path.expanduser("/Users/eiichi/Desktop/AHdata/improve_rate")

# CSVの列名と、表での表示名の対応
COLUMN_MAPPING = {
    "Short Turn": "shortRate",
    "Long Turn":  "longRate",
    "Tight Turn": "tightRate",
    "Total": "passRate"
}

# 列の幅設定 [Condition, Short, Long, Tight, Total]
# 項目数が減ったのでバランスを調整
col_widths = [0.15, 0.15, 0.15, 0.15, 0.15]

# ==========================================
# データ集計処理 (差分用)
# ==========================================
def generate_diff_summary_table():
    # ファイル名に使われている条件ID
    conditions = ['C', 'A', 'B']
    
    # 表示用のマッピング (A->T, B->A, C->B)
    cond_display_map = { 'C': 'B', 'A': 'T', 'B': 'A'}
    
    rows = []

    for condition in conditions:
        # ファイル名: improvement_diff_A.csv など
        filename = f"improvement_diff_{condition}.csv"
        filepath = os.path.join(TARGET_DIR, filename)
        
        row_data = {
            "Condition": cond_display_map[condition]
        }
        
        if not os.path.exists(filepath):
            # ファイルがない場合は "-" で埋める
            for display_name in COLUMN_MAPPING.keys():
                row_data[display_name] = "-"
            rows.append(row_data)
            continue
        
        try:
            df = pd.read_csv(filepath)
            
            # 各項目について平均と標準偏差を計算
            for display_name, csv_col_name in COLUMN_MAPPING.items():
                if csv_col_name in df.columns:
                    # 空文字や非数値を排除して計算
                    vals = pd.to_numeric(df[csv_col_name], errors='coerce').dropna()
                    
                    if not vals.empty:
                        mean_val = vals.mean()
                        sd_val = vals.std()
                        # フォーマット: "Mean (SD)"
                        formatted_val = f"{mean_val:.2f} ({sd_val:.2f})"
                    else:
                        formatted_val = "N/A"
                    
                    row_data[display_name] = formatted_val
                else:
                    row_data[display_name] = "N/A"
            
            rows.append(row_data)

        except Exception as e:
            print(f"Error processing {filename}: {e}")

    result_df = pd.DataFrame(rows)
    # 列の並び順を固定 (Condition, Short, Long, Tight, Total)
    cols_order = ["Condition"] + list(COLUMN_MAPPING.keys())
    result_df = result_df[cols_order]
    
    return result_df

# ==========================================
# 論文用テーブル画像生成関数 (差分用)
# ==========================================
def save_diff_table_as_image(df, output_filename="table_diff_summary.png"):
    if df.empty:
        print("No data to plot.")
        return

    # 1. フォント設定
    plt.rcParams["font.family"] = "serif"
    plt.rcParams["font.serif"] = ["Times New Roman"]
    plt.rcParams["font.size"] = 12

    # 2. 図の描画準備
    # 行数が少ないため高さを調整
    w, h = 10, 3  
    fig, ax = plt.subplots(figsize=(w, h))
    
    # 軸を消す
    ax.axis('off')

    # 3. テーブルの作成
    table = ax.table(
        cellText=df.values,
        colLabels=df.columns,
        loc='center',
        cellLoc='center',
        colWidths=col_widths,
        edges='open'
    )

    # 4. スタイルの詳細設定
    table.auto_set_font_size(False)
    table.set_fontsize(12)
    table.scale(1, 2.0) # 行の高さを少し広げる

    # 各セルへのスタイル適用
    for (row, col), cell in table.get_celld().items():
        cell.set_text_props(fontfamily='serif')
        cell.set_linewidth(0) 

        # Condition列 (左揃え)
        if col == 0 and row > 0:
            cell.set_text_props(ha='left')
            # 少しインデントを入れる
            text_obj = cell.get_text()
            val = text_obj.get_text()
            text_obj.set_text("  " + val)

        # ヘッダー行 (太字)
        if row == 0:
            cell.set_text_props(weight='bold')

    # ---------------------------------------------------------
    # 罫線（割線）の描画 - 3行テーブル用に座標を調整
    # ---------------------------------------------------------
    # 座標は ax.transAxes (0.0~1.0) で指定します。
    # テーブルの行数や scale によって微調整が必要な場合があります。
    
    footer_ex = 0.15 # 横幅の余白調整

    # Y座標の目安 (scale=2.0, row=3+1 の場合)
    y_top = 0.75
    y_header_bottom = 0.60
    y_bottom = 0.15

    # 一番上の線 (Top Line)
    ax.plot([0+footer_ex, 1-footer_ex], [y_top+0.02, y_top+0.02], color='black', linewidth=2, transform=ax.transAxes)
    
    # ヘッダーの下の線 (Middle Line)
    ax.plot([0+footer_ex, 1-footer_ex], [y_header_bottom+0.05, y_header_bottom+0.05], color='black', linewidth=1, transform=ax.transAxes)
    
    # 一番下の線 (Bottom Line)
    ax.plot([0+footer_ex, 1-footer_ex], [y_bottom+0.07, y_bottom+0.07], color='black', linewidth=2, transform=ax.transAxes)

    # 保存
    plt.savefig(output_filename, dpi=300, bbox_inches='tight', pad_inches=0.1)
    print(f"Table image saved as: {output_filename}")
    plt.show()

# ==========================================
# メイン実行
# ==========================================
if __name__ == "__main__":
    df_diff_summary = generate_diff_summary_table()
    
    print("\n--- Difference Summary Table Data ---")
    print(df_diff_summary)
    
    save_diff_table_as_image(df_diff_summary, "table_improvement_summary.png")