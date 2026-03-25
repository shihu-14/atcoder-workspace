import os
import subprocess

# ==== 設定 ====
EXE = "./src/a.out"
INPUT_DIR = "./in"
LOG_PATH = "./ahc054_log_selected_log.txt"
TIMEOUT_SEC = 10.0  # 必要に応じて調整

# 処理したいテスト番号（例）。必要に応じて書き換え。
CASE_IDS = [3, 5, 14, 15, 16, 19, 20, 24, 26, 29, 30, 32, 40, 44, 47, 48, 52, 54, 55, 57, 58, 60, 62, 64, 71, 73, 74, 75, 79, 82, 90, 92, 93, 94, 97, 98, 99]

# ゼロ埋めの桁数（in/0000.txt 形式なら 4）
ZERO_PAD = 4
# ============

def resolve_input_path(case_id: int) -> tuple[str, str] | None:
    """
    指定番号から候補ファイル名を作成し、存在するものを返す。
    戻り値: (実在ファイルのフルパス, 表示用ファイル名)
    """
    candidates = [
        f"{case_id:0{ZERO_PAD}d}.txt",  # 0000.txt 形式
        f"{case_id}.txt",               # 0.txt 形式
    ]
    for name in candidates:
        ipath = os.path.join(INPUT_DIR, name)
        if os.path.isfile(ipath):
            return ipath, name
    return None

total = 0
with open(LOG_PATH, "w", encoding="utf-8") as log:
    for cid in sorted(CASE_IDS):
        resolved = resolve_input_path(cid)
        if not resolved:
            print(f"見つからない: case_id={cid}", file=log, flush=True)
            continue

        ipath, display_name = resolved

        try:
            with open(ipath, "r") as fin:
                r = subprocess.run(
                    [EXE],
                    stdin=fin,
                    stdout=subprocess.PIPE,
                    stderr=log,   # 子プロセスstderrはログへ直行
                    text=True,
                    timeout=TIMEOUT_SEC,
                )
        except subprocess.TimeoutExpired:
            print(f"タイムアウト: {display_name} (> {TIMEOUT_SEC}s)", file=log, flush=True)
            continue
        except Exception as e:
            print(f"起動失敗: {display_name} ({e})", file=log, flush=True)
            continue

        out = (r.stdout or "").strip()
        if not out:
            print(f"スキップ: 出力なし ({display_name})", file=log, flush=True)
            continue

        try:
            tokens = out.split()
            if len(tokens) != 1:
                raise ValueError("複数のトークン")
            score = int(tokens[0])
        except Exception as e:
            print(f"スキップ: {display_name} の出力を整数に変換不可: '{out}' ({e})", file=log, flush=True)
            continue

        # 個別結果（標準出力）
        print(f"{display_name} {score}")
        total += score

        if r.returncode != 0:
            print(f"警告: {display_name} returncode={r.returncode}", file=log, flush=True)

# 合計（標準出力の最後の1行）
print(total)
