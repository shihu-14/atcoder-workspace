import matplotlib.pyplot as plt
import numpy as np

# 1. 出力ファイル名の設定
output_filename = 'nasa_tlx_uist2026.png'

# 2. NASA-TLX項目 (UIST向けにすべて英語ラベル化)
nasa_items_en = ['Mental Demand', 'Physical Demand', 'Temporal Demand', 'Performance', 'Effort', 'Frustration']

# 3. 事前算出された統計値の組み込み (平均値と標準誤差)
# CSVデータから各条件における平均値を算出
means = {
    'A': [61.250, 72.500, 49.167, 83.750, 68.750, 27.500],
    'B': [57.500, 73.750, 50.000, 82.917, 75.417, 39.167],
    'C': [65.833, 76.250, 55.417, 82.500, 69.583, 37.917]
}

# CSVデータから各条件における標準誤差(SEM)を算出
sems = {
    'A': [4.692, 6.046, 5.533, 5.678, 6.158, 4.827],
    'B': [6.614, 6.370, 7.687, 4.150, 5.382, 7.559],
    'C': [4.345, 5.405, 7.057, 4.544, 7.216, 6.894]
}

# 4. グラフの描画設定
fig, ax = plt.subplots(figsize=(10, 6))
x = np.arange(len(nasa_items_en))
width = 0.25
conditions = ['A', 'B', 'C']
colors = ['#1f77b4', '#ff7f0e', '#2ca02c']

# 5. エラーバー付きの棒グラフの描画
for i, cond in enumerate(conditions):
    ax.bar(x + (i - 1) * width, means[cond], width, 
           label=f'Cond {cond}', 
           yerr=sems[cond], capsize=5, color=colors[i], edgecolor='black')

# 6. 軸ラベル・タイトル・凡例の設定 (英語統一・Y軸0-100固定)
ax.set_ylabel('Score (0-100)')
ax.set_title('NASA-TLX Scores by Condition')
ax.set_xticks(x)
ax.set_xticklabels(nasa_items_en, rotation=15)
ax.set_ylim(0, 100)
ax.legend()

# 7. レイアウト調整と保存
plt.tight_layout()
plt.savefig(output_filename, dpi=300)
print(f'Saved plot as {output_filename}')
plt.show()