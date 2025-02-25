import os
import subprocess

# 実行可能ファイルのパス
executable = "./src/a.out"

# 入力フォルダと出力フォルダの定義（カレントディレクトリ基準）
input_dir = "./in"
output_dir = "./out"

# 出力フォルダを作成（存在しない場合）
os.makedirs(output_dir, exist_ok=True)

# inフォルダ内の全テキストファイルを処理
for filename in sorted(os.listdir(input_dir)):  # ファイルをソートして順番に処理
    if filename.endswith(".txt"):  # .txt ファイルのみ処理
        try:
            # ファイル名から番号部分を取得
            base_name, ext = os.path.splitext(filename)  # "0000.txt" → ("0000", ".txt")
            file_number = int(base_name)  # 文字列 "0000" → 整数 0
            new_file_number = file_number + 1  # 番号を+1
            new_filename = f"{new_file_number:04d}{ext}"  # 4桁のゼロパディング

            input_path = os.path.join(input_dir, filename)
            output_path = os.path.join(output_dir, new_filename)
            
            # 標準出力に現在処理中のファイルを表示
            print(f"処理中: {filename} → {new_filename}")

            # a.out を呼び出し、入力を渡して出力を取得
            with open(input_path, "r") as infile, open(output_path, "w") as outfile:
                subprocess.run([executable], stdin=infile, stdout=outfile, check=True)

        except ValueError:
            print(f"スキップ: ファイル名 {filename} は整数に変換できません")

