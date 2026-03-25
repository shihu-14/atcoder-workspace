# process.py
import os, subprocess, sys

exe = "./src/a.out"
input_dir = "./in2"
output_dir = "./out"
os.makedirs(output_dir, exist_ok=True)

for fn in sorted(os.listdir(input_dir)):
    if not fn.endswith(".txt"):
        continue
    inp = os.path.join(input_dir, fn)
    outp = os.path.join(output_dir, fn)
    print(f"処理中: {fn} -> {fn}", flush=True)

    with open(inp) as fin, open(outp, "w") as fout:
        # stdout はファイルへ、stderr はパイプで受けてコンソールへ流す
        p = subprocess.Popen(
            [exe], stdin=fin, stdout=fout, stderr=subprocess.PIPE, text=True, bufsize=1
        )
        for line in p.stderr:          # cerr を逐次コンソールへ
            sys.stderr.write(line)
        rc = p.wait()

    if rc != 0:
        print(f"[{fn}] returncode={rc}", file=sys.stderr)
