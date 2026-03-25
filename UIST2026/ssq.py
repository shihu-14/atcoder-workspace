import matplotlib.pyplot as plt
import numpy as np

# 1. 出力ファイル名の設定
output_filename = 'ssq_uist2026.png'

# 2. SSQの3指標および総合スコアの項目 (UIST向けにすべて英語ラベル化)
ssq_items_en = ['Nausea', 'Oculomotor', 'Disorientation', 'Total Score']

# 3. 事前算出された統計値の組み込み (Kennedy et al., 1993に基づく平均値と標準誤差)
# CSVデータから各条件における平均値を算出
means = {
    'A': [3.975, 12.002, 11.600, 10.597],
    'B': [5.565, 9.096, 6.327, 7.480],
    'C': [12.720, 17.687, 20.880, 17.869]
}

# CSVデータから各条件における標準誤差(SEM)を算出
sems = {
    'A': [2.184, 4.009, 5.640, 3.902],
    'B': [3.415, 3.713, 3.921, 3.656],
    'C': [5.549, 5.507, 9.104, 6.963]
}

# 4. グラフの描画設定
fig, ax = plt.subplots(figsize=(10, 6))
x = np.arange(len(ssq_items_en))
width = 0.25
conditions = ['A', 'B', 'C']
colors = ['#1f77b4', '#ff7f0e', '#2ca02c']

# 5. エラーバー付きの棒グラフの描画
for i, cond in enumerate(conditions):
    ax.bar(x + (i - 1) * width, means[cond], width, 
           label=f'Cond {cond}', 
           yerr=sems[cond], capsize=5, color=colors[i], edgecolor='black')

# 6. 軸ラベル・タイトル・凡例の設定 (英語統一)
ax.set_ylabel('Score')
ax.set_title('Simulator Sickness Questionnaire (SSQ) Scores by Condition')
ax.set_xticks(x)
ax.set_xticklabels(ssq_items_en, rotation=0)

# エラーバーの長さを考慮してY軸の最大値を35に固定(データのばらつきを収めるため)
ax.set_ylim(0, 35)
ax.legend()

# 7. レイアウト調整と保存
plt.tight_layout()
plt.savefig(output_filename, dpi=300)
print(f'Saved plot as {output_filename}')
plt.show()