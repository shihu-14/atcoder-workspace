import matplotlib.pyplot as plt
import numpy as np

def draw_likert_grid_paper_style_v3():
    # --- 1. フォント設定 (論文調: Times New Roman) ---
    plt.rcParams['font.family'] = 'serif'
    plt.rcParams['font.serif'] = ['Times New Roman']
    plt.rcParams['mathtext.fontset'] = 'stix'
    plt.rcParams['font.size'] = 12

    # --- 2. データ定義 ---
    # 適切な英語に翻訳 (改行を入れて見やすく調整)
    questions_en = [
        "Q1. I understood where the\nexpert was looking.",
        "Q2. I could predict the course and\nturn with composure at the widest turn.",
        "Q3. I was able to ski smoothly\nand stably throughout the run.",
        "Q4. I felt forced to look at\nspecific locations by the system.",
        "Q5. I felt the system is useful\nfor alpine skiing training."
    ]

    conditions = ['B', 'T', 'A']  

    # データ: [Q1, Q2, Q3, Q4, Q5]
    data = [
        # Q1
        [
            [4, 3, 1, 1, 2, 0, 1],  # C (Baseline)
            [0, 0, 1, 1, 2, 7, 1], # A (Task)
            [0, 0, 0, 2, 4, 4, 2] # B (Adaptive)
        ],
        # Q2
        [
            [0, 1, 2, 1, 4, 1, 3], 
            [0, 0, 0, 1, 3, 7, 1],
            [0, 0, 1, 0, 5, 5, 1]
        ],
        # Q3
        [
            [0, 1, 0, 1, 6, 1, 3],
            [0, 0, 1, 2, 2, 6, 1],
            [0, 1, 0, 2, 4, 4, 1]
        ],
        # Q4
        [
            [9, 2, 0, 0, 1, 0, 0], 
            [0, 1, 0, 1, 3, 5, 2],
            [1, 0, 2, 1, 2, 3, 3]
        ],
        # Q5
        [
            [0, 0, 1, 2, 6, 3, 0],
            [0, 0, 0, 0, 4, 5, 3],
            [0, 0, 0, 1, 2, 6, 3]
        ]
    ]

    # --- 3. グラフ描画設定 ---
    colors = ['#d73027', '#fc8d59', '#fee08b', '#ffffbf', '#d9ef8b', '#91cf60', '#1a9850']
    
    # 図の作成: 3行2列
    fig, axes = plt.subplots(nrows=3, ncols=2, figsize=(10, 12))
    axes_flat = axes.flatten()
    
    y_pos = np.arange(len(conditions))

    # ループでQ1~Q5をプロット
    for q_idx, ax in enumerate(axes_flat):
        # 6番目の枠(index 5)は非表示にしてスキップ
        if q_idx >= len(data):
            ax.axis('off')
            continue

        q_data = np.array(data[q_idx])
        lefts = np.zeros(len(conditions))

        # 積み上げ棒グラフ
        for i in range(7):
            score_counts = q_data[:, i]
            # Aを上にするため逆順プロット
            ax.barh(y_pos, score_counts[::-1], left=lefts[::-1], height=0.6, 
                    color=colors[i], edgecolor='black', linewidth=0.5)
            lefts += score_counts

        # 軸設定
        ax.set_yticks(y_pos)
        ax.set_yticklabels(conditions[::-1], fontweight='bold')
        
        # タイトルの設定
        ax.set_title(questions_en[q_idx], fontsize=12, fontweight='bold', pad=10)
        
        # スタイル調整
        ax.spines['top'].set_visible(False)
        ax.spines['right'].set_visible(False)
        ax.spines['left'].set_visible(False)
        ax.spines['bottom'].set_color('black')
        
        ax.set_xticks(np.arange(0, 13, 2))
        ax.set_xlabel('') 

        # グリッド
        ax.xaxis.grid(True, linestyle=':', color='gray', alpha=0.5)
        ax.set_axisbelow(True)

    
    # --- 5. 全体レイアウト調整 ---
    plt.subplots_adjust(bottom=0.2, wspace=0.3, hspace=0.5, top=0.95)

    # 共通ラベル
    fig.text(0.02, 0.55, 'Condition', va='center', rotation='vertical', fontsize=18, fontweight='bold')
    fig.text(0.5, 0.072, 'Number of Participants', ha='center', fontsize=18, fontweight='bold')

    # リッカード尺度の凡例 (最下部)
    dummy_lines = [plt.Rectangle((0,0),1,1, fc=c, ec='black', linewidth=0.5) for c in colors]
    likert_labels = ['1', '2', '3', '4', '5', '6', '7']

    # 中央揃えで配置
    fig.legend(dummy_lines, likert_labels, 
               loc='lower center', 
               bbox_to_anchor=(0.5, 0.1), 
               ncol=7, 
               frameon=False, 
               fontsize=14, 
               handlelength=1.5, 
               handletextpad=0.5, 
               columnspacing=1.0)
    
    # Strongly Disagree / Agree のテキスト配置
    fig.text(0.25, 0.12, 'Strongly Disagree', 
             ha='right', va='center', fontsize=14, fontweight='bold')
    
    fig.text(0.75, 0.12, 'Strongly Agree', 
             ha='left', va='center', fontsize=14, fontweight='bold')
    
    plt.savefig('likert_plot_3x2_en.png', dpi=300)
    # plt.show()

# 実行
draw_likert_grid_paper_style_v3()