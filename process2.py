import os
import subprocess

# 実行可能ファイルのパス
executable = "./src/a.out"

# 入力フォルダの定義（カレントディレクトリ基準）
input_dir = "./in3"

# 出力の合計値を保持する変数
total_output_value = 0

# inフォルダ内の全テキストファイルを処理
for filename in sorted(os.listdir(input_dir)):  # ファイルをソートして順番に処理
    if filename.endswith(".txt"):  # .txt ファイルのみ処理
        try:
            input_path = os.path.join(input_dir, filename)

            # 標準出力に現在処理中のファイルを表示
            print(f"処理中: {filename}")

            # a.out を呼び出し、入力を渡して出力を取得
            result = subprocess.run(
                [executable],
                stdin=open(input_path, "r"),
                stdout=subprocess.PIPE,  # 出力を取得
                universal_newlines=True,
                check=True
            )

            # a.out の出力（stdout）を整数に変換して加算
            output_str = result.stdout.strip()
            if output_str:  # 何か出力がある場合のみ処理
                output_value = int(output_str)
                total_output_value += output_value

        except ValueError:
            print(f"スキップ: ファイル名 {filename} または出力 '{result.stdout.strip()}' が整数に変換できません。")
        except subprocess.CalledProcessError as e:
            print(f"エラー: {filename} を処理中に異常終了。メッセージ: {e}")
        except Exception as e:
            print(f"例外発生: {e}")

# 最終的な合計値を出力
print(f"---\n合計出力値: {total_output_value}")
