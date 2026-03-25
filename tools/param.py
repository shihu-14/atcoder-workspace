#!/usr/bin/env python3
import argparse
import glob
import os
import random
import re
import subprocess
import tempfile
import time
from concurrent.futures import ProcessPoolExecutor, as_completed
from typing import Dict, List, Tuple, Optional

SCORE_RE = re.compile(r"(-?\d+(?:\.\d+)?)")

DEFAULT_PARAMS = {
    "time_limit": 1.94,
    "ucb_c": 0.35,
    "t_early": 30,
    "t_mid": 70,
    "depth_early": 6,
    "depth_mid": 8,
    "depth_late": 10,
    "base_wa": 0.6,
    "base_wb": 0.6,
    "base_wc": 0.6,
    "base_wd": 0.6,
    "base_eps": 0.25,
    "my_empty_mult": 3.0,
    "my_own_ltU_threat_mult": 6.0,
    "my_own_ltU_safe_mult": 0.6,
    "my_own_eqU_threat_mult": 0.8,
    "my_own_eqU_safe_mult": 0.05,
    "my_enemy_lvl1_leader_mult": 10.0,
    "my_enemy_lvl1_other_mult": 6.0,
    "my_enemy_lvlge2_leader_mult": 1.2,
    "my_enemy_lvlge2_other_mult": 0.8,
    "my_min_w": 0.01,
    "fast_empty_mult": 3.0,
    "fast_own_ltU_mult": 1.0,
    "fast_own_eqU_mult": 0.05,
    "fast_enemy_lvl1_leader_mult": 10.0,
    "fast_enemy_lvl1_other_mult": 6.0,
    "fast_enemy_lvlge2_leader_mult": 0.9,
    "fast_enemy_lvlge2_other_mult": 0.6,
}


def write_params_file(path: str, params: Dict[str, float]) -> None:
    with open(path, "w", encoding="utf-8") as f:
        for k, v in params.items():
            f.write(f"{k}={v}\n")


def extract_score(stderr_text: str) -> float:
    nums = SCORE_RE.findall(stderr_text)
    if not nums:
        raise RuntimeError("Failed to parse score from tester stderr:\n" + stderr_text)
    return float(nums[-1])


def _base_env_one_thread() -> Dict[str, str]:
    # 並列時の過負荷を避ける（OpenMP / Rayon / BLAS などが勝手にスレッドを増やすのを抑止）
    env = dict(os.environ)
    env["OMP_NUM_THREADS"] = "1"
    env["OPENBLAS_NUM_THREADS"] = "1"
    env["MKL_NUM_THREADS"] = "1"
    env["VECLIB_MAXIMUM_THREADS"] = "1"
    env["NUMEXPR_NUM_THREADS"] = "1"
    env["RAYON_NUM_THREADS"] = "1"
    return env


def run_one_case(
    tester_bin: str,
    solver_cmd_with_param: List[str],
    in_path: str,
    timeout_sec: Optional[float],
    isolate_cwd: bool,
) -> float:
    with open(in_path, "rb") as fin:
        env = _base_env_one_thread()

        if isolate_cwd:
            with tempfile.TemporaryDirectory() as td:
                proc = subprocess.run(
                    [tester_bin] + solver_cmd_with_param,
                    stdin=fin,
                    stdout=subprocess.DEVNULL,
                    stderr=subprocess.PIPE,
                    text=True,
                    check=False,
                    timeout=timeout_sec,
                    cwd=td,
                    env=env,
                )
        else:
            proc = subprocess.run(
                [tester_bin] + solver_cmd_with_param,
                stdin=fin,
                stdout=subprocess.DEVNULL,
                stderr=subprocess.PIPE,
                text=True,
                check=False,
                timeout=timeout_sec,
                env=env,
            )

    if proc.returncode != 0:
        raise RuntimeError(
            f"tester failed for {in_path} (code={proc.returncode})\n{proc.stderr}"
        )
    return extract_score(proc.stderr)


def total_score_parallel(
    tester_bin: str,
    solver_cmd: List[str],
    in_files: List[str],
    params: Dict[str, float],
    jobs: int,
    timeout_sec: Optional[float],
    isolate_cwd: bool,
    case_progress: bool,
    progress_interval_sec: float = 2.0,  # 追加：間引き表示の間隔
) -> float:
    # 1 trial（1 パラメータ候補）につき一時 params ファイルを1つ作り、それを全ケースで共有
    with tempfile.NamedTemporaryFile("w", delete=False, encoding="utf-8", suffix=".txt") as tf:
        param_path = tf.name

    try:
        write_params_file(param_path, params)
        cmd_with_param = solver_cmd + [param_path]

        total_cases = len(in_files)
        s = 0.0
        done = 0

        # 追加：進捗表示（case_progress=Trueなら毎回、Falseなら一定間隔）
        start_t = time.time()
        last_print_t = start_t

        def report(force: bool = False) -> None:
            nonlocal last_print_t
            now = time.time()
            if force or case_progress or (now - last_print_t) >= progress_interval_sec:
                elapsed = now - start_t
                print(f"  progress: {done}/{total_cases} score_sum={s:.6f} elapsed={elapsed:.2f}s", flush=True)
                last_print_t = now

        if jobs <= 1:
            for p in in_files:
                s += run_one_case(tester_bin, cmd_with_param, p, timeout_sec, isolate_cwd)
                done += 1
                report(force=(done == total_cases))
            return s

        # ケース単位で並列化
        with ProcessPoolExecutor(max_workers=jobs) as ex:
            futs = [
                ex.submit(run_one_case, tester_bin, cmd_with_param, p, timeout_sec, isolate_cwd)
                for p in in_files
            ]
            for fut in as_completed(futs):
                s += fut.result()
                done += 1
                report(force=(done == total_cases))
        return s
    finally:
        try:
            os.remove(param_path)
        except OSError:
            pass


def clamp(x: float, lo: float, hi: float) -> float:
    return lo if x < lo else hi if x > hi else x


def propose_params(rng: random.Random, base: Dict[str, float]) -> Dict[str, float]:
    p = dict(base)

    def mul(key: str, lo: float, hi: float, scale: float):
        factor = pow(2.718281828, rng.uniform(-scale, scale))
        p[key] = clamp(p[key] * factor, lo, hi)

    mul("ucb_c", 0.05, 1.0, 0.6)
    mul("time_limit", 0.1, 10.0, 0.1)

    for k in ["depth_early", "depth_mid", "depth_late"]:
        p[k] = int(clamp(int(p[k] + rng.choice([-2, -1, 0, 1, 2])), 0, 30))

    mul("my_empty_mult", 0.1, 20.0, 0.4)
    mul("my_own_ltU_threat_mult", 0.1, 30.0, 0.4)
    mul("my_enemy_lvl1_leader_mult", 0.1, 40.0, 0.4)
    mul("fast_enemy_lvl1_leader_mult", 0.1, 40.0, 0.4)

    mul("my_own_eqU_safe_mult", 0.0001, 1.0, 0.3)
    mul("fast_own_eqU_mult", 0.0001, 1.0, 0.3)

    return p


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--tester", required=True, help="tester binary path (e.g. ./target/release/tester)")
    ap.add_argument("--solver", required=True, help="solver binary path (e.g. ./a.out)")
    ap.add_argument("--in_dir", default="in", help="input directory (default: in)")
    ap.add_argument("--iters", type=int, default=50, help="number of parameter trials")
    ap.add_argument("--seed", type=int, default=0, help="random seed for tuning")
    ap.add_argument("--topk", type=int, default=10, help="keep top-k results")

    ap.add_argument("--jobs", type=int, default=1, help="parallel workers for cases (default: 1)")
    ap.add_argument("--timeout", type=float, default=None, help="timeout seconds per case (default: no timeout)")
    ap.add_argument("--isolate_cwd", action="store_true", help="run each case in its own temporary cwd to avoid file conflicts")

    # 既存：毎ケース表示したい人向け（progressは常に出るが、これは「毎ケース強制」にする）
    ap.add_argument("--case_progress", action="store_true", help="print progress for every case (very verbose)")

    args = ap.parse_args()

    in_files = sorted(glob.glob(os.path.join(args.in_dir, "*")))
    if not in_files:
        raise RuntimeError(f"No input files found in {args.in_dir}")

    rng = random.Random(args.seed)
    solver_cmd = [args.solver]

    best: List[Tuple[float, Dict[str, float]]] = []

    # デフォルト
    print(f"[0] start: cases={len(in_files)} jobs={max(1,args.jobs)} isolate_cwd={args.isolate_cwd} timeout={args.timeout}", flush=True)
    t0 = time.time()
    base_score = total_score_parallel(
        args.tester,
        solver_cmd,
        in_files,
        DEFAULT_PARAMS,
        jobs=max(1, args.jobs),
        timeout_sec=args.timeout,
        isolate_cwd=args.isolate_cwd,
        case_progress=args.case_progress,
    )
    best.append((base_score, dict(DEFAULT_PARAMS)))
    best.sort(key=lambda x: x[0], reverse=True)
    best = best[:args.topk]
    print(f"[0] total_score={base_score} elapsed={time.time()-t0:.2f}s", flush=True)

    for i in range(1, args.iters + 1):
        cand = propose_params(rng, DEFAULT_PARAMS)

        print(f"[{i}] start: cases={len(in_files)} jobs={max(1,args.jobs)} isolate_cwd={args.isolate_cwd} timeout={args.timeout}", flush=True)
        t1 = time.time()
        sc = total_score_parallel(
            args.tester,
            solver_cmd,
            in_files,
            cand,
            jobs=max(1, args.jobs),
            timeout_sec=args.timeout,
            isolate_cwd=args.isolate_cwd,
            case_progress=args.case_progress,
        )

        best.append((sc, cand))
        best.sort(key=lambda x: x[0], reverse=True)
        best = best[:args.topk]

        print(f"[{i}] total_score={sc} best={best[0][0]} elapsed={time.time()-t1:.2f}s", flush=True)

    print("\n=== TOP RESULTS ===")
    for rank, (sc, p) in enumerate(best, 1):
        print(f"\n#{rank} total_score={sc}")
        for k in sorted(p.keys()):
            print(f"{k}={p[k]}")


if __name__ == "__main__":
    main()
