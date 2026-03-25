import matplotlib.pyplot as plt
import numpy as np

def draw_likert_grid_uist2026():
    # --- 1. フォント設定 (論文調) ---
    plt.rcParams['font.family'] = 'serif'
    # ※ 環境にTimes New Romanがない場合の警告を防ぐため標準のserifを使用します
    plt.rcParams['mathtext.fontset'] = 'stix'
    plt.rcParams['font.size'] = 12

    # --- 2. 設問の英語翻訳 (元のニュアンスを保持したHCI論文向けの表現) ---
    questions_en = [
        "Q1. I understood where I should\ndirect my gaze in slalom skiing.",
        "Q2. I felt forced to look at\nspecific locations by the system.",
        "Q3. I was able to maintain a\nsense of agency while skiing.",
        "Q4. I felt the training system\ninterfered with my skiing.",
        "Q5. I felt the system provided\na natural training experience.",
        "Q6. I knew where to look even\nwithout the training system.",
        "Q7. I noticed my vision was different\nthan usual during the training.",
        "Q8. Overall, I would like to incorporate\nthis system into my regular training."
    ]

    # 上からA, B, Cの順に並べるため、Y軸の下(0)からC, B, Aとして配置します
    conditions = ['C', 'B', 'A']  

    # --- 3. CSVから集計済みのデータ (1〜7の回答数) ---
    # 構造: [Q1, Q2, ... Q8]
    # 各Qの中身: [Condition Cの分布, Condition Bの分布, Condition Aの分布]
    data = [
        # Q1: どこに目線を向けるべきかを理解できた
        [[0, 0, 0, 1, 3, 4, 4], [2, 2, 0, 1, 2, 3, 2], [0, 0, 0, 0, 2, 4, 6]],
        # Q2: 見る場所を強制されている感じがした
        [[2, 1, 0, 0, 3, 4, 2], [5, 2, 2, 0, 0, 3, 0], [1, 0, 1, 2, 2, 3, 3]],
        # Q3: 自分が主体的に滑っている感覚を保てた
        [[0, 1, 0, 3, 2, 4, 2], [0, 0, 0, 0, 2, 5, 5], [0, 0, 1, 1, 6, 1, 3]],
        # Q4: 練習システムは滑走の邪魔に感じた
        [[1, 1, 0, 2, 6, 1, 1], [5, 1, 0, 1, 5, 0, 0], [4, 5, 2, 0, 1, 0, 0]],
        # Q5: スキーの練習として自然に感じた
        [[0, 0, 3, 3, 3, 2, 1], [0, 1, 2, 1, 2, 3, 3], [0, 0, 1, 2, 5, 3, 1]],
        # Q6: 練習システムがなくても自分で見るべき場所が分かった
        [[0, 0, 0, 3, 3, 5, 1], [2, 1, 2, 2, 3, 1, 1], [0, 0, 1, 4, 4, 2, 1]],
        # Q7: 練習時にいつもと違う見え方であることに気づいた
        [[0, 0, 1, 0, 0, 1, 10], [3, 1, 0, 1, 2, 1, 4], [0, 0, 0, 0, 2, 1, 9]],
        # Q8: 全体的にこのシステムを普段の練習に取り入れたいと思った
        [[0, 0, 3, 1, 2, 4, 2], [0, 2, 2, 3, 1, 2, 2], [0, 0, 0, 0, 4, 5, 3]]
    ]

    # --- 4. グラフ描画設定 ---
    # 赤(Disagree) から 緑(Agree) へのダイバージングカラー
    colors = ['#d73027', '#fc8d59', '#fee08b', '#ffffbf', '#d9ef8b', '#91cf60', '#1a9850']
    
    # 図の作成: 4行2列で8問分を描画
    fig, axes = plt.subplots(nrows=4, ncols=2, figsize=(11, 15))
    axes_flat = axes.flatten()
    
    y_pos = np.arange(len(conditions))

    # 各設問(Q1〜Q8)をプロット
    for q_idx, ax in enumerate(axes_flat):
        q_data = np.array(data[q_idx])
        lefts = np.zeros(len(conditions))

        # スコア1〜7の積み上げ処理
        for i in range(7):
            score_counts = q_data[:, i]
            # y_pos [0, 1, 2] に対して [C, B, A] のデータをプロット
            ax.barh(y_pos, score_counts, left=lefts, height=0.6, 
                    color=colors[i], edgecolor='black', linewidth=0.5)
            lefts += score_counts

        # 軸とラベルの設定
        ax.set_yticks(y_pos)
        ax.set_yticklabels(conditions, fontweight='bold')
        ax.set_title(questions_en[q_idx], fontsize=12, fontweight='bold', pad=10)
        
        # 不要な枠線の削除
        ax.spines['top'].set_visible(False)
        ax.spines['right'].set_visible(False)
        ax.spines['left'].set_visible(False)
        ax.spines['bottom'].set_color('black')
        
        # 被験者数(最大12名)に合わせたX軸メモリ
        ax.set_xticks(np.arange(0, 13, 2))
        ax.set_xlabel('') 
        ax.xaxis.grid(True, linestyle=':', color='gray', alpha=0.5)
        ax.set_axisbelow(True)

    # --- 5. 全体レイアウトの微調整 ---
    plt.subplots_adjust(bottom=0.12, wspace=0.3, hspace=0.6, top=0.95)

    # 共通の軸ラベル
    fig.text(0.04, 0.55, 'Condition', va='center', rotation='vertical', fontsize=18, fontweight='bold')
    fig.text(0.5, 0.08, 'Number of Participants', ha='center', fontsize=18, fontweight='bold')

    # リッカート尺度の凡例 (最下部)
    dummy_lines = [plt.Rectangle((0,0),1,1, fc=c, ec='black', linewidth=0.5) for c in colors]
    likert_labels = ['1', '2', '3', '4', '5', '6', '7']

    fig.legend(dummy_lines, likert_labels, 
               loc='lower center', 
               bbox_to_anchor=(0.5, 0.03), 
               ncol=7, 
               frameon=False, 
               fontsize=14, 
               handlelength=1.5, 
               handletextpad=0.5, 
               columnspacing=1.0)
    
    # 凡例両端の補助テキスト
    fig.text(0.20, 0.045, 'Strongly Disagree', 
             ha='right', va='center', fontsize=14, fontweight='bold')
    fig.text(0.80, 0.045, 'Strongly Agree', 
             ha='left', va='center', fontsize=14, fontweight='bold')
    
    # 保存
    output_filename = 'likert_plot_uist2026.png'
    plt.savefig(output_filename, dpi=300)
    print(f"Saved Likert plot as {output_filename}")
    plt.show()

# 実行
draw_likert_grid_uist2026()