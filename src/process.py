# batch_run_score.py
import os
import re
import sys
import shutil
import subprocess
from pathlib import Path

# ===== 設定 =====
EXE = "./src/a.out"
INPUT_DIR = "./in"
OUTPUT_DIR = "./out"
REPORT_FILE = "./report.txt"

# README があるローカルテスターのディレクトリ
# 例: "./tools/ahc063"
TESTER_DIR = "."

# vis.html をケースごとに保存したいなら True
SAVE_VIS_HTML = False
VIS_DIR = "./vis_out"


def parse_score(text: str) -> int:
    """
    vis の標準出力からスコアを抜き出す
    想定:
      Score = 12345
      score: 12345
      12345
    """
    patterns = [
        re.compile(r"score\s*[:=]\s*(-?\d+)", re.IGNORECASE),
        re.compile(r"(-?\d+)")
    ]
    for pat in patterns:
        m = pat.search(text)
        if m:
            return int(m.group(1))
    raise ValueError(f"score を読めませんでした: {text!r}")


def run_solver(exe: str, inp: Path, outp: Path):
    with open(inp, "r") as fin, open(outp, "w") as fout:
        p = subprocess.Popen(
            [exe],
            stdin=fin,
            stdout=fout,
            stderr=subprocess.PIPE,
            text=True,
            bufsize=1,
        )
        stderr_text = []
        if p.stderr is not None:
            for line in p.stderr:
                stderr_text.append(line)
        rc = p.wait()
    return rc, "".join(stderr_text)


def run_visualizer(inp: Path, outp: Path):
    cmd = ["cargo", "run", "-r", "--bin", "vis", str(inp), str(outp)]
    p = subprocess.run(
        cmd,
        cwd=TESTER_DIR,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    return cmd, p.returncode, p.stdout, p.stderr


def main():
    exe = Path(EXE)
    input_dir = Path(INPUT_DIR)
    output_dir = Path(OUTPUT_DIR)
    report_file = Path(REPORT_FILE)

    output_dir.mkdir(parents=True, exist_ok=True)
    if SAVE_VIS_HTML:
        Path(VIS_DIR).mkdir(parents=True, exist_ok=True)

    if not exe.exists():
        print(f"実行ファイルが見つかりません: {exe}", file=sys.stderr)
        sys.exit(1)

    if not input_dir.exists():
        print(f"入力ディレクトリが見つかりません: {input_dir}", file=sys.stderr)
        sys.exit(1)

    files = sorted([p for p in input_dir.iterdir() if p.suffix == ".txt"])
    if not files:
        print(f"{input_dir} に .txt がありません", file=sys.stderr)
        sys.exit(1)

    total_score = 0
    ok_cases = 0

    with open(report_file, "w", encoding="utf-8") as rep:
        rep.write("=== report ===\n\n")

        for idx, inp in enumerate(files, 1):
            outp = output_dir / inp.name
            print(f"[{idx}/{len(files)}] 実行中: {inp.name}", flush=True)

            solver_rc, solver_stderr = run_solver(str(exe), inp, outp)

            rep.write(f"===== {inp.name} =====\n")
            rep.write("[solver stderr]\n")
            rep.write(solver_stderr if solver_stderr else "(no stderr)\n")
            rep.write("\n")

            if solver_rc != 0:
                print(f"  solver失敗: returncode={solver_rc}", flush=True)
                rep.write(f"[solver returncode] {solver_rc}\n\n")
                continue

            cmd, vis_rc, vis_stdout, vis_stderr = run_visualizer(inp, outp)

            rep.write("[visualizer command]\n")
            rep.write(" ".join(cmd) + "\n\n")
            rep.write("[visualizer stdout]\n")
            rep.write(vis_stdout if vis_stdout else "(no stdout)\n")
            rep.write("\n")
            rep.write("[visualizer stderr]\n")
            rep.write(vis_stderr if vis_stderr else "(no stderr)\n")
            rep.write("\n")

            if vis_rc != 0:
                print(f"  vis失敗: returncode={vis_rc}", flush=True)
                rep.write(f"[visualizer returncode] {vis_rc}\n\n")
                continue

            try:
                score = parse_score(vis_stdout)
            except ValueError as e:
                print(f"  スコア取得失敗: {e}", flush=True)
                rep.write(f"[score parse error] {e}\n\n")
                continue

            total_score += score
            ok_cases += 1

            print(f"  score = {score}", flush=True)
            rep.write(f"[parsed score] {score}\n\n")

            if SAVE_VIS_HTML:
                vis_html = Path(TESTER_DIR) / "vis.html"
                if vis_html.exists():
                    shutil.copyfile(vis_html, Path(VIS_DIR) / f"{inp.stem}.html")

        print(f"完了: {ok_cases}/{len(files)} ケース", flush=True)
        print(f"総合スコア: {total_score}", flush=True)

        rep.write("===== summary =====\n")
        rep.write(f"ok_cases = {ok_cases}/{len(files)}\n")
        rep.write(f"total_score = {total_score}\n")


if __name__ == "__main__":
    main()