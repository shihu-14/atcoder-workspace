# gaze_tolerance_analysis.py
# Usage:
#   python gaze_tolerance_analysis.py --input_dir "PATH/TO/gaze_logs" --output_dir "PATH/TO/output" --discard_sec 1.0
#
# Input:
#   Directory that contains per-subject CSV files written by GazeErrorRecorder, like:
#     gaze_error_YYYYMMDD_HHMMSS_<SubjectID>.csv
#
# Output (written into output_dir):
#   - group_summary.csv              : overall summary + recommended tolerance angles
#   - ue_thresholds.csv              : U|E table (U in {0.50,0.75,0.80,0.90,0.95} × E in {0.50,0.75,0.90,0.95})
#   - subject_summary.csv            : per-subject quality + error distribution
#   - subject_e_quantiles.csv        : per-subject E-quantiles (E50/E75/E90/E95)
#   - flag_summary.csv               : per-flag pooled summary (after discard) across subjects (optional but useful)

import argparse
import os
import re
from pathlib import Path

import numpy as np
import pandas as pd


def parse_subject_id_from_filename(path: Path) -> str:
    """
    Expected: .../gaze_error_YYYYMMDD_HHMMSS_<SubjectID>.csv
    Fallback: stem last '_' token.
    """
    stem = path.stem
    parts = stem.split("_")
    if len(parts) >= 2:
        return parts[-1]
    return stem


def safe_quantile(values, q: float, method: str = "linear") -> float:
    """Quantile with numpy version compatibility."""
    v = np.asarray(values, dtype=float)
    v = v[np.isfinite(v)]
    if v.size == 0:
        return float("nan")
    try:
        return float(np.quantile(v, q, method=method))
    except TypeError:
        # older numpy
        return float(np.quantile(v, q, interpolation=method))


def compute_segments(df: pd.DataFrame) -> pd.DataFrame:
    """
    Add 'seg_id' per contiguous run of the same flag_id.
    Assumes df is sorted by time.
    """
    flag = df["flag_id"].to_numpy()
    time = df["time"].to_numpy()

    # boundary if flag changes or time goes backwards (safety)
    flag_change = np.r_[True, flag[1:] != flag[:-1]]
    time_back = np.r_[False, time[1:] < time[:-1]]
    boundary = flag_change | time_back

    seg_id = np.cumsum(boundary) - 1
    df = df.copy()
    df["seg_id"] = seg_id
    return df


def analyze_one_file(path: Path, discard_sec: float) -> dict:
    """
    Returns:
      {
        'subject_id': str,
        'file': str,
        'raw_df': DataFrame (basic cleaned),
        'usable_df': DataFrame (flag_id>=0, after per-segment discard, err_deg finite),
        'subject_summary': dict,
        'subject_e_quantiles': dict,
      }
    """
    subject_id = parse_subject_id_from_filename(path)

    df = pd.read_csv(
        path,
        na_values=["NaN", "nan", "NAN"],
        dtype={"flag_id": "Int64"},
    )

    required_cols = ["time", "flag_id", "err_deg"]
    for c in required_cols:
        if c not in df.columns:
            raise ValueError(f"{path.name}: missing required column '{c}'")

    # numeric conversion
    df["time"] = pd.to_numeric(df["time"], errors="coerce")
    df["flag_id"] = pd.to_numeric(df["flag_id"], errors="coerce").astype("Int64")
    df["err_deg"] = pd.to_numeric(df["err_deg"], errors="coerce")

    df = df.dropna(subset=["time"]).copy()
    df = df.sort_values("time").reset_index(drop=True)

    total_rows = int(len(df))
    rows_with_flag = int((df["flag_id"].fillna(-1) >= 0).sum())
    rows_no_flag = int((df["flag_id"].fillna(-1) < 0).sum())

    # rows where a flag is shown but error is NaN => gaze invalid / missing
    mask_flag = df["flag_id"].fillna(-1) >= 0
    rows_flag_err_nan = int((mask_flag & df["err_deg"].isna()).sum())
    rows_flag_err_ok = int((mask_flag & df["err_deg"].notna()).sum())

    # Estimate dt from time differences (for reporting only)
    dt = df["time"].diff()
    dt_pos = dt[(dt > 0) & (dt < 1.0)]
    dt_median = float(dt_pos.median()) if len(dt_pos) > 0 else float("nan")

    # Segment by contiguous flag_id; discard first discard_sec per segment
    df_flag = df[mask_flag].copy()
    if len(df_flag) > 0:
        df_flag = compute_segments(df_flag)

        # segment start time
        seg_start = df_flag.groupby("seg_id")["time"].transform("min")
        df_flag["seg_t"] = df_flag["time"] - seg_start

        # discard
        df_use = df_flag[df_flag["seg_t"] >= discard_sec].copy()
    else:
        df_use = df_flag.copy()

    # keep only valid errors
    df_use = df_use[df_use["err_deg"].notna()].copy()

    usable_rows = int(len(df_use))
    usable_seconds_approx = float(usable_rows * dt_median) if np.isfinite(dt_median) else float("nan")

    # Per-subject error distribution
    errs = df_use["err_deg"].to_numpy(dtype=float)
    e50 = safe_quantile(errs, 0.50)
    e75 = safe_quantile(errs, 0.75)
    e90 = safe_quantile(errs, 0.90)
    e95 = safe_quantile(errs, 0.95)
    e99 = safe_quantile(errs, 0.99)

    subject_summary = {
        "subject_id": subject_id,
        "file": path.name,
        "total_rows": total_rows,
        "rows_no_flag": rows_no_flag,
        "rows_with_flag": rows_with_flag,
        "rows_flag_err_ok": rows_flag_err_ok,
        "rows_flag_err_nan": rows_flag_err_nan,
        "dt_median_sec": dt_median,
        "discard_sec": discard_sec,
        "usable_rows": usable_rows,
        "usable_seconds_approx": usable_seconds_approx,
        "usable_fraction_of_flag_rows": (usable_rows / rows_with_flag) if rows_with_flag > 0 else float("nan"),
        "err_mean_deg": float(np.nanmean(errs)) if errs.size > 0 else float("nan"),
        "err_std_deg": float(np.nanstd(errs, ddof=1)) if errs.size > 1 else float("nan"),
        "err_min_deg": float(np.nanmin(errs)) if errs.size > 0 else float("nan"),
        "err_max_deg": float(np.nanmax(errs)) if errs.size > 0 else float("nan"),
        "err_p50_deg": e50,
        "err_p75_deg": e75,
        "err_p90_deg": e90,
        "err_p95_deg": e95,
        "err_p99_deg": e99,
        "err_iqr_deg": (e75 - e50) if np.isfinite(e75) and np.isfinite(e50) else float("nan"),
        "segments_count": int(df_flag["seg_id"].nunique()) if len(df_flag) > 0 else 0,
    }

    subject_e_quantiles = {
        "subject_id": subject_id,
        "E50_deg": e50,
        "E75_deg": e75,
        "E90_deg": e90,
        "E95_deg": e95,
        "E99_deg": e99,
        "usable_rows": usable_rows,
    }

    return {
        "subject_id": subject_id,
        "file": path.name,
        "raw_df": df,
        "usable_df": df_use.assign(subject_id=subject_id, file=path.name),
        "subject_summary": subject_summary,
        "subject_e_quantiles": subject_e_quantiles,
    }


def build_ue_table(subject_e_df: pd.DataFrame, u_list, e_list, methods=("linear", "lower", "higher")) -> pd.DataFrame:
    """
    subject_e_df columns: subject_id, E50_deg, E75_deg, E90_deg, E95_deg, ...
    Returns long-form table: U, E, method, threshold_deg
    """
    out_rows = []
    e_map = {
        0.50: "E50_deg",
        0.75: "E75_deg",
        0.90: "E90_deg",
        0.95: "E95_deg",
    }

    for e in e_list:
        col = e_map.get(e)
        if col is None or col not in subject_e_df.columns:
            continue
        vals = subject_e_df[col].to_numpy(dtype=float)

        for u in u_list:
            for m in methods:
                thr = safe_quantile(vals, u, method=m)
                out_rows.append({
                    "U": u,
                    "E": e,
                    "method": m,
                    "threshold_deg": thr,
                    "source": col,
                })

    return pd.DataFrame(out_rows)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--input_dir", required=True, help="Directory containing gaze_error_*.csv files.")
    ap.add_argument("--output_dir", required=True, help="Directory to write result CSV files.")
    ap.add_argument("--discard_sec", type=float, default=1.0, help="Seconds to discard at the start of each flag segment.")
    ap.add_argument("--pattern", default="gaze_error_*.csv", help="Glob pattern for input files.")
    args = ap.parse_args()

    in_dir = Path(args.input_dir)
    out_dir = Path(args.output_dir)
    out_dir.mkdir(parents=True, exist_ok=True)

    files = sorted(in_dir.glob(args.pattern))
    if len(files) == 0:
        raise SystemExit(f"No files matched: {in_dir / args.pattern}")

    all_subject_summaries = []
    all_subject_e = []
    usable_rows_all = []

    for fp in files:
        r = analyze_one_file(fp, discard_sec=args.discard_sec)
        all_subject_summaries.append(r["subject_summary"])
        all_subject_e.append(r["subject_e_quantiles"])
        usable_rows_all.append(r["usable_df"])

    subject_summary_df = pd.DataFrame(all_subject_summaries).sort_values("subject_id").reset_index(drop=True)
    subject_e_df = pd.DataFrame(all_subject_e).sort_values("subject_id").reset_index(drop=True)

    # Pooled usable rows (after discard). This is reference only (unequal samples per subject).
    pooled = pd.concat(usable_rows_all, ignore_index=True)
    pooled_err = pooled["err_deg"].to_numpy(dtype=float)

    pooled_p50 = safe_quantile(pooled_err, 0.50)
    pooled_p75 = safe_quantile(pooled_err, 0.75)
    pooled_p90 = safe_quantile(pooled_err, 0.90)
    pooled_p95 = safe_quantile(pooled_err, 0.95)
    pooled_p99 = safe_quantile(pooled_err, 0.99)

    # Build U|E table
    u_list = [0.50, 0.75, 0.80, 0.90, 0.95]
    e_list = [0.50, 0.75, 0.90, 0.95]
    ue_df = build_ue_table(subject_e_df, u_list=u_list, e_list=e_list)

    # Recommend one tolerance (default): U80|E95 (linear)
    rec_row = ue_df[(ue_df["U"] == 0.80) & (ue_df["E"] == 0.95) & (ue_df["method"] == "linear")]
    rec_tol = float(rec_row["threshold_deg"].iloc[0]) if len(rec_row) > 0 else float("nan")

    # Also include U75|E95 and U90|E95 (linear) for paper-friendly context
    def get_thr(u, e):
        rr = ue_df[(ue_df["U"] == u) & (ue_df["E"] == e) & (ue_df["method"] == "linear")]
        return float(rr["threshold_deg"].iloc[0]) if len(rr) > 0 else float("nan")

    u75e95 = get_thr(0.75, 0.95)
    u90e95 = get_thr(0.90, 0.95)
    u95e95 = get_thr(0.95, 0.95)

    # Group summary (single CSV in long format)
    group_rows = [
        {"metric": "input_dir", "value": str(in_dir)},
        {"metric": "file_count", "value": len(files)},
        {"metric": "subject_count", "value": int(subject_summary_df["subject_id"].nunique())},
        {"metric": "discard_sec_per_flag_segment", "value": args.discard_sec},
        {"metric": "recommended_tolerance_deg (U80|E95, linear)", "value": rec_tol},
        {"metric": "tolerance_deg (U75|E95, linear)", "value": u75e95},
        {"metric": "tolerance_deg (U90|E95, linear)", "value": u90e95},
        {"metric": "tolerance_deg (U95|E95, linear)", "value": u95e95},
        {"metric": "pooled_err_mean_deg (reference)", "value": float(np.nanmean(pooled_err)) if pooled_err.size > 0 else float("nan")},
        {"metric": "pooled_err_std_deg (reference)", "value": float(np.nanstd(pooled_err, ddof=1)) if pooled_err.size > 1 else float("nan")},
        {"metric": "pooled_err_p50_deg (reference)", "value": pooled_p50},
        {"metric": "pooled_err_p75_deg (reference)", "value": pooled_p75},
        {"metric": "pooled_err_p90_deg (reference)", "value": pooled_p90},
        {"metric": "pooled_err_p95_deg (reference)", "value": pooled_p95},
        {"metric": "pooled_err_p99_deg (reference)", "value": pooled_p99},
        {"metric": "mean(E95_i) across subjects", "value": float(np.nanmean(subject_e_df["E95_deg"].to_numpy(dtype=float)))},
        {"metric": "std(E95_i) across subjects", "value": float(np.nanstd(subject_e_df["E95_deg"].to_numpy(dtype=float), ddof=1)) if len(subject_e_df) > 1 else float("nan")},
        {"metric": "median(E95_i) across subjects", "value": safe_quantile(subject_e_df["E95_deg"].to_numpy(dtype=float), 0.50)},
    ]
    group_df = pd.DataFrame(group_rows)

    # Per-flag pooled summary (after discard). Useful to show whether some flags are harder.
    flag_summary = []
    if "flag_id" in pooled.columns and len(pooled) > 0:
        for fid, g in pooled.groupby("flag_id"):
            if pd.isna(fid) or int(fid) < 0:
                continue
            e = g["err_deg"].to_numpy(dtype=float)
            flag_summary.append({
                "flag_id": int(fid),
                "usable_rows": int(len(g)),
                "subjects_contributing": int(g["subject_id"].nunique()),
                "err_mean_deg": float(np.nanmean(e)) if e.size > 0 else float("nan"),
                "err_p50_deg": safe_quantile(e, 0.50),
                "err_p75_deg": safe_quantile(e, 0.75),
                "err_p90_deg": safe_quantile(e, 0.90),
                "err_p95_deg": safe_quantile(e, 0.95),
                "err_max_deg": float(np.nanmax(e)) if e.size > 0 else float("nan"),
            })
    flag_summary_df = pd.DataFrame(flag_summary).sort_values("flag_id").reset_index(drop=True)

    # Write outputs
    subject_summary_path = out_dir / "subject_summary.csv"
    subject_e_path = out_dir / "subject_e_quantiles.csv"
    ue_path = out_dir / "ue_thresholds.csv"
    group_path = out_dir / "group_summary.csv"
    flag_path = out_dir / "flag_summary.csv"

    subject_summary_df.to_csv(subject_summary_path, index=False)
    subject_e_df.to_csv(subject_e_path, index=False)
    ue_df.to_csv(ue_path, index=False)
    group_df.to_csv(group_path, index=False)
    flag_summary_df.to_csv(flag_path, index=False)

    print("Wrote:")
    print(" -", group_path)
    print(" -", ue_path)
    print(" -", subject_summary_path)
    print(" -", subject_e_path)
    print(" -", flag_path)
    print()
    print("Recommended tolerance (deg):", rec_tol)


if __name__ == "__main__":
    main()