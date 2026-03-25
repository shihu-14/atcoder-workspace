import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from io import StringIO

# データの読み込み
raw_data = """
2	A	1	4	1	5	1	0
2	C	3	2	0	3	2	0
2	B	3	4	0	4	2	0
1	A	5	5	4	7	5	1
1	B	5	5	5	8	7	1
1	C	5	5	5	5	5	1
3	B	7	9	8	9	7	2
3	A	7	10	6	6	5	2
3	C	3	8	4	8	8	2
4	B	9	7	8	1	8	0
4	C	7	7	6	2	7	0
4	A	7	6	4	3	6	0
5	C	4	6	3	7	7	3
5	A	5	6	5	9	6	3
5	B	6	6	6	8	6	5
6	C	5	9	8	4	7	5
6	B	7	8	6	8	6	5
6	A	8	6	3	1	3	2
7	A	6	9	4	7	7	4
7	B	5	6	3	9	5	3
7	C	7	7	5	7	7	5
8	A	6	8	2	9	6	3
8	C	3	7	3	7	6	2
8	B	7	8	2	8	7	6
9	B	2	7	1	10	0	0
9	A	1	7	5	7	2	2
9	C	2	7	1	9	2	1
10	B	5	7	3	9	6	3
10	C	5	6	2	9	5	3
10	A	7	5	8	8	7	5
11	C	3	6	2	6	7	0
11	A	3	5	7	3	9	3
11	B	2	4	1	6	2	0
12	C	9	9	9	0	9	2
12	B	9	9	7	0	8	2
12	A	8	8	7	0	7	2
"""

# データのパース
columns = ['ID', 'Condition', 'Mental Demand', 'Physical Demand', 'Temporal Demand', 'Performance', 'Effort', 'Frustration']
# タブまたはスペース区切りに対応
df = pd.read_csv(StringIO(raw_data), sep='\s+', names=columns)

# グラフ描画用に変形（Long形式）
# 'ID'と'Condition'以外（各項目のスコア）を縦持ちに変換
df_melted = df.melt(id_vars=['ID', 'Condition'], 
                    value_vars=['Mental Demand', 'Physical Demand', 'Temporal Demand', 'Performance', 'Effort', 'Frustration'], 
                    var_name='Item', value_name='Score')

# 表示順序の定義
item_order = ['Mental Demand', 'Physical Demand', 'Temporal Demand', 'Performance', 'Effort', 'Frustration']
hue_order = ['C', 'A', 'B']  # Baseline -> Task-based -> Adaptive
cond_labels = {'C': 'Baseline', 'A': 'Task-based', 'B': 'Adaptive'} # 凡例用ラベル

# グラフの初期化
plt.figure(figsize=(14, 8))
sns.set_style("whitegrid")

# 棒グラフの描画
ax = sns.barplot(
    data=df_melted, 
    x='Item', 
    y='Score', 
    hue='Condition', 
    order=item_order, 
    hue_order=hue_order, 
    palette='viridis',
    errorbar=None# エラーバーを表示せず平均値のみ表示
)
ax.tick_params(axis='x', labelsize=14)
# タイトルと軸ラベルの設定
plt.title('NASA-TLX Scores by Condition', fontsize=24, fontweight='bold', pad=35)
plt.ylabel('Mean Score', fontsize=26, labelpad=25)
plt.xlabel('NASA-TLX Items', fontsize=26, labelpad=25)
plt.ylim(0, 11) # データの範囲に合わせて調整

# 凡例の修正（C, A, B を Baseline, Task-based, Adaptive に変更）
handles, labels = ax.get_legend_handles_labels()
new_labels = [cond_labels[l] for l in labels]
plt.legend(handles, new_labels, title='Condition', loc='upper right', fontsize=20, title_fontsize=22)

# 数値ラベルの表示
for p in ax.patches:
    height = p.get_height()
    if height > 0:
        ax.annotate(f'{height:.2f}', 
                    (p.get_x() + p.get_width() / 2., height), 
                    ha='center', va='center', 
                    xytext=(0, 10), 
                    textcoords='offset points',
                    fontsize=18,
                    )

plt.tight_layout()
plt.show()

# 画像保存用（必要に応じてコメントアウトを外してください）
# plt.savefig('nasa_tlx_chart.png', dpi=300)