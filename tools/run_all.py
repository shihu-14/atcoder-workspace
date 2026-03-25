import subprocess
import glob
import re
from concurrent.futures import ThreadPoolExecutor
import os

# 設定
input_dir = "in"               # 入力ファイルのディレクトリ
command = "cargo run -r --bin tester ./a.out" # 実行コマンド
# Windowsでバイナリ直接実行の場合は以下のように書き換えてください
# command = "./tester.exe ./a.out" 

def run_test(input_file):
    # コマンドを構築 (< in.txt の部分はPythonで処理します)
    cmd = command.split()
    
    try:
        # 入力ファイルを開く
        with open(input_file, "r") as f:
            # プロセス実行
            result = subprocess.run(
                cmd,
                stdin=f,
                capture_output=True,
                text=True,
                check=True
            )
        
        # 標準エラー出力からスコアを抽出 (形式: "Score = 1234")
        # 出力形式に合わせて正規表現は調整が必要かもしれません
        output = result.stderr
        match = re.search(r"Score = (\d+)", output)
        if match:
            score = int(match.group(1))
            return input_file, score
        else:
            return input_file, 0 # スコアが見つからない場合

    except subprocess.CalledProcessError as e:
        print(f"Error in {input_file}: {e}")
        return input_file, 0

def main():
    # 入力ファイル一覧を取得 (0000.txt ～ 0099.txt)
    files = sorted(glob.glob(os.path.join(input_dir, "*.txt")))
    
    total_score = 0
    results = []

    print(f"Running {len(files)} cases in parallel...")

    # 並列実行 (max_workers=NoneでCPUコア数に合わせて自動調整)
    with ThreadPoolExecutor() as executor:
        for file, score in executor.map(run_test, files):
            print(f"{os.path.basename(file)}: {score}")
            total_score += score
            results.append((file, score))

    print("-" * 30)
    print(f"Total Score: {total_score}")

if __name__ == "__main__":
    main()