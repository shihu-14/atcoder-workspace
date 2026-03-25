import matplotlib.pyplot as plt
import numpy as np

def draw_preference_vertical_style():
    # --- 1. フォント設定 (論文調) ---
    plt.rcParams['font.family'] = 'serif'
    plt.rcParams['font.serif'] = ['Times New Roman']
    plt.rcParams['mathtext.fontset'] = 'stix'
    plt.rcParams['font.size'] = 12

    # --- 2. データ定義 ---
    # 条件ラベル
    conditions = ['Baseline', 'Task-based', 'Adaptive']
    
    # 各順位に選んだ人数 (合計12人)
    # データの並び順は [Baselineの人数, Task-basedの人数, Adaptiveの人数]
    
    # 1位に選んだ人数 (C=1, A=5, B=6)
    rank_1st = np.array([1, 5, 6])
    # 2位に選んだ人数 (C=3, A=4, B=5)
    rank_2nd = np.array([3, 4, 5])
    # 3位に選んだ人数 (C=8, A=3, B=1)
    rank_3rd = np.array([8, 3, 1])

    # --- 3. グラフ描画設定 ---
    # 色設定: 1st(緑) -> 2nd(黄) -> 3rd(オレンジ/赤系)
    # NASA-TLXや前のグラフとの統一感を意識した色
    colors = ['#ffffbf', '#d9ef8b', '#1a9850'] 
    labels = ['1st ', '2nd ', '3rd ']
    
    # 図の作成
    fig, ax = plt.subplots(figsize=(8, 6)) # 縦横比を調整

    # 棒の幅
    bar_width = 0.6
    x_pos = np.arange(len(conditions))

    # 積み上げ棒グラフの描画 (下から 1st -> 2nd -> 3rd)
    # 1st (Green)
    p1 = ax.bar(x_pos, rank_1st, width=bar_width, color=colors[0], 
                edgecolor='black', linewidth=0.8, label=labels[0])
    
    # 2nd (Yellow) - 1stの上に積む
    p2 = ax.bar(x_pos, rank_2nd, width=bar_width, bottom=rank_1st, 
                color=colors[1], edgecolor='black', linewidth=0.8, label=labels[1])
    
    # 3rd (Red) - 1st + 2nd の上に積む
    p3 = ax.bar(x_pos, rank_3rd, width=bar_width, bottom=rank_1st + rank_2nd, 
                color=colors[2], edgecolor='black', linewidth=0.8, label=labels[2])

    # --- 4. 軸・ラベル設定 ---
    ax.set_ylabel('Number of Participants', fontsize=14, labelpad=20)
    ax.set_title('Preference Ranking', fontsize=16, fontweight='bold', pad=15)
    
    ax.set_xticks(x_pos)
    ax.set_xticklabels(conditions, fontsize=14, fontweight='bold') 
    ax.tick_params(axis='x', pad=10)
    ax.set_yticks(np.arange(0, 13, 2)) # 0から12まで2刻み
    
    # Y軸の範囲 (0〜12人)
    ax.set_ylim(0, 12.5)

    # グリッド (Y軸のみ)
    ax.yaxis.grid(True, linestyle=':', color='gray', alpha=0.7)
    ax.set_axisbelow(True)

    # スタイル調整 (上と右の枠線を消す)
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.spines['left'].set_linewidth(1.0)
    ax.spines['bottom'].set_linewidth(1.0)

    # --- 5. 凡例と数値ラベル ---
    # 凡例をグラフの外側（上または右）に配置
    ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.1),
              ncol=3, frameon=False, fontsize=16)

    # 各バーの中に数値を表示
    for i, (r1, r2, r3) in enumerate(zip(rank_1st, rank_2nd, rank_3rd)):
        # 1stの数値
        if r1 > 0:
            ax.text(i, r1/2, str(r1), ha='center', va='center', fontweight='bold', fontsize=14)
        # 2ndの数値
        if r2 > 0:
            ax.text(i, r1 + r2/2, str(r2), ha='center', va='center', fontweight='bold', fontsize=14)
        # 3rdの数値
        if r3 > 0:
            ax.text(i, r1 + r2 + r3/2, str(r3), ha='center', va='center', fontweight='bold', fontsize=14)

    plt.tight_layout()
    plt.savefig('preference_ranking_vertical.png', dpi=300)
    plt.show()

# 実行
draw_preference_vertical_style()