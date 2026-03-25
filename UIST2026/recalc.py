import argparse
import csv
import math
from pathlib import Path
from typing import Dict, List, Tuple, Optional, Any

# -------------------------
# small helpers
# -------------------------
def is_nan(x: float) -> bool:
    return math.isnan(x)

def f32(s: str) -> float:
    s = s.strip()
    if s == "" or s.lower() == "nan":
        return float("nan")
    return float(s)

def fmt_float(x: float) -> str:
    if is_nan(x):
        return "NaN"
    return f"{x:.6f}"

def dot(a: Tuple[float, float, float], b: Tuple[float, float, float]) -> float:
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]

def sub(a: Tuple[float, float, float], b: Tuple[float, float, float]) -> Tuple[float, float, float]:
    return (a[0] - b[0], a[1] - b[1], a[2] - b[2])

def add(a: Tuple[float, float, float], b: Tuple[float, float, float]) -> Tuple[float, float, float]:
    return (a[0] + b[0], a[1] + b[1], a[2] + b[2])

def mul(a: Tuple[float, float, float], s: float) -> Tuple[float, float, float]:
    return (a[0] * s, a[1] * s, a[2] * s)

def norm(a: Tuple[float, float, float]) -> float:
    return math.sqrt(dot(a, a))

def normalize(a: Tuple[float, float, float]) -> Optional[Tuple[float, float, float]]:
    n = norm(a)
    if n < 1e-12:
        return None
    return (a[0] / n, a[1] / n, a[2] / n)

def angle_deg(a: Tuple[float, float, float], b: Tuple[float, float, float]) -> float:
    na = normalize(a)
    nb = normalize(b)
    if na is None or nb is None:
        return float("inf")
    d = dot(na, nb)
    d = max(-1.0, min(1.0, d))
    return math.degrees(math.acos(d))

def rot_x(v: Tuple[float, float, float], deg: float) -> Tuple[float, float, float]:
    a = math.radians(deg)
    c = math.cos(a)
    s = math.sin(a)
    x, y, z = v
    return (x, c * y - s * z, s * y + c * z)


# -------------------------
# parsing
# -------------------------
class ParsedLog:
    def __init__(self) -> None:
        self.header_cols: List[str] = []
        self.data_rows: List[Dict[str, float]] = []

        self.pref_scalar: Dict[str, str] = {}
        self.pref_vec3: Dict[str, Tuple[str, str, str]] = {}
        self.result_scalar: Dict[str, str] = {}
        self.flag_section_lines: List[str] = []

BASIC_PREF_KEYS = [
    "CourseWidth", "SlopeAngle", "PlayerFSpeed", "PlayerHSpeed", "CameraHeight",
    "SubjectID", "Method", "RandomSeed", "Blur_OuterAngleDeg", "Blur_SigmaPx",
]

def parse_csv_log(path: Path) -> ParsedLog:
    lines = path.read_text(encoding="utf-8", errors="ignore").splitlines()

    header_idx = -1
    header_line = None
    for i, ln in enumerate(lines):
        if ln.startswith("time,"):
            header_idx = i
            header_line = ln.strip()
            break
    if header_idx == -1 or header_line is None:
        raise RuntimeError(f"Header line starting with 'time,' not found in {path.name}.")

    header_line = header_line.replace("gaze_dir_zp0Id", "gaze_dir_z,p0Id")
    header_line = header_line.replace("gaze_dir_zp0_x", "gaze_dir_z,p0_x")
    header_cols = [c.strip() for c in header_line.split(",")]

    out = ParsedLog()
    out.header_cols = header_cols

    in_flag_section = False

    for ln in lines[header_idx + 1 :]:
        if not ln.strip():
            continue

        if ln.startswith("flagId,passedCorrect,turnDir,minDistToFlagDuringApproach"):
            in_flag_section = True
            out.flag_section_lines.append(ln.rstrip("\n"))
            continue

        if in_flag_section:
            if ln.startswith("pId,") or ln.startswith("P1_EVENTS_FORMAT") or ln.startswith("RESULT,") or ln.startswith("PREF,") or ln.startswith("START_REL_SEC,") or ln.startswith("SETTING,") or ln.startswith("time,"):
                in_flag_section = False
            else:
                out.flag_section_lines.append(ln.rstrip("\n"))
                continue

        if ln.startswith("PREF,"):
            parts = next(csv.reader([ln]))
            if len(parts) >= 3:
                key = parts[1].strip().strip('"')
                if key in ("SnowPlaneNormal", "SnowPlanePoint") and len(parts) >= 5:
                    out.pref_vec3[key] = (parts[2].strip(), parts[3].strip(), parts[4].strip())
                else:
                    out.pref_scalar[key] = parts[2].strip()
            continue

        if ln.startswith("RESULT,"):
            parts = next(csv.reader([ln]))
            if len(parts) >= 3:
                k = parts[1].strip()
                v = parts[2].strip()
                out.result_scalar[k] = v
            continue

        if ln.startswith("SETTING,") or ln.startswith("START_REL_SEC,") or ln.startswith("P1_EVENTS_FORMAT") or ln.startswith("pId,"):
            continue

        parts = next(csv.reader([ln]))
        if len(parts) != len(header_cols):
            continue

        row: Dict[str, float] = {}
        for c, p in zip(header_cols, parts):
            try:
                row[c] = f32(p)
            except Exception:
                row[c] = float("nan")
        out.data_rows.append(row)

    return out

# -------------------------
# reconstruction (CalcPassRate2 logic)
# -------------------------
class P1EventType:
    Rise = 0
    Duration = 1
    Fall = 2

def recompute_from_rows(
    rows: List[Dict[str, float]],
    threshold_deg: float,
    course_width: float,
    snow_n: Tuple[float, float, float],
    snow_p: Tuple[float, float, float],
    slope_angle_deg: float,
    player_f_speed: float,
    total_flags_denom: int,
) -> Dict[str, Any]:
    fwd = normalize(rot_x((0.0, 0.0, 1.0), slope_angle_deg))
    if fwd is None:
        fwd = (0.0, 0.0, 1.0)

    sn = normalize(snow_n) or (0.0, 1.0, 0.0)

    timeP0 = timeP1 = timeS0 = timeS1 = timeOther = 0.0
    totalValidTime = 0.0
    cntP0 = cntP1 = cntS0 = cntS1 = 0
    counted_p0: set[int] = set()
    counted_p1: set[int] = set()

    prevLookingS0 = prevLookingS1 = False

    p1_events: Dict[int, List[Tuple[int, int, float, float]]] = {}
    prev_pId = -1
    prev_isLookingP1 = False
    seg_accum = 0.0
    seg_target_p1id = -1

    def get_time(i: int) -> float:
        return float(rows[i]["time"])

    for i in range(1, len(rows)):
        t_prev = get_time(i - 1)
        t_cur = get_time(i)
        dt = t_cur - t_prev
        if not (dt > 0.0):
            continue

        r = rows[i]
        gaze_o = (float(r.get("gaze_o_x", float("nan"))), float(r.get("gaze_o_y", float("nan"))), float(r.get("gaze_o_z", float("nan"))))
        gaze_d = (float(r.get("gaze_dir_x", float("nan"))), float(r.get("gaze_dir_y", float("nan"))), float(r.get("gaze_dir_z", float("nan"))))
        cam_p = (float(r.get("cam_x", float("nan"))), float(r.get("cam_y", float("nan"))), float(r.get("cam_z", float("nan"))))

        p0Id = int(r.get("p0Id", -1)) if not is_nan(float(r.get("p0Id", float("nan")))) else -1
        p1Id = int(r.get("p1Id", -1)) if not is_nan(float(r.get("p1Id", float("nan")))) else -1

        p0Pos = (float(r.get("p0_x", float("nan"))), float(r.get("p0_y", float("nan"))), float(r.get("p0_z", float("nan"))))
        p1Pos = (float(r.get("p1_x", float("nan"))), float(r.get("p1_y", float("nan"))), float(r.get("p1_z", float("nan"))))

        if is_nan(gaze_o[0]) or is_nan(gaze_d[0]): continue
        gd = normalize(gaze_d)
        if gd is None: continue

        if p0Id < 0 or is_nan(p0Pos[0]): continue
        hasP1 = (p1Id >= 0) and (not is_nan(p1Pos[0]))

        cls = "Other"
        a0 = angle_deg(gd, sub(p0Pos, gaze_o))
        p0Ok = (a0 <= threshold_deg)

        p1Ok = False
        if hasP1:
            a1 = angle_deg(gd, sub(p1Pos, gaze_o))
            p1Ok = (a1 <= threshold_deg)

        if p0Ok or p1Ok:
            if p0Ok and p1Ok:
                cls = "P1"
                # cls = "P1" if a1 < a0 else "P0"
            elif p1Ok: cls = "P1"
            else: cls = "P0"
        else:
            denom = dot(sn, gd)
            if abs(denom) > 1e-6:
                tHit = dot(sn, sub(snow_p, gaze_o)) / denom
                if tHit > 0.0:
                    hit = add(gaze_o, mul(gd, tHit))
                    inCourse = (abs(hit[0]) <= course_width)
                    if inCourse:
                        zPlayer = gaze_o[2]
                        zP0 = p0Pos[2]
                        zHit = hit[2]
                        mn0, mx0 = min(zPlayer, zP0), max(zPlayer, zP0)
                        if mn0 <= zHit < mx0:
                            cls = "S0"
                        elif hasP1:
                            zP1 = p1Pos[2]
                            mn1, mx1 = min(zP0, zP1), max(zP0, zP1)
                            if mn1 <= zHit < mx1:
                                cls = "S1"

        totalValidTime += dt
        if cls == "P0": timeP0 += dt
        elif cls == "P1": timeP1 += dt
        elif cls == "S0": timeS0 += dt
        elif cls == "S1": timeS1 += dt
        else: timeOther += dt

        isS0 = (cls == "S0")
        isS1 = (cls == "S1")
        if isS0 and not prevLookingS0: cntS0 += 1
        if isS1 and not prevLookingS1: cntS1 += 1
        prevLookingS0 = isS0
        prevLookingS1 = isS1

        if cls == "P0":
            if p0Id not in counted_p0:
                cntP0 += 1
                counted_p0.add(p0Id)
        elif cls == "P1" and hasP1:
            if p1Id not in counted_p1:
                cntP1 += 1
                counted_p1.add(p1Id)

        if p0Id not in p1_events: p1_events[p0Id] = []
        secBeforePassP = 0.0
        if player_f_speed > 1e-6:
            alongP0 = dot(sub(p0Pos, gaze_o), fwd)
            secBeforePassP = (alongP0 / player_f_speed) if (alongP0 > 0.0) else 0.0

        isNowLookingP1 = (cls == "P1")

        if prev_pId != -1 and prev_pId != p0Id:
            if prev_isLookingP1:
                p1_events[prev_pId].append((P1EventType.Duration, seg_target_p1id, float("nan"), seg_accum))
                p1_events[prev_pId].append((P1EventType.Fall, seg_target_p1id, 0.0, 0.0))
            prev_isLookingP1 = False
            seg_accum = 0.0
            seg_target_p1id = -1

        if isNowLookingP1 and not prev_isLookingP1:
            seg_accum = 0.0
            seg_target_p1id = p1Id
            p1_events[p0Id].append((P1EventType.Rise, p1Id, secBeforePassP, 0.0))

        if isNowLookingP1: seg_accum += dt

        if (not isNowLookingP1) and prev_isLookingP1:
            p1_events[p0Id].append((P1EventType.Duration, seg_target_p1id, float("nan"), seg_accum))
            p1_events[p0Id].append((P1EventType.Fall, seg_target_p1id, secBeforePassP, 0.0))
            seg_accum = 0.0
            seg_target_p1id = -1

        prev_pId = p0Id
        prev_isLookingP1 = isNowLookingP1

    if prev_pId != -1 and prev_isLookingP1:
        p1_events[prev_pId].append((P1EventType.Duration, seg_target_p1id, float("nan"), seg_accum))
        p1_events[prev_pId].append((P1EventType.Fall, seg_target_p1id, 0.0, 0.0))

    denom_t = totalValidTime if totalValidTime > 0.0 else float("nan")
    rateTimeP0 = (timeP0 / denom_t) if not is_nan(denom_t) else float("nan")
    rateTimeP1 = (timeP1 / denom_t) if not is_nan(denom_t) else float("nan")
    rateTimeS0 = (timeS0 / denom_t) if not is_nan(denom_t) else float("nan")
    rateTimeS1 = (timeS1 / denom_t) if not is_nan(denom_t) else float("nan")
    rateTimeOther = (timeOther / denom_t) if not is_nan(denom_t) else float("nan")

    denom_cnt = float(total_flags_denom) if total_flags_denom > 0 else float("nan")
    rateCntP0 = (cntP0 / denom_cnt) if not is_nan(denom_cnt) else float("nan")
    rateCntP1 = (cntP1 / denom_cnt) if not is_nan(denom_cnt) else float("nan")

    lastRise: Dict[int, float] = {}
    for pid, evs in p1_events.items():
        lr = float("nan")
        for (typ, _p1id, sec, _dur) in evs:
            if typ == P1EventType.Rise: lr = sec
        lastRise[pid] = lr

    return {
        "totalValidTime_calc": totalValidTime,
        "timeP0": timeP0, "timeP1": timeP1, "timeS0": timeS0, "timeS1": timeS1, "timeOther": timeOther,
        "cntP0": cntP0, "cntP1": cntP1, "cntS0": cntS0, "cntS1": cntS1,
        "rateTimeP0": rateTimeP0, "rateTimeP1": rateTimeP1, "rateTimeS0": rateTimeS0, "rateTimeS1": rateTimeS1, "rateTimeOther": rateTimeOther,
        "rateCntP0": rateCntP0, "rateCntP1": rateCntP1,
        "p1_events": p1_events, "lastRise": lastRise,
    }


def write_recalc_output(
    out_path: Path, parsed: ParsedLog, threshold_deg: float, recomputed: Dict[str, Any]
) -> None:
    def result_val(key: str) -> str: return parsed.result_scalar.get(key, "NaN")
    def pref_vec3_line(key: str) -> str:
        if key in parsed.pref_vec3:
            x, y, z = parsed.pref_vec3[key]
            return f'PREF,"{key}",{x},{y},{z}'
        return f'PREF,"{key}",NaN,NaN,NaN'

    with out_path.open("w", encoding="utf-8", newline="") as f:
        w = f.write

        for k in BASIC_PREF_KEYS:
            actual_key = k
            if k == "Blur_OuterAngleDeg" and "Blur_OuterRad" in parsed.pref_scalar and k not in parsed.pref_scalar:
                actual_key = "Blur_OuterRad"
            elif k == "Blur_SigmaPx" and "Blur_MaxSigmaPx" in parsed.pref_scalar and k not in parsed.pref_scalar:
                actual_key = "Blur_MaxSigmaPx"
                
            w(f"PREF,{actual_key},{parsed.pref_scalar.get(actual_key, 'NaN')}\n")

        w(f"PREF,ThresholdAngleDeg,{fmt_float(threshold_deg)}\n")
        w(pref_vec3_line("SnowPlaneNormal") + "\n")
        w(pref_vec3_line("SnowPlanePoint") + "\n")

        as_is_result_keys = ["totalFlags", "totalShort", "totalLong", "totalTight", "passedFlags", "shortTurns", "longTurns", "tightTurns", "passRate", "shortRate", "longRate", "tightRate", "totalValidTime"]
        for k in as_is_result_keys: w(f"RESULT,{k},{result_val(k)}\n")

        w(f"RESULT,timeP0,{fmt_float(recomputed['timeP0'])}\n")
        w(f"RESULT,timeP1,{fmt_float(recomputed['timeP1'])}\n")
        w(f"RESULT,timeS0,{fmt_float(recomputed['timeS0'])}\n")
        w(f"RESULT,timeS1,{fmt_float(recomputed['timeS1'])}\n")
        w(f"RESULT,timeOther,{fmt_float(recomputed['timeOther'])}\n")

        w(f"RESULT,cntP0,{recomputed['cntP0']}\n")
        w(f"RESULT,cntP1,{recomputed['cntP1']}\n")

        w(f"RESULT,rateTimeP0,{fmt_float(recomputed['rateTimeP0'])}\n")
        w(f"RESULT,rateTimeP1,{fmt_float(recomputed['rateTimeP1'])}\n")
        w(f"RESULT,rateTimeS0,{fmt_float(recomputed['rateTimeS0'])}\n")
        w(f"RESULT,rateTimeS1,{fmt_float(recomputed['rateTimeS1'])}\n")
        w(f"RESULT,rateTimeOther,{fmt_float(recomputed['rateTimeOther'])}\n")

        w(f"RESULT,rateCntP0,{fmt_float(recomputed['rateCntP0'])}\n")
        w(f"RESULT,rateCntP1,{fmt_float(recomputed['rateCntP1'])}\n")

        if parsed.flag_section_lines:
            for ln in parsed.flag_section_lines: w(ln.rstrip("\n") + "\n")
        else:
            w("flagId,passedCorrect,turnDir,minDistToFlagDuringApproach\n")

        w("pId,lastRiseSecBeforePassP\n")
        pids_from_flag = []
        if len(parsed.flag_section_lines) >= 2:
            for ln in parsed.flag_section_lines[1:]:
                parts = next(csv.reader([ln]))
                if len(parts) >= 1:
                    try: pids_from_flag.append(int(parts[0]))
                    except Exception: pass
        pids = sorted(set(pids_from_flag)) if pids_from_flag else sorted(recomputed["lastRise"].keys())

        for pid in pids:
            lr = recomputed["lastRise"].get(pid, float("nan"))
            w(f"{pid},{fmt_float(lr)}\n")

        w("P1_EVENTS_FORMAT\n")
        w("pId\n")
        w("riseSecBeforePassP,duration,fallSecBeforePassP\n")
        evs_by_pid = recomputed["p1_events"]
        for pid in pids:
            w(f"{pid}\n")
            evs = evs_by_pid.get(pid, [])
            any_triplet = False
            rise_sec = dur = float("nan")
            for (typ, _p1id, sec, duration) in evs:
                if typ == P1EventType.Rise:
                    rise_sec = sec
                    dur = float("nan")
                elif typ == P1EventType.Duration:
                    dur = duration
                elif typ == P1EventType.Fall:
                    fall_sec = sec
                    w(f"{fmt_float(rise_sec)},{fmt_float(dur)},{fmt_float(fall_sec)}\n")
                    any_triplet = True
                    rise_sec = dur = float("nan")
            if not any_triplet: w("NaN\n")


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--input_dir", required=True, help="Path to the root directory containing subject folders.")
    ap.add_argument("--output_dir", required=True, help="Path to the directory where results will be saved.")
    ap.add_argument("--threshold_deg", type=float, required=True, help="Tolerance angle in degrees.")
    args = ap.parse_args()

    in_dir = Path(args.input_dir)
    out_dir = Path(args.output_dir)
    
    # Store summaries per condition: { "A": [...], "B": [...] }
    summary_data_by_condition: Dict[str, List[Dict[str, Any]]] = {}

    print(f"Scanning directory: {in_dir.resolve()} ...")
    
    for csv_path in sorted(in_dir.rglob("*.csv")):
        try:
            rel_path = csv_path.relative_to(in_dir)
            if len(rel_path.parts) < 3:
                print(f"Skipping {csv_path}: Directory structure does not match [SubID]/[Cond]/[File]")
                continue
            subject_id = rel_path.parts[0]
            condition = rel_path.parts[1]
        except ValueError:
            continue

        filename = csv_path.stem.lower()
        if filename.endswith("_before"):
            phase = "pre-test"
        elif filename.endswith("_after"):
            phase = "post-test"
        else:
            phase = "training"

        print(f"Processing: Sub={subject_id}, Cond={condition}, Phase={phase}, File={csv_path.name}")

        parsed = parse_csv_log(csv_path)

        course_width = f32(parsed.pref_scalar.get("CourseWidth", "NaN"))
        slope_angle = f32(parsed.pref_scalar.get("SlopeAngle", "NaN"))
        player_f_speed = f32(parsed.pref_scalar.get("PlayerFSpeed", "NaN"))

        if "SnowPlaneNormal" in parsed.pref_vec3:
            sx, sy, sz = parsed.pref_vec3["SnowPlaneNormal"]
            snow_n = (f32(sx), f32(sy), f32(sz))
        else:
            snow_n = rot_x((0.0, 1.0, 0.0), slope_angle if not is_nan(slope_angle) else 0.0)

        if "SnowPlanePoint" in parsed.pref_vec3:
            sx, sy, sz = parsed.pref_vec3["SnowPlanePoint"]
            snow_p = (f32(sx), f32(sy), f32(sz))
        else:
            snow_p = (0.0, 0.0, 0.0)

        if is_nan(course_width): course_width = 13.5
        if is_nan(slope_angle): slope_angle = 0.0
        if is_nan(player_f_speed) or player_f_speed <= 0.0: player_f_speed = 30.0

        total_flags_denom = 0
        if "totalFlags" in parsed.result_scalar:
            try: total_flags_denom = int(float(parsed.result_scalar["totalFlags"]))
            except Exception: total_flags_denom = 0
        if total_flags_denom <= 0 and len(parsed.flag_section_lines) >= 2:
            total_flags_denom = max(0, len(parsed.flag_section_lines) - 1)

        recomputed = recompute_from_rows(
            rows=parsed.data_rows, threshold_deg=args.threshold_deg,
            course_width=course_width, snow_n=snow_n, snow_p=snow_p,
            slope_angle_deg=slope_angle, player_f_speed=player_f_speed,
            total_flags_denom=total_flags_denom,
        )

        # 3. 出力先ディレクトリの構築と保存 (フェーズ別フォルダへ)
        phase_out_dir = out_dir / condition / phase
        phase_out_dir.mkdir(parents=True, exist_ok=True)

        out_recalc_path = phase_out_dir / f"{subject_id}_{csv_path.stem}_recalc.csv"
        write_recalc_output(out_recalc_path, parsed, args.threshold_deg, recomputed)

        if condition not in summary_data_by_condition:
            summary_data_by_condition[condition] = []
        
        summary_data_by_condition[condition].append({
            "SubjectID": subject_id,
            "Condition": condition,
            "Phase": phase,
            "FileName": csv_path.name,
            "TotalValidTime": recomputed["totalValidTime_calc"],
            "rateTimeP0": recomputed["rateTimeP0"],
            "rateTimeP1": recomputed["rateTimeP1"],
            "rateTimeS0": recomputed["rateTimeS0"],
            "rateTimeS1": recomputed["rateTimeS1"],
            "rateTimeOther": recomputed["rateTimeOther"],
            "rateCntP0": recomputed["rateCntP0"],
            "rateCntP1": recomputed["rateCntP1"]
        })

    print("\nWriting summary files per condition...")
    for cond, records in summary_data_by_condition.items():
        cond_out_dir = out_dir / cond
        summary_path = cond_out_dir / "summary.csv"
        
        if not records:
            continue
            
        fieldnames = records[0].keys()
        with summary_path.open("w", encoding="utf-8", newline="") as f:
            writer = csv.DictWriter(f, fieldnames=fieldnames)
            writer.writeheader()
            for row in records:
                writer.writerow(row)
        print(f"Created: {summary_path}")

    print("All processing completed successfully.")

if __name__ == "__main__":
    main()


# import argparse
# import csv
# import math
# from pathlib import Path
# from typing import Dict, List, Tuple, Optional, Any

# # -------------------------
# # small helpers
# # -------------------------
# def is_nan(x: float) -> bool:
#     return math.isnan(x)

# def f32(s: str) -> float:
#     s = s.strip()
#     if s == "" or s.lower() == "nan":
#         return float("nan")
#     return float(s)

# def fmt_float(x: float) -> str:
#     if is_nan(x):
#         return "NaN"
#     return f"{x:.6f}"

# def dot(a: Tuple[float, float, float], b: Tuple[float, float, float]) -> float:
#     return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]

# def sub(a: Tuple[float, float, float], b: Tuple[float, float, float]) -> Tuple[float, float, float]:
#     return (a[0] - b[0], a[1] - b[1], a[2] - b[2])

# def add(a: Tuple[float, float, float], b: Tuple[float, float, float]) -> Tuple[float, float, float]:
#     return (a[0] + b[0], a[1] + b[1], a[2] + b[2])

# def mul(a: Tuple[float, float, float], s: float) -> Tuple[float, float, float]:
#     return (a[0] * s, a[1] * s, a[2] * s)

# def norm(a: Tuple[float, float, float]) -> float:
#     return math.sqrt(dot(a, a))

# def normalize(a: Tuple[float, float, float]) -> Optional[Tuple[float, float, float]]:
#     n = norm(a)
#     if n < 1e-12:
#         return None
#     return (a[0] / n, a[1] / n, a[2] / n)

# def angle_deg(a: Tuple[float, float, float], b: Tuple[float, float, float]) -> float:
#     na = normalize(a)
#     nb = normalize(b)
#     if na is None or nb is None:
#         return float("inf")
#     d = dot(na, nb)
#     d = max(-1.0, min(1.0, d))
#     return math.degrees(math.acos(d))

# def rot_x(v: Tuple[float, float, float], deg: float) -> Tuple[float, float, float]:
#     a = math.radians(deg)
#     c = math.cos(a)
#     s = math.sin(a)
#     x, y, z = v
#     return (x, c * y - s * z, s * y + c * z)


# # -------------------------
# # parsing
# # -------------------------
# class ParsedLog:
#     def __init__(self) -> None:
#         self.header_cols: List[str] = []
#         self.data_rows: List[Dict[str, float]] = []

#         self.pref_scalar: Dict[str, str] = {}
#         self.pref_vec3: Dict[str, Tuple[str, str, str]] = {}
#         self.result_scalar: Dict[str, str] = {}
#         self.flag_section_lines: List[str] = []

# BASIC_PREF_KEYS = [
#     "CourseWidth", "SlopeAngle", "PlayerFSpeed", "PlayerHSpeed", "CameraHeight",
#     "SubjectID", "Method", "RandomSeed", "Blur_OuterAngleDeg", "Blur_SigmaPx",
# ]

# def parse_csv_log(path: Path) -> ParsedLog:
#     lines = path.read_text(encoding="utf-8", errors="ignore").splitlines()

#     header_idx = -1
#     header_line = None
#     for i, ln in enumerate(lines):
#         if ln.startswith("time,"):
#             header_idx = i
#             header_line = ln.strip()
#             break
#     if header_idx == -1 or header_line is None:
#         raise RuntimeError(f"Header line starting with 'time,' not found in {path.name}.")

#     header_line = header_line.replace("gaze_dir_zp0Id", "gaze_dir_z,p0Id")
#     header_line = header_line.replace("gaze_dir_zp0_x", "gaze_dir_z,p0_x")
#     header_cols = [c.strip() for c in header_line.split(",")]

#     out = ParsedLog()
#     out.header_cols = header_cols

#     in_flag_section = False

#     for ln in lines[header_idx + 1 :]:
#         if not ln.strip():
#             continue

#         if ln.startswith("flagId,passedCorrect,turnDir,minDistToFlagDuringApproach"):
#             in_flag_section = True
#             out.flag_section_lines.append(ln.rstrip("\n"))
#             continue

#         if in_flag_section:
#             if ln.startswith("pId,") or ln.startswith("P1_EVENTS_FORMAT") or ln.startswith("RESULT,") or ln.startswith("PREF,") or ln.startswith("START_REL_SEC,") or ln.startswith("SETTING,") or ln.startswith("time,"):
#                 in_flag_section = False
#             else:
#                 out.flag_section_lines.append(ln.rstrip("\n"))
#                 continue

#         if ln.startswith("PREF,"):
#             parts = next(csv.reader([ln]))
#             if len(parts) >= 3:
#                 key = parts[1].strip().strip('"')
#                 if key in ("SnowPlaneNormal", "SnowPlanePoint") and len(parts) >= 5:
#                     out.pref_vec3[key] = (parts[2].strip(), parts[3].strip(), parts[4].strip())
#                 else:
#                     out.pref_scalar[key] = parts[2].strip()
#             continue

#         if ln.startswith("RESULT,"):
#             parts = next(csv.reader([ln]))
#             if len(parts) >= 3:
#                 k = parts[1].strip()
#                 v = parts[2].strip()
#                 out.result_scalar[k] = v
#             continue

#         if ln.startswith("SETTING,") or ln.startswith("START_REL_SEC,") or ln.startswith("P1_EVENTS_FORMAT") or ln.startswith("pId,"):
#             continue

#         parts = next(csv.reader([ln]))
#         if len(parts) != len(header_cols):
#             continue

#         row: Dict[str, float] = {}
#         for c, p in zip(header_cols, parts):
#             try:
#                 row[c] = f32(p)
#             except Exception:
#                 row[c] = float("nan")
#         out.data_rows.append(row)

#     return out

# # -------------------------
# # reconstruction (CalcPassRate2 logic)
# # -------------------------
# class P1EventType:
#     Rise = 0
#     Duration = 1
#     Fall = 2

# def recompute_from_rows(
#     rows: List[Dict[str, float]],
#     threshold_deg: float,
#     course_width: float,
#     snow_n: Tuple[float, float, float],
#     snow_p: Tuple[float, float, float],
#     slope_angle_deg: float,
#     player_f_speed: float,
#     total_flags_denom: int,
# ) -> Dict[str, Any]:
#     fwd = normalize(rot_x((0.0, 0.0, 1.0), slope_angle_deg))
#     if fwd is None:
#         fwd = (0.0, 0.0, 1.0)

#     sn = normalize(snow_n) or (0.0, 1.0, 0.0)

#     timeP0 = timeP1 = timeS0 = timeS1 = timeOther = 0.0
#     totalValidTime = 0.0
#     cntP0 = cntP1 = cntS0 = cntS1 = 0
#     counted_p0: set[int] = set()
#     counted_p1: set[int] = set()

#     prevLookingS0 = prevLookingS1 = False

#     p1_events: Dict[int, List[Tuple[int, int, float, float]]] = {}
#     prev_pId = -1
#     prev_isLookingP1 = False
#     seg_accum = 0.0
#     seg_target_p1id = -1

#     def get_time(i: int) -> float:
#         return float(rows[i]["time"])

#     for i in range(1, len(rows)):
#         t_prev = get_time(i - 1)
#         t_cur = get_time(i)
#         dt = t_cur - t_prev
#         if not (dt > 0.0):
#             continue

#         r = rows[i]
#         gaze_o = (float(r.get("gaze_o_x", float("nan"))), float(r.get("gaze_o_y", float("nan"))), float(r.get("gaze_o_z", float("nan"))))
#         gaze_d = (float(r.get("gaze_dir_x", float("nan"))), float(r.get("gaze_dir_y", float("nan"))), float(r.get("gaze_dir_z", float("nan"))))
#         cam_p = (float(r.get("cam_x", float("nan"))), float(r.get("cam_y", float("nan"))), float(r.get("cam_z", float("nan"))))

#         p0Id = int(r.get("p0Id", -1)) if not is_nan(float(r.get("p0Id", float("nan")))) else -1
#         p1Id = int(r.get("p1Id", -1)) if not is_nan(float(r.get("p1Id", float("nan")))) else -1

#         p0Pos = (float(r.get("p0_x", float("nan"))), float(r.get("p0_y", float("nan"))), float(r.get("p0_z", float("nan"))))
#         p1Pos = (float(r.get("p1_x", float("nan"))), float(r.get("p1_y", float("nan"))), float(r.get("p1_z", float("nan"))))

#         if is_nan(gaze_o[0]) or is_nan(gaze_d[0]): continue
#         gd = normalize(gaze_d)
#         if gd is None: continue

#         if p0Id < 0 or is_nan(p0Pos[0]): continue
#         hasP1 = (p1Id >= 0) and (not is_nan(p1Pos[0]))

#         # --- 判定フラグの初期化 ---
#         is_P0 = False
#         is_P1 = False
#         is_S0 = False
#         is_S1 = False
#         is_Other = False

#         a0 = angle_deg(gd, sub(p0Pos, gaze_o))
#         p0Ok = (a0 <= threshold_deg)

#         p1Ok = False
#         if hasP1:
#             a1 = angle_deg(gd, sub(p1Pos, gaze_o))
#             p1Ok = (a1 <= threshold_deg)

#         if p0Ok or p1Ok:
#             if p0Ok and p1Ok:
#                 # 両方を見ていると判定された場合は両方のフラグを立てる
#                 is_P0 = True
#                 is_P1 = True
#             elif p1Ok:
#                 is_P1 = True
#             else:
#                 is_P0 = True
#         else:
#             denom = dot(sn, gd)
#             if abs(denom) > 1e-6:
#                 tHit = dot(sn, sub(snow_p, gaze_o)) / denom
#                 if tHit > 0.0:
#                     hit = add(gaze_o, mul(gd, tHit))
#                     inCourse = (abs(hit[0]) <= course_width)
#                     if inCourse:
#                         zPlayer = gaze_o[2]
#                         zP0 = p0Pos[2]
#                         zHit = hit[2]
#                         mn0, mx0 = min(zPlayer, zP0), max(zPlayer, zP0)
#                         if mn0 <= zHit < mx0:
#                             is_S0 = True
#                         elif hasP1:
#                             zP1 = p1Pos[2]
#                             mn1, mx1 = min(zP0, zP1), max(zP0, zP1)
#                             if mn1 <= zHit < mx1:
#                                 is_S1 = True
            
#             # S0にもS1にも該当しなかった場合
#             if not is_S0 and not is_S1:
#                 is_Other = True

#         # --- 時間（dt）の集計・配分 ---
#         totalValidTime += dt
#         p1_dt = 0.0

#         if is_P0 and is_P1:
#             # 両方見ている場合は時間を半分ずつ配分
#             timeP0 += dt / 2.0
#             timeP1 += dt / 2.0
#             p1_dt = dt / 2.0
#         elif is_P1:
#             timeP1 += dt
#             p1_dt = dt
#         elif is_P0:
#             timeP0 += dt
#         elif is_S0:
#             timeS0 += dt
#         elif is_S1:
#             timeS1 += dt
#         else:
#             timeOther += dt

#         # --- イベントカウント・状態管理 ---
#         if is_S0 and not prevLookingS0: cntS0 += 1
#         if is_S1 and not prevLookingS1: cntS1 += 1
#         prevLookingS0 = is_S0
#         prevLookingS1 = is_S1

#         if is_P0:
#             if p0Id not in counted_p0:
#                 cntP0 += 1
#                 counted_p0.add(p0Id)
#         if is_P1 and hasP1:
#             if p1Id not in counted_p1:
#                 cntP1 += 1
#                 counted_p1.add(p1Id)

#         if p0Id not in p1_events: p1_events[p0Id] = []
#         secBeforePassP = 0.0
#         if player_f_speed > 1e-6:
#             alongP0 = dot(sub(p0Pos, gaze_o), fwd)
#             secBeforePassP = (alongP0 / player_f_speed) if (alongP0 > 0.0) else 0.0

#         isNowLookingP1 = is_P1

#         if prev_pId != -1 and prev_pId != p0Id:
#             if prev_isLookingP1:
#                 p1_events[prev_pId].append((P1EventType.Duration, seg_target_p1id, float("nan"), seg_accum))
#                 p1_events[prev_pId].append((P1EventType.Fall, seg_target_p1id, 0.0, 0.0))
#             prev_isLookingP1 = False
#             seg_accum = 0.0
#             seg_target_p1id = -1

#         if isNowLookingP1 and not prev_isLookingP1:
#             seg_accum = 0.0
#             seg_target_p1id = p1Id
#             p1_events[p0Id].append((P1EventType.Rise, p1Id, secBeforePassP, 0.0))

#         if isNowLookingP1:
#             # 注視時間（duration）も配分された時間（p1_dt）を加算する
#             seg_accum += p1_dt

#         if (not isNowLookingP1) and prev_isLookingP1:
#             p1_events[p0Id].append((P1EventType.Duration, seg_target_p1id, float("nan"), seg_accum))
#             p1_events[p0Id].append((P1EventType.Fall, seg_target_p1id, secBeforePassP, 0.0))
#             seg_accum = 0.0
#             seg_target_p1id = -1

#         prev_pId = p0Id
#         prev_isLookingP1 = isNowLookingP1

#     # ファイル末端のイベントクローズ
#     if prev_pId != -1 and prev_isLookingP1:
#         p1_events[prev_pId].append((P1EventType.Duration, seg_target_p1id, float("nan"), seg_accum))
#         p1_events[prev_pId].append((P1EventType.Fall, seg_target_p1id, 0.0, 0.0))

#     denom_t = totalValidTime if totalValidTime > 0.0 else float("nan")
#     rateTimeP0 = (timeP0 / denom_t) if not is_nan(denom_t) else float("nan")
#     rateTimeP1 = (timeP1 / denom_t) if not is_nan(denom_t) else float("nan")
#     rateTimeS0 = (timeS0 / denom_t) if not is_nan(denom_t) else float("nan")
#     rateTimeS1 = (timeS1 / denom_t) if not is_nan(denom_t) else float("nan")
#     rateTimeOther = (timeOther / denom_t) if not is_nan(denom_t) else float("nan")

#     denom_cnt = float(total_flags_denom) if total_flags_denom > 0 else float("nan")
#     rateCntP0 = (cntP0 / denom_cnt) if not is_nan(denom_cnt) else float("nan")
#     rateCntP1 = (cntP1 / denom_cnt) if not is_nan(denom_cnt) else float("nan")

#     lastRise: Dict[int, float] = {}
#     for pid, evs in p1_events.items():
#         lr = float("nan")
#         for (typ, _p1id, sec, _dur) in evs:
#             if typ == P1EventType.Rise: lr = sec
#         lastRise[pid] = lr

#     return {
#         "totalValidTime_calc": totalValidTime,
#         "timeP0": timeP0, "timeP1": timeP1, "timeS0": timeS0, "timeS1": timeS1, "timeOther": timeOther,
#         "cntP0": cntP0, "cntP1": cntP1, "cntS0": cntS0, "cntS1": cntS1,
#         "rateTimeP0": rateTimeP0, "rateTimeP1": rateTimeP1, "rateTimeS0": rateTimeS0, "rateTimeS1": rateTimeS1, "rateTimeOther": rateTimeOther,
#         "rateCntP0": rateCntP0, "rateCntP1": rateCntP1,
#         "p1_events": p1_events, "lastRise": lastRise,
#     }


# def write_recalc_output(
#     out_path: Path, parsed: ParsedLog, threshold_deg: float, recomputed: Dict[str, Any]
# ) -> None:
#     def result_val(key: str) -> str: return parsed.result_scalar.get(key, "NaN")
#     def pref_vec3_line(key: str) -> str:
#         if key in parsed.pref_vec3:
#             x, y, z = parsed.pref_vec3[key]
#             return f'PREF,"{key}",{x},{y},{z}'
#         return f'PREF,"{key}",NaN,NaN,NaN'

#     with out_path.open("w", encoding="utf-8", newline="") as f:
#         w = f.write

#         for k in BASIC_PREF_KEYS:
#             actual_key = k
#             if k == "Blur_OuterAngleDeg" and "Blur_OuterRad" in parsed.pref_scalar and k not in parsed.pref_scalar:
#                 actual_key = "Blur_OuterRad"
#             elif k == "Blur_SigmaPx" and "Blur_MaxSigmaPx" in parsed.pref_scalar and k not in parsed.pref_scalar:
#                 actual_key = "Blur_MaxSigmaPx"
                
#             w(f"PREF,{actual_key},{parsed.pref_scalar.get(actual_key, 'NaN')}\n")

#         w(f"PREF,ThresholdAngleDeg,{fmt_float(threshold_deg)}\n")
#         w(pref_vec3_line("SnowPlaneNormal") + "\n")
#         w(pref_vec3_line("SnowPlanePoint") + "\n")

#         as_is_result_keys = ["totalFlags", "totalShort", "totalLong", "totalTight", "passedFlags", "shortTurns", "longTurns", "tightTurns", "passRate", "shortRate", "longRate", "tightRate", "totalValidTime"]
#         for k in as_is_result_keys: w(f"RESULT,{k},{result_val(k)}\n")

#         w(f"RESULT,timeP0,{fmt_float(recomputed['timeP0'])}\n")
#         w(f"RESULT,timeP1,{fmt_float(recomputed['timeP1'])}\n")
#         w(f"RESULT,timeS0,{fmt_float(recomputed['timeS0'])}\n")
#         w(f"RESULT,timeS1,{fmt_float(recomputed['timeS1'])}\n")
#         w(f"RESULT,timeOther,{fmt_float(recomputed['timeOther'])}\n")

#         w(f"RESULT,cntP0,{recomputed['cntP0']}\n")
#         w(f"RESULT,cntP1,{recomputed['cntP1']}\n")

#         w(f"RESULT,rateTimeP0,{fmt_float(recomputed['rateTimeP0'])}\n")
#         w(f"RESULT,rateTimeP1,{fmt_float(recomputed['rateTimeP1'])}\n")
#         w(f"RESULT,rateTimeS0,{fmt_float(recomputed['rateTimeS0'])}\n")
#         w(f"RESULT,rateTimeS1,{fmt_float(recomputed['rateTimeS1'])}\n")
#         w(f"RESULT,rateTimeOther,{fmt_float(recomputed['rateTimeOther'])}\n")

#         w(f"RESULT,rateCntP0,{fmt_float(recomputed['rateCntP0'])}\n")
#         w(f"RESULT,rateCntP1,{fmt_float(recomputed['rateCntP1'])}\n")

#         if parsed.flag_section_lines:
#             for ln in parsed.flag_section_lines: w(ln.rstrip("\n") + "\n")
#         else:
#             w("flagId,passedCorrect,turnDir,minDistToFlagDuringApproach\n")

#         w("pId,lastRiseSecBeforePassP\n")
#         pids_from_flag = []
#         if len(parsed.flag_section_lines) >= 2:
#             for ln in parsed.flag_section_lines[1:]:
#                 parts = next(csv.reader([ln]))
#                 if len(parts) >= 1:
#                     try: pids_from_flag.append(int(parts[0]))
#                     except Exception: pass
#         pids = sorted(set(pids_from_flag)) if pids_from_flag else sorted(recomputed["lastRise"].keys())

#         for pid in pids:
#             lr = recomputed["lastRise"].get(pid, float("nan"))
#             w(f"{pid},{fmt_float(lr)}\n")

#         w("P1_EVENTS_FORMAT\n")
#         w("pId\n")
#         w("riseSecBeforePassP,duration,fallSecBeforePassP\n")
#         evs_by_pid = recomputed["p1_events"]
#         for pid in pids:
#             w(f"{pid}\n")
#             evs = evs_by_pid.get(pid, [])
#             any_triplet = False
#             rise_sec = dur = float("nan")
#             for (typ, _p1id, sec, duration) in evs:
#                 if typ == P1EventType.Rise:
#                     rise_sec = sec
#                     dur = float("nan")
#                 elif typ == P1EventType.Duration:
#                     dur = duration
#                 elif typ == P1EventType.Fall:
#                     fall_sec = sec
#                     w(f"{fmt_float(rise_sec)},{fmt_float(dur)},{fmt_float(fall_sec)}\n")
#                     any_triplet = True
#                     rise_sec = dur = float("nan")
#             if not any_triplet: w("NaN\n")


# def main() -> None:
#     ap = argparse.ArgumentParser()
#     ap.add_argument("--input_dir", required=True, help="Path to the root directory containing subject folders.")
#     ap.add_argument("--output_dir", required=True, help="Path to the directory where results will be saved.")
#     ap.add_argument("--threshold_deg", type=float, required=True, help="Tolerance angle in degrees.")
#     args = ap.parse_args()

#     in_dir = Path(args.input_dir)
#     out_dir = Path(args.output_dir)
    
#     # Store summaries per condition: { "A": [...], "B": [...] }
#     summary_data_by_condition: Dict[str, List[Dict[str, Any]]] = {}

#     print(f"Scanning directory: {in_dir.resolve()} ...")
    
#     for csv_path in sorted(in_dir.rglob("*.csv")):
#         try:
#             rel_path = csv_path.relative_to(in_dir)
#             if len(rel_path.parts) < 3:
#                 print(f"Skipping {csv_path}: Directory structure does not match [SubID]/[Cond]/[File]")
#                 continue
#             subject_id = rel_path.parts[0]
#             condition = rel_path.parts[1]
#         except ValueError:
#             continue

#         filename = csv_path.stem.lower()
#         if filename.endswith("_before"):
#             phase = "pre-test"
#         elif filename.endswith("_after"):
#             phase = "post-test"
#         else:
#             phase = "training"

#         print(f"Processing: Sub={subject_id}, Cond={condition}, Phase={phase}, File={csv_path.name}")

#         parsed = parse_csv_log(csv_path)

#         course_width = f32(parsed.pref_scalar.get("CourseWidth", "NaN"))
#         slope_angle = f32(parsed.pref_scalar.get("SlopeAngle", "NaN"))
#         player_f_speed = f32(parsed.pref_scalar.get("PlayerFSpeed", "NaN"))

#         if "SnowPlaneNormal" in parsed.pref_vec3:
#             sx, sy, sz = parsed.pref_vec3["SnowPlaneNormal"]
#             snow_n = (f32(sx), f32(sy), f32(sz))
#         else:
#             snow_n = rot_x((0.0, 1.0, 0.0), slope_angle if not is_nan(slope_angle) else 0.0)

#         if "SnowPlanePoint" in parsed.pref_vec3:
#             sx, sy, sz = parsed.pref_vec3["SnowPlanePoint"]
#             snow_p = (f32(sx), f32(sy), f32(sz))
#         else:
#             snow_p = (0.0, 0.0, 0.0)

#         if is_nan(course_width): course_width = 13.5
#         if is_nan(slope_angle): slope_angle = 0.0
#         if is_nan(player_f_speed) or player_f_speed <= 0.0: player_f_speed = 30.0

#         total_flags_denom = 0
#         if "totalFlags" in parsed.result_scalar:
#             try: total_flags_denom = int(float(parsed.result_scalar["totalFlags"]))
#             except Exception: total_flags_denom = 0
#         if total_flags_denom <= 0 and len(parsed.flag_section_lines) >= 2:
#             total_flags_denom = max(0, len(parsed.flag_section_lines) - 1)

#         recomputed = recompute_from_rows(
#             rows=parsed.data_rows, threshold_deg=args.threshold_deg,
#             course_width=course_width, snow_n=snow_n, snow_p=snow_p,
#             slope_angle_deg=slope_angle, player_f_speed=player_f_speed,
#             total_flags_denom=total_flags_denom,
#         )

#         # 3. 出力先ディレクトリの構築と保存 (フェーズ別フォルダへ)
#         phase_out_dir = out_dir / condition / phase
#         phase_out_dir.mkdir(parents=True, exist_ok=True)

#         out_recalc_path = phase_out_dir / f"{subject_id}_{csv_path.stem}_recalc.csv"
#         write_recalc_output(out_recalc_path, parsed, args.threshold_deg, recomputed)

#         if condition not in summary_data_by_condition:
#             summary_data_by_condition[condition] = []
        
#         summary_data_by_condition[condition].append({
#             "SubjectID": subject_id,
#             "Condition": condition,
#             "Phase": phase,
#             "FileName": csv_path.name,
#             "TotalValidTime": recomputed["totalValidTime_calc"],
#             "rateTimeP0": recomputed["rateTimeP0"],
#             "rateTimeP1": recomputed["rateTimeP1"],
#             "rateTimeS0": recomputed["rateTimeS0"],
#             "rateTimeS1": recomputed["rateTimeS1"],
#             "rateTimeOther": recomputed["rateTimeOther"],
#             "rateCntP0": recomputed["rateCntP0"],
#             "rateCntP1": recomputed["rateCntP1"]
#         })

#     print("\nWriting summary files per condition...")
#     for cond, records in summary_data_by_condition.items():
#         cond_out_dir = out_dir / cond
#         summary_path = cond_out_dir / "summary.csv"
        
#         if not records:
#             continue
            
#         fieldnames = records[0].keys()
#         with summary_path.open("w", encoding="utf-8", newline="") as f:
#             writer = csv.DictWriter(f, fieldnames=fieldnames)
#             writer.writeheader()
#             for row in records:
#                 writer.writerow(row)
#         print(f"Created: {summary_path}")

#     print("All processing completed successfully.")

# if __name__ == "__main__":
#     main()