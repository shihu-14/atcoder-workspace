import argparse
import csv
import math
from pathlib import Path
from collections import defaultdict
import numpy as np
from typing import Dict, List, Any

# -------------------------
# helpers
# -------------------------
def is_nan(val: Any) -> bool:
    if val is None:
        return True
    if isinstance(val, str):
        return val.strip().lower() == "nan" or val.strip() == ""
    return math.isnan(float(val))

def to_float(val: str) -> float:
    if is_nan(val):
        return np.nan
    try:
        return float(val)
    except ValueError:
        return np.nan

def safe_mean(values: List[float]) -> float:
    """NaNを除外して平均を計算する。有効な値が無い場合はNaNを返す"""
    valid_vals = [v for v in values if not math.isnan(v)]
    if not valid_vals:
        return np.nan
    return sum(valid_vals) / len(valid_vals)

class RecalcParser:
    """_recalc.csv ファイルから必要なセクションを読み取るパーサー"""
    def __init__(self, filepath: Path):
        self.filepath = filepath
        self.results: Dict[str, float] = {}
        
        # セクションごとのデータ保持用
        self.min_dists: List[float] = []
        self.last_rises: List[float] = []
        self.p1_durations: List[float] = []
        
        # pIdごとのイベントカウント（最後の旗を除外するために辞書を使用）
        self.p1_counts_dict: Dict[int, int] = {}
        
        self.parse()

    def parse(self):
        lines = self.filepath.read_text(encoding="utf-8", errors="ignore").splitlines()
        
        current_section = None
        current_pid = None
        
        for ln in lines:
            ln = ln.strip()
            if not ln:
                continue
                
            # RESULTから指定された指標を取得
            if ln.startswith("RESULT,"):
                parts = ln.split(",")
                if len(parts) >= 3:
                    key = parts[1].strip()
                    val = to_float(parts[2].strip())
                    self.results[key] = val
                continue
            
            # --- セクションの切り替わり判定 ---
            if ln.startswith("flagId,passedCorrect,turnDir,minDistToFlagDuringApproach"):
                current_section = "flag"
                continue
            elif ln.startswith("pId,lastRiseSecBeforePassP"):
                current_section = "lastRise"
                continue
            elif ln.startswith("P1_EVENTS_FORMAT"):
                current_section = "p1_events"
                continue
            elif ln == "pId" or ln == "riseSecBeforePassP,duration,fallSecBeforePassP":
                continue

            # --- 各セクションのデータ抽出 ---
            parts = ln.split(",")
            
            if current_section == "flag":
                if len(parts) >= 4 and parts[0] != "flagId":
                    dist = to_float(parts[3])
                    if not math.isnan(dist):
                        self.min_dists.append(dist)
                        
            elif current_section == "lastRise":
                if len(parts) >= 2 and parts[0] != "pId":
                    rise = to_float(parts[1])
                    if not math.isnan(rise):
                        self.last_rises.append(rise)
                        
            elif current_section == "p1_events":
                if len(parts) == 1:
                    if parts[0].lower() != "nan":
                        current_pid = int(parts[0])
                        self.p1_counts_dict[current_pid] = 0
                elif len(parts) >= 3:
                    dur = to_float(parts[1])
                    if not math.isnan(dur):
                        self.p1_durations.append(dur)
                        if current_pid is not None:
                            self.p1_counts_dict[current_pid] += 1

        # P+1が存在しない「一番最後の旗」をカウントから除外する
        if self.p1_counts_dict:
            max_pid = max(self.p1_counts_dict.keys())
            del self.p1_counts_dict[max_pid]

    def get_file_summary(self) -> Dict[str, float]:
        p1_counts_list = list(self.p1_counts_dict.values())
        
        return {
            "rateTimeP0": self.results.get("rateTimeP0", np.nan),
            "rateTimeP1": self.results.get("rateTimeP1", np.nan),
            "rateTimeS0": self.results.get("rateTimeS0", np.nan),
            "rateTimeS1": self.results.get("rateTimeS1", np.nan),
            "rateTimeOther": self.results.get("rateTimeOther", np.nan),
            "passRate": self.results.get("passRate", np.nan),
            "longRate": self.results.get("longRate", np.nan),
            "tightRate": self.results.get("tightRate", np.nan),
            "mean_minDistToFlag": safe_mean(self.min_dists),
            "mean_lastRiseSec": safe_mean(self.last_rises),
            "mean_p1Duration": safe_mean(self.p1_durations),
            "mean_p1CountPerSection": safe_mean(p1_counts_list)
        }

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--input_dir", required=True, help="Path to processed files.")
    ap.add_argument("--output_dir", required=True, help="Path to save aggregated CSVs.")
    args = ap.parse_args()

    in_dir = Path(args.input_dir)
    out_dir = Path(args.output_dir)
    out_dir.mkdir(parents=True, exist_ok=True)

    aggregated_data = defaultdict(lambda: defaultdict(lambda: defaultdict(list)))

    print(f"Scanning processed files in: {in_dir.resolve()} ...")
    
    for csv_path in sorted(in_dir.rglob("*_recalc.csv")):
        # =====================================================================
        # ★ バグ修正箇所: 内部のデータやフォルダ階層に依存せず、
        # ファイル名の先頭文字 (例: "10_CondB_pre-test_recalc.csv") から確実にIDを抽出
        # =====================================================================
        subject_id = csv_path.name.split("_")[0]
        
        try:
            rel_path = csv_path.relative_to(in_dir)
            condition = rel_path.parts[0]
            
            filename = csv_path.name.lower()
            if "_before" in filename:
                phase = "pre-test"
            elif "_after" in filename:
                phase = "post-test"
            else:
                phase = "training"
        except (ValueError, IndexError):
            continue

        parser = RecalcParser(csv_path)
        file_summary = parser.get_file_summary()
        
        # 確実にファイル名から取った subject_id を使用してデータを格納
        aggregated_data[condition][phase][subject_id].append(file_summary)

    target_keys = [
        "rateTimeP0", "rateTimeP1", "rateTimeS0", "rateTimeS1", "rateTimeOther",
        "passRate", "longRate", "tightRate",
        "mean_minDistToFlag", "mean_lastRiseSec", "mean_p1Duration", "mean_p1CountPerSection"
    ]

    # 複数条件・フェーズの統計量を1つのフォルダに書き出す
    for condition, phases in aggregated_data.items():
        for phase, subjects in phases.items():
            # ファイル名に条件とフェーズを組み込み、1つのフォルダに保存
            out_csv_path = out_dir / f"aggregated_stats_{condition}_{phase}.csv"
            
            with out_csv_path.open("w", encoding="utf-8", newline="") as f:
                writer = csv.writer(f)
                header = ["SubjectID"] + target_keys
                writer.writerow(header)
                
                # SubjectID を数値としてソート (1, 2, ..., 10, 11, 12 の順になるように)
                for subj_id in sorted(subjects.keys(), key=lambda x: int(x) if x.isdigit() else x):
                    trials = subjects[subj_id]
                    row = [subj_id]
                    for key in target_keys:
                        trial_vals = [t[key] for t in trials if not math.isnan(t[key])]
                        subj_mean = sum(trial_vals) / len(trial_vals) if trial_vals else np.nan
                        row.append(f"{subj_mean:.6f}" if not math.isnan(subj_mean) else "NaN")
                    writer.writerow(row)
                    
            print(f"Generated: {out_csv_path.name}")

    print("Aggregation completed successfully.")

if __name__ == "__main__":
    main()