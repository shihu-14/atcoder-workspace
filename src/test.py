import matplotlib.pyplot as plt
import pandas as pd

# ---------------------------------------------------------
# 1. データの定義
# ---------------------------------------------------------
data = {
    "Scale": ["NASA-TLX", "", "", "SUS", "", ""],
    "Condition": ["T", "A", "B", "T", "A", "B"],
    "Mean": ["49.31", "51.11", "48.06", "72.92", "74.79", "77.08"],
    "SD": ["12.84", "15.98", "13.35", "14.30", "17.00", "15.03"], 
    "Median": ["51.67", "55.00", "49.17", "70.00", "76.25", "77.50"]
}

df = pd.DataFrame(data)

# ---------------------------------------------------------
# 2. スタイル設定
# ---------------------------------------------------------
plt.rcParams['font.family'] = 'serif'
plt.rcParams['font.serif'] = ['Times New Roman']
# ---------------------------------------------------------
# 3. 表の描画関数
# ---------------------------------------------------------
def render_table(data, font_size=12, header_color='#ffffff', row_colors=['#ffffff', '#ffffff'], edge_color='w',
                 bbox=[0, 0, 1, 1], ax=None, **kwargs):
    if ax is None:
        fig, ax = plt.subplots(figsize=(8, 4))
        ax.axis('off')

    mpl_table = ax.table(cellText=data.values, bbox=bbox, colLabels=data.columns, **kwargs)

    mpl_table.auto_set_font_size(False)
    mpl_table.set_fontsize(font_size)

    # 行数 (ヘッダー + データ行)
    rows = len(data) + 1
    # 各行の高さを「1 / 行数」にして、描画領域を隙間なく埋める
    row_height = 1.0 / rows

    # セルのスタイル設定
    for (row, col), cell in mpl_table.get_celld().items():
        cell.set_edgecolor(edge_color)
        cell.set_height(row_height) # 高さを自動調整

        # ヘッダー (row=0)
        if row == 0:
            cell.set_text_props(weight='bold', color='black', ha='left')
            cell.set_facecolor(header_color)
        # データ行 (row > 0)
        else:
            cell.set_facecolor(row_colors[row % len(row_colors)])
            cell.set_text_props(ha='left')

    return ax

# ---------------------------------------------------------
# 4. 描画実行と罫線の追加
# ---------------------------------------------------------
# ★変更点: figsizeを (10, 5) -> (7, 3) にしてコンパクト化
fig, ax = plt.subplots(figsize=(7, 3)) 
ax.axis('off')

# テーブル生成
render_table(df, ax=ax, cellLoc='left')

# --- 罫線 (黒線) の追加 ---
# 行数 (ヘッダー1 + データ6 = 7行)
rows = 7
line_width_thick = 1.5 # 少し細くしてバランス調整
line_width_thin = 0.8

# 1. 上端の線 (Top rule)
ax.plot([0, 1], [1, 1], transform=ax.transAxes, color='black', linewidth=line_width_thick, clip_on=False)

# 2. ヘッダー下の線 (Middle rule)
# 高さの計算: 1 - (1行分)
y_header_bottom = 1 - (1.0 / rows)
ax.plot([0, 1], [y_header_bottom, y_header_bottom], transform=ax.transAxes, color='black', linewidth=line_width_thin, clip_on=False)

# 3. 区切り線 (NASA-TLXとSUSの間)
# 3行目(A,B,C)が終わったところ: 1 - (ヘッダー1 + データ3)/7
y_separator = 1 - (4.0 / rows)
ax.plot([0, 1], [y_separator, y_separator], transform=ax.transAxes, color='black', linewidth=0.5, clip_on=False)

# 4. 下端の線 (Bottom rule)
ax.plot([0, 1], [0, 0], transform=ax.transAxes, color='black', linewidth=line_width_thick, clip_on=False)

# 保存 (余白を極限まで削る)
plt.savefig('final_compact_table.png', dpi=300, bbox_inches='tight', pad_inches=0.05)
plt.show()