import subprocess
import glob
import os
import re
import time
import threading
from dataclasses import dataclass
from concurrent.futures import ThreadPoolExecutor, as_completed
from typing import Optional, List

# =============================
# 設定
# =============================
INPUT_DIR = "in"

SOLVER_PATH = "./a.out"

# cargo run を毎回呼ぶと重いので、最初に build して exe を直接叩く
TESTER_EXE = os.path.join("target", "release", "tester")
BUILD_TESTER_CMD = ["cargo", "build", "-r", "--bin", "tester"]
TESTER_CMD = [TESTER_EXE, SOLVER_PATH]  # tester の引数仕様に合わせて必要なら追加

LOG_DIR = "logs"
LOG_PREFIX = "run"

# 並列数：cargo/tester/solver の負荷が高いなら控えめが安定
MAX_WORKERS = min(6, os.cpu_count() or 4)

# スコア抽出（Score=..., total_score=... の両対応。小数も許容）
SCORE_PATTERNS = [
    re.compile(r"Score\s*=\s*([0-9]+(?:\.[0-9]+)?)"),
    re.compile(r"total_score\s*=\s*([0-9]+(?:\.[0-9]+)?)"),
]

HARNESS_TIMEOUT_SEC = 120


# =============================
# 型
# =============================
@dataclass
class CaseResult:
    input_file: str
    score: Optional[float]
    returncode: int
    elapsed_sec: float
    timed_out: bool
    error_msg: str = ""


# =============================
# ユーティリティ
# =============================
def ensure_dir(path: str) -> None:
    os.makedirs(path, exist_ok=True)


def extract_score(text: str) -> Optional[float]:
    # 複数回出る場合は最後を採用
    last: Optional[float] = None
    for pat in SCORE_PATTERNS:
        for m in pat.finditer(text):
            try:
                last = float(m.group(1))
            except ValueError:
                continue
    return last


def build_tester_if_needed() -> None:
    if not os.path.exists(SOLVER_PATH):
        raise FileNotFoundError(f"solver not found: {SOLVER_PATH}")

    if os.path.exists(TESTER_EXE):
        return

    # tester を1回だけ build（並列ケース実行前にやるのが重要）
    p = subprocess.run(BUILD_TESTER_CMD, capture_output=True, text=True, check=False)
    if p.returncode != 0:
        raise RuntimeError(
            "cargo build failed\n"
            f"cmd={' '.join(BUILD_TESTER_CMD)}\n"
            f"stdout:\n{p.stdout}\n"
            f"stderr:\n{p.stderr}\n"
        )
    if not os.path.exists(TESTER_EXE):
        raise FileNotFoundError(f"tester exe not found after build: {TESTER_EXE}")


_LOG_LOCK = threading.Lock()


def append_log(log_path: str, text: str) -> None:
    with _LOG_LOCK:
        with open(log_path, "a", encoding="utf-8") as lf:
            lf.write(text)
            lf.flush()


def _append_debug_only(log_path: str, input_file: str, stderr_text: str) -> None:
    # .log には debug(=stderr) のみ。区切りだけ最小限付ける
    if not stderr_text:
        return
    base = os.path.basename(input_file)
    if not stderr_text.endswith("\n"):
        stderr_text += "\n"
    append_log(log_path, f"=== CASE {base} ===\n{stderr_text}")


def run_test(input_file: str, log_path: str) -> CaseResult:
    t0 = time.perf_counter()

    try:
        with open(input_file, "r", encoding="utf-8") as f:
            p = subprocess.run(
                TESTER_CMD,
                stdin=f,
                capture_output=True,
                text=True,
                check=False,  # FAILを0に丸めない
                timeout=HARNESS_TIMEOUT_SEC,
            )

        elapsed = time.perf_counter() - t0

        # スコア抽出は stdout/stderr 両方を見る（ただしログに書くのは stderr のみ）
        score = extract_score(p.stderr)
        if score is None:
            score = extract_score(p.stdout)

        # debug情報のみ（stderrのみ）を .log へ
        _append_debug_only(log_path, input_file, p.stderr)

        return CaseResult(
            input_file=input_file,
            score=score,
            returncode=p.returncode,
            elapsed_sec=elapsed,
            timed_out=False,
        )

    except subprocess.TimeoutExpired as e:
        elapsed = time.perf_counter() - t0
        err = e.stderr if isinstance(e.stderr, str) else (e.stderr.decode() if e.stderr else "")

        # timeout時も debug（stderr）のみ
        _append_debug_only(log_path, input_file, err)

        return CaseResult(
            input_file=input_file,
            score=None,
            returncode=124,
            elapsed_sec=elapsed,
            timed_out=True,
            error_msg="TimeoutExpired",
        )

    except OSError as e:
        elapsed = time.perf_counter() - t0
        # OSエラーも stderr 相当として書く（stdoutは書かない）
        _append_debug_only(log_path, input_file, f"OSError: {e}\n")

        return CaseResult(
            input_file=input_file,
            score=None,
            returncode=-1,
            elapsed_sec=elapsed,
            timed_out=False,
            error_msg=f"OSError: {e}",
        )


def main() -> None:
    files = sorted(glob.glob(os.path.join(INPUT_DIR, "*.txt")))
    if not files:
        raise FileNotFoundError(f"No input files found in: {INPUT_DIR}")

    ensure_dir(LOG_DIR)
    ts = time.strftime("%Y%m%d_%H%M%S")
    log_path = os.path.join(LOG_DIR, f"{LOG_PREFIX}_{ts}.log")

    build_tester_if_needed()

    # ログは空で初期化（ヘッダ等は書かない）
    with open(log_path, "w", encoding="utf-8"):
        pass

    print(f"Running {len(files)} cases (max_workers={MAX_WORKERS})...")

    results: List[CaseResult] = []
    done = 0

    with ThreadPoolExecutor(max_workers=MAX_WORKERS) as ex:
        futs = [ex.submit(run_test, f, log_path) for f in files]
        for fut in as_completed(futs):
            r = fut.result()
            results.append(r)
            done += 1
            if done % 10 == 0 or done == len(files):
                print(f"progress: {done}/{len(files)}")

    # 集計（OKのみ）※コンソール表示のみ。logには書かない
    ok_scores = [r.score for r in results if (r.score is not None and r.returncode == 0 and not r.timed_out)]
    fail = [r for r in results if (r.returncode != 0 or r.timed_out or r.score is None)]
    total = sum(ok_scores) if ok_scores else 0.0

    print("-" * 30)
    print(f"Total Score (OK only): {total}")
    print(f"FAIL cases: {len(fail)}")
    print(f"Log saved: {log_path}")


if __name__ == "__main__":
    main()
