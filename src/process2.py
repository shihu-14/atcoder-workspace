
import os
import subprocess

EXE = "./src/a.out"
INPUT_DIR = "./in"
LOG_PATH = "./ahc054_log.txt"
TIMEOUT_SEC = 10.0  # 必要に応じて調整

total = 0
error_count = 0

with open(LOG_PATH, "w", encoding="utf-8") as log:
    for filename in sorted(os.listdir(INPUT_DIR)):
        if not filename.endswith(".txt"):
            continue

        ipath = os.path.join(INPUT_DIR, filename)
        print(f"処理中: {filename}", file=log, flush=True)

        try:
            with open(ipath, "r") as fin:
                r = subprocess.run(
                    [EXE],
                    stdin=fin,
                    stdout=subprocess.PIPE,
                    stderr=log,          # 子プロセスstderrはログへ直行
                    text=True,
                    timeout=TIMEOUT_SEC,
                )
        except subprocess.TimeoutExpired:
            print(f"タイムアウト: {filename} (> {TIMEOUT_SEC}s)", file=log, flush=True)
            error_count += 1
            continue
        except Exception as e:
            print(f"起動失敗: {filename} ({e})", file=log, flush=True)
            error_count += 1
            continue

        out = (r.stdout or "").strip()
        if not out:
            print(f"スキップ: 出力なし ({filename})", file=log, flush=True)
            error_count += 1
            continue

        try:
            tokens = out.split()
            if len(tokens) != 1:
                raise ValueError("複数のトークン")
            total += int(tokens[0])
        except Exception as e:
            print(f"スキップ: {filename} の出力を整数に変換不可: '{out}' ({e})", file=log, flush=True)
            error_count += 1
            continue

        if r.returncode != 0:
            print(f"警告: {filename} returncode={r.returncode}", file=log, flush=True)

# 合計とエラー件数は標準出力（各1行）
print(total)
print(error_count)
