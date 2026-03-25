import pandas as pd
import os
import matplotlib.pyplot as plt

# ==========================================
# 設定箇所
# ==========================================
# CSVファイルがあるディレクトリ (ご自身の環境に合わせてください)
TARGET_DIR = os.path.expanduser("/Users/eiichi/Desktop/AHdata/passrate")

COLUMN_MAPPING = {
    "Short Turn": "shortRate",
    "Long Turn":  "longRate",
    "Tight Turn": "tightRate",
    "Total": "passRate"
}

# 列の幅設定 (元の値を維持しつつ、2列目だけ文字数増加に合わせて微調整)
# "pre-test"が入るため 0.08 -> 0.12 程度確保しないと文字が重なる可能性がありますが、
# 厳密に元のままが良い場合は [0.05, 0.08, ...] に戻してください。
col_widths = [0.05, 0.12, 0.17, 0.17, 0.17, 0.17]

# ==========================================
# データ集計処理
# ==========================================
def generate_summary_table():
    conditions = ['A', 'B', 'C']
    times = ['before', 'after']
    
    # ★追加: 表示名のマッピング定義
    cond_map = {'A': 'T', 'B': 'A', 'C': 'B'}
    time_map = {'before': 'pre-test', 'after': 'post-test'}
    
    rows = []

    for condition in conditions:
        for time in times:
            filename = f"passrate_{condition}_{time}.csv"
            filepath = os.path.join(TARGET_DIR, filename)
            
            # ★変更: マッピングを適用して表示名を設定
            row_data = {
                "Condition": cond_map[condition], # A->T, B->A, C->B
                "": time_map[time]                # before->pre-test
            }
            
            if not os.path.exists(filepath):
                for display_name in COLUMN_MAPPING.keys():
                    row_data[display_name] = "-"
                rows.append(row_data)
                continue
            
            try:
                df = pd.read_csv(filepath)
                for display_name, csv_col_name in COLUMN_MAPPING.items():
                    if csv_col_name in df.columns:
                        vals = df[csv_col_name]
                        mean_val = vals.mean()
                        sd_val = vals.std()
                        
                        if pd.isna(sd_val):
                            formatted_val = f"{mean_val:.2f} (-)"
                        else:
                            formatted_val = f"{mean_val:.2f} ({sd_val:.2f})"
                        
                        row_data[display_name] = formatted_val
                    else:
                        row_data[display_name] = "N/A"
                rows.append(row_data)

            except Exception as e:
                print(f"Error processing {filename}: {e}")

    result_df = pd.DataFrame(rows)
    return result_df

# ==========================================
# 論文用テーブル画像生成関数
# ==========================================
def save_table_as_image(df, output_filename="table_summary.png"):
    if df.empty:
        print("No data to plot.")
        return

    # 1. 論文調のフォント設定
    plt.rcParams["font.family"] = "serif"
    plt.rcParams["font.serif"] = ["Times New Roman"]
    plt.rcParams["font.size"] = 12

    # 2. Conditionの重複を空文字にする
    plot_df = df.copy()
    plot_df['Condition'] = plot_df['Condition'].mask(plot_df['Condition'].duplicated(), "")

    # 3. 図の描画準備
    w, h = 10, len(plot_df) * 0.6 + 1.2
    fig, ax = plt.subplots(figsize=(w, h))
    
    # 軸を消す
    ax.axis('off')

    # 4. テーブルの作成
    table = ax.table(
        cellText=plot_df.values,
        colLabels=plot_df.columns,
        loc='center',
        cellLoc='center',
        colWidths=col_widths,
        edges='open'
    )

    # 5. スタイルの詳細設定
    table.auto_set_font_size(False)
    table.set_fontsize(12)
    table.scale(1, 1.8)

    # 各セルへのアクセス
    for (row, col), cell in table.get_celld().items():
        cell.set_text_props(fontfamily='serif')
        cell.set_linewidth(0) 

        # Condition列の設定
        if col == 0:
            if row > 0:
                cell.set_text_props(ha='left')
                text_obj = cell.get_text()
                val = text_obj.get_text()
                if val:
                    text_obj.set_text("     " + val)
            
            if row == 0:
                 cell.set_text_props(ha='left')
                 text_obj = cell.get_text()
                 text_obj.set_text("       Condition")

        # Time列 (2列目) の設定
        if col == 1:
            cell.set_text_props(ha='center')

        # ヘッダー行の設定
        if row == 0:
            cell.set_text_props(weight='bold')

    # ---------------------------------------------------------
    # 罫線（割線）の描画と調整 - 指定されたパラメータをそのまま使用
    # ---------------------------------------------------------
    footer_ex = 0.1

    # 条件ごとの区切り線 (Separators)
    # AとBの間
    ax.plot([0+footer_ex, 1-footer_ex], [0.54, 0.54], color='gray', linewidth=0.5, linestyle='-', transform=ax.transAxes)

    # BとCの間
    ax.plot([0+footer_ex, 1-footer_ex], [0.38, 0.38], color='gray', linewidth=0.5, linestyle='-', transform=ax.transAxes)

    # 一番上の線 (Top Line)
    ax.plot([0+footer_ex-0.00, 1-footer_ex+0.00],[0.80, 0.80], color='black', linewidth=2, transform=ax.transAxes)
    
    # ヘッダーの下の線 (Middle Line)
    ax.plot([0+footer_ex, 1-footer_ex], [0.71, 0.71], color='black', linewidth=1, transform=ax.transAxes)
    
    # 一番下の線 (Bottom Line)
    ax.plot([0+footer_ex-0.00, 1-footer_ex+0.00], [0.21, 0.21], color='black', linewidth=2, transform=ax.transAxes)

    # 保存
    plt.savefig(output_filename, dpi=300, bbox_inches='tight', pad_inches=0.1)
    print(f"Table image saved as: {output_filename}")
    plt.show()

# ==========================================
# メイン実行
# ==========================================
if __name__ == "__main__":
    df_summary = generate_summary_table()
    
    print("\n--- Summary Table Data ---")
    print(df_summary)
    
    save_table_as_image(df_summary, "table_summary.png")