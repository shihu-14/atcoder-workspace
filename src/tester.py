import subprocess
import os
import glob
import time
from pathlib import Path

# --- 設定項目 ---
EXECUTABLE = "./a.out"            # 実行ファイル
INPUT_DIR = "./in"                # 入力ディレクトリ
OUTPUT_DIR = "./output"           # 出力ディレクトリ
COMBINED_LOG_FILE = "all_stderr.log" # cerrの内容をすべてまとめるファイル
SUMMARY_FILE = "summary.txt"      # 各ケースの結果一覧

def run_test():
    os.makedirs(OUTPUT_DIR, exist_ok=True)

    input_files = sorted(glob.glob(f"{INPUT_DIR}/*.txt"))
    if not input_files:
        print(f"❌ {INPUT_DIR} に入力ファイルが見つかりません。")
        return

    # ログファイルを新規作成（既存のものは上書き）
    with open(COMBINED_LOG_FILE, "w") as f_log:
        f_log.write(f"=== All Test Logs (Run at {time.ctime()}) ===\n\n")

    print(f"📂 {len(input_files)} 件のテストケースを実行し、ログを {COMBINED_LOG_FILE} に集約します...")
    
    summary_data = []

    for idx, in_path in enumerate(input_files):
        case_name = Path(in_path).stem
        out_path = os.path.join(OUTPUT_DIR, f"{case_name}.txt")

        print(f"[{idx+1}/{len(input_files)}] Running {case_name}...", end="\r")

        # cerr出力を一時的に保持するためのバッファ
        stderr_output = ""
        start_time = time.time()
        
        try:
            with open(in_path, "r") as fin, open(out_path, "w") as fout:
                # 実行 (stderr=subprocess.PIPE でキャプチャ)
                result = subprocess.run(
                    [EXECUTABLE],
                    stdin=fin,
                    stdout=fout,
                    stderr=subprocess.PIPE,
                    text=True,
                    timeout=4.0 # セーフティ
                )
                stderr_output = result.stderr
            
            elapsed = time.time() - start_time
            
            # まとめてログファイルに書き込み
            with open(COMBINED_LOG_FILE, "a") as f_log:
                f_log.write(f"--- Case: {case_name} (Time: {elapsed:.3f}s) ---\n")
                f_log.write(stderr_output)
                f_log.write("\n" + "="*40 + "\n\n")

            # 集計用スコア抽出
            score = extract_value(stderr_output, "Final Best: ")
            summary_data.append(f"{case_name}: Score = {score:14}, Time = {elapsed:.3f}s")

        except subprocess.TimeoutExpired:
            summary_data.append(f"{case_name}: TIMEOUT")
        except Exception as e:
            summary_data.append(f"{case_name}: ERROR ({str(e)})")

    # サマリー（スコア一覧）の保存
    with open(SUMMARY_FILE, "w") as f_sum:
        f_sum.write("\n".join(summary_data))

    print(f"\n\n✅ 完了！")
    print(f"📄 全ログ集約先: {COMBINED_LOG_FILE}")
    print(f"📄 スコア一覧: {SUMMARY_FILE}")

def extract_value(text, keyword):
    """ログからスコア等の数値を抽出"""
    try:
        if keyword in text:
            # キーワードの後の最初の連続した数字を拾う
            line = [l for l in text.split('\n') if keyword in l][-1]
            return line.split(keyword)[1].strip().split()[0]
    except:
        pass
    return "N/A"

if __name__ == "__main__":
    run_test()