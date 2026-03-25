import matplotlib.pyplot as plt
import numpy as np

def draw_likert_grid_paper_style():
    # --- 1. フォント設定 (論文調: Times New Roman) ---
    plt.rcParams['font.family'] = 'serif'
    plt.rcParams['font.serif'] = ['Times New Roman']
    plt.rcParams['mathtext.fontset'] = 'stix' # 数式なども含めてTimes調に
    plt.rcParams['font.size'] = 12

    # --- 2. データ定義 ---
    questions_en = [
        "Q1. 上級者の視線の向く先が分かるようになった",
        "Q2. 左右への振り幅が一番大きかったターンにおいて、コースを予測し余裕を持ったターンを行えたと感じた.",
        "Q3. 全体を通して、安定してスムーズに滑走することができた.",
        "Q4. システムから特定の場所を見させるような強制力を感じた.",
        "Q5. アルペンスキーのトレーニングに役立つと感じた."
    ]

    conditions = ['B', 'T', 'A']  

    # データ: [Q1データ, Q2データ...], 各Qデータは [Cond A, Cond B, Cond C] の分布(1-7の人数)
    data = [
        # Q1
        [
            [4, 3, 1, 1, 2, 0, 1],  # C
            [0, 0, 1, 1, 2, 7, 1], # A
            [0, 0, 0, 2, 4, 4, 2] # B
        ],
        # Q2
        [
            [0, 1, 2, 1, 4, 1, 3],  # C
            [0, 0, 0, 1, 3, 7, 1], # A
            [0, 0, 1, 0, 5, 5, 1] # B
        ],
        # Q3
        [
            [0, 1, 0, 1, 6, 1, 3],  # C
            [0, 0, 1, 2, 2, 6, 1], # A
            [0, 1, 0, 2, 4, 4, 1] # B
        ],
        # Q4
        [
            [9, 2, 0, 0, 1, 0, 0],  # C
            [0, 1, 0, 1, 3, 5, 2], # A
            [1, 0, 2, 1, 2, 3, 3] # B
        ],
        # Q5
        [
            [0, 0, 1, 2, 6, 3, 0],  # C
            [0, 0, 0, 0, 4, 5, 3], # A
            [0, 0, 0, 1, 2, 6, 3] # B
        ]
    ]

    # --- 3. グラフ描画設定 ---
    colors = ['#d73027', '#fc8d59', '#fee08b', '#ffffbf', '#d9ef8b', '#91cf60', '#1a9850']
    
    # 図の作成
    # 少し下のスペース(bottom)を空けておく
    fig, axes = plt.subplots(nrows=2, ncols=3, figsize=(12, 7))
    axes_flat = axes.flatten()
    
    y_pos = np.arange(len(conditions))

    # ループでプロット
    for q_idx, ax in enumerate(axes_flat):
        # データがない6番目の枠は非表示
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
                    color=colors[i], edgecolor='black', linewidth=0.5) # 論文用に枠線を黒く細く
            lefts += score_counts

        # 軸設定
        ax.set_yticks(y_pos)
        ax.set_yticklabels(conditions[::-1], fontweight='bold')
        
        # タイトルの折り返し処理
        title_text = questions_en[q_idx]
        if len(title_text) > 45:
             words = title_text.split()
             mid = len(words) // 2
             title_text = " ".join(words[:mid]) + "\n" + " ".join(words[mid:])
        ax.set_title(title_text, fontsize=11, fontweight='bold', pad=8)
        
        # スタイル
        ax.spines['top'].set_visible(False)
        ax.spines['right'].set_visible(False)
        ax.spines['left'].set_visible(False)
        ax.spines['bottom'].set_color('black')
        
        ax.set_xticks(np.arange(0, 13, 2))
        # 個別のラベルは書かない
        ax.set_xlabel('') 

        # グリッド
        ax.xaxis.grid(True, linestyle=':', color='gray', alpha=0.5)
        ax.set_axisbelow(True)

    # --- 4. 全体ラベルと凡例の配置 ---

#--------------------------------------------------------------------------------
    # --- 【追加部分】 Preference Ranking (右下のサブプロット) ---
    # 6番目のサブプロットを取得し、再表示する
    ax_pref = axes_flat[5]
    ax_pref.axis('on') 

    # データ定義 (C, B, A の順) 
    # ※グラフのY軸は下から 0, 1, 2 となるため、C, B, A の順にデータを並べます
    # 3rd(不人気) -> 2nd -> 1st(人気) の順に積み上げます
    p_data_3rd = np.array([8, 1, 3]) # C=8, B=1, A=3
    p_data_2nd = np.array([3, 5, 4]) # C=3, B=5, A=4
    p_data_1st = np.array([1, 6, 5]) # C=1, B=6, A=5

    pref_datasets = [p_data_3rd, p_data_2nd, p_data_1st]
    pref_colors = ['#ffffbf', '#d9ef8b', '#1a9850'] # Red, Yellow, Green
    pref_labels = ['3rd', '2nd', '1st']

    p_lefts = np.zeros(3)

    # 積み上げ棒グラフの描画
    for d, c, l in zip(pref_datasets, pref_colors, pref_labels):
        ax_pref.barh(y_pos, d, left=p_lefts, height=0.6, 
                     color=c, edgecolor='black', linewidth=0.5, label=l)
        p_lefts += d

    # 軸とタイトルの設定
    ax_pref.set_yticks(y_pos)
    ax_pref.set_yticklabels(conditions[::-1], fontweight='bold')
    ax_pref.set_title("Preference Ranking", fontsize=11, fontweight='bold', pad=8)
    
    ax_pref.spines['top'].set_visible(False)
    ax_pref.spines['right'].set_visible(False)
    ax_pref.spines['left'].set_visible(False)
    ax_pref.spines['bottom'].set_color('black')
    
    ax_pref.set_xticks(np.arange(0, 13, 2))
    ax_pref.set_xlabel('') # 個別のラベルは省略
    ax_pref.xaxis.grid(True, linestyle=':', color='gray', alpha=0.5)
    ax_pref.set_axisbelow(True)

    # Preference専用の凡例 (グラフのすぐ下に配置)
    dummy_pref = [plt.Rectangle((0,0),1,1, fc=c, ec='black', linewidth=0.5) for c in pref_colors]
    ax_pref.legend(dummy_pref, pref_labels, 
                   loc='upper center', 
                   bbox_to_anchor=(0.5, -0.18), # サブプロットの下部マージン領域に配置
                   ncol=3, frameon=False, fontsize=10, 
                   handlelength=1.5, handletextpad=0.5, columnspacing=1.0)
    
    #--------------------------------------------------------------------------------
    
    # 全体のレイアウト調整 (下のスペースを確保)
    # top, bottom, left, right, hspace, wspace
    plt.subplots_adjust(bottom=0.2, wspace=0.3, hspace=0.5)

    # 共通のY軸ラベル (左側)
    fig.text(0.08, 0.55, 'Condition', va='center', rotation='vertical', fontsize=14, fontweight='bold')

    # 共通のX軸ラベル (一番下)
    fig.text(0.5, 0.06, 'Number of Participants', ha='center', fontsize=14)

    # 凡例 (X軸ラベルの上、グラフの下)
    # ダミーのプロットを作って凡例用のハンドルを取得
    dummy_lines = []
    for i, color in enumerate(colors):
        dummy_lines.append(plt.Rectangle((0,0),1,1, fc=color, ec='black', linewidth=0.5))
    
    # ラベルをシンプルにする（両端だけ文字を入れて、真ん中は数字だけ、など）
    # 画像のようにしたい場合、テキストを短くするか、配置の間隔を調整します
    likert_labels = ['1', '2', '3', '4', '5', '6', '7']

    # 凡例の描画設定
    fig.legend(dummy_lines, likert_labels, 
               loc='lower center', 
               bbox_to_anchor=(0.38, 0.1),  # 【位置】 2つ目の数字を小さくすると下へ、大きくすると上へ (0.02 ~ 0.1くらいで調整)
               ncol=7,                      # 横一列に並べる数
               frameon=False,               # 枠線を消す
               fontsize=10,                 # 文字の大きさ
               handlelength=1.5,            # 【色の幅】 ここを「1.5」→「3.0」などに増やすと、色のバーが横長になります
               handletextpad=0.5,           # 【文字と色の間隔】
               columnspacing=1.0,
               )           # 【項目同士の間隔】 ここを調整して全体の幅を整えます
    
    fig.text(0.235, 0.128, 'Strongly Disagree', 
             ha='right', va='center', fontsize=10, fontweight='bold')
    
    fig.text(0.523, 0.128, 'Strongly Agree', 
             ha='left', va='center', fontsize=10, fontweight='bold')
    
    # fig.legend(dummy_lines, likert_labels, 
    #            loc='lower center', 
    #            bbox_to_anchor=(0.3, 0.08), # 位置調整: (横中央, 下からの高さ)
    #            ncol=7, frameon=False, fontsize=10,
    #            handlelength=1.5, handleheight=1.0)

    plt.show()

# 実行
draw_likert_grid_paper_style()