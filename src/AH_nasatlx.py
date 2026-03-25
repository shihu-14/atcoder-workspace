import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from io import StringIO

# データの定義
data = """Condition,Mental Demand,Physical Demand,Temporal Demand,Performance,Effort,Frustration,Total
A,5.33,6.58,4.67,5.42,5.33,2.25,4.93
B,5.58,6.67,4.17,6.67,5.33,2.25,5.11
C,4.67,6.58,4.00,5.58,6.00,2.00,4.81"""

# データフレームの読み込み
df = pd.read_csv(StringIO(data))

# グラフ描画用にデータを変形（Long形式：項目とスコアの列にまとめる）
df_melted = df.melt(id_vars='Condition', var_name='Item', value_name='Score')

# グラフのX軸における項目の並び順を指定（Totalを一番右に）
item_order = ['Mental Demand', 'Physical Demand', 'Temporal Demand', 'Performance', 'Effort', 'Frustration', 'Total']

# グラフの初期化
plt.figure(figsize=(14, 8))
sns.set_style("whitegrid")

# 棒グラフの描画
# x='Item': 項目を横軸に
# y='Score': 数値を縦軸に
# hue='Condition': 条件ごとに色分け
# hue_order=['A', 'B', 'C']: 凡例の順序を固定
# palette='viridis': 配色（必要に応じて 'Set2', 'deep' などに変更可能）
ax = sns.barplot(
    data=df_melted, 
    x='Item', 
    y='Score', 
    hue='Condition', 
    order=item_order, 
    hue_order=['A', 'B', 'C'], 
    palette='viridis'
)

# タイトルと軸ラベルの設定
plt.title('NASA-TLX Scores by Condition', fontsize=18)
plt.ylabel('Mean Score', fontsize=14)
plt.xlabel('NASA-TLX Items', fontsize=14)
plt.ylim(0, 10.5) # 縦軸の範囲（データの最大値に合わせて調整してください）
plt.legend(title='Condition', loc='upper right')

# 各バーの上に数値を表示する処理
for p in ax.patches:
    height = p.get_height()
    if height > 0: # 高さがある場合のみラベルを表示
        ax.annotate(f'{height:.2f}', 
                    (p.get_x() + p.get_width() / 2., height), 
                    ha='center', va='center', 
                    xytext=(0, 9), # バーの上端から少し上に表示
                    textcoords='offset points',
                    fontsize=10)

# レイアウトを整えて表示
plt.tight_layout()
plt.show()

# 画像として保存したい場合は以下のコメントアウトを外してください
# plt.savefig('nasa_tlx_chart.png', dpi=300)