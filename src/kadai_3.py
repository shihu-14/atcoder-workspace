#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Problem 3 (fix): Dual SVM – proper step size from Lipschitz constant
"""

import numpy as np
import matplotlib.pyplot as plt

# ---------- 1. Dataset -----------------------------------------------------
np.random.seed(42)
n = 40
omega = np.random.randn()
noise = 0.8 * np.random.randn(n)

X = np.random.randn(n, 2)
y = 2 * ((omega * X[:, 0] + X[:, 1] + noise) > 0).astype(int) - 1
d = X.shape[1]

# ---------- 2. Hyper-parameters -------------------------------------------
lam       = 1.0
max_iter  = 5000

# step for dual: η < 2λ / λ_max(K)
K = (y[:, None] * y[None, :]) * (X @ X.T)
lmax = np.linalg.eigvalsh(K).max()           # 最大固有値 (正値)
eta_dual = 0.9 * (2 * lam / lmax)            # 安全に 90 % を採用

# step for primal subgradient (diminishing)
def eta_primal(t):          # 1 / (λ t) スケジュール
    return 1.0 / (lam * (t + 1))

# ---------- 3-A. Dual : Projected Gradient --------------------------------
alpha = np.zeros(n)
dual_hist, prim_hist, gap_hist = [], [], []

for it in range(max_iter):
    grad = (1 / (2 * lam)) * (K @ alpha) - 1.0      # −∇D
    alpha -= eta_dual * grad
    np.clip(alpha, 0.0, 1.0, out=alpha)             # projection

    # Dual value
    dual_val = -0.25 / lam * alpha @ (K @ alpha) + alpha.sum()

    # Primal value via KKT
    w = (alpha * y) @ X / (2 * lam)
    margins = 1 - y * (X @ w)
    prim_val = np.maximum(0, margins).sum() + 0.5 * lam * np.dot(w, w)

    dual_hist.append(dual_val)
    prim_hist.append(prim_val)
    gap_hist.append(prim_val - dual_val)

# ---------- 3-B. Primal : Subgradient -------------------------------------
w_sub = np.zeros(d)
prim_sub_hist = []

for t in range(max_iter):
    margins = 1 - y * (X @ w_sub)
    subgrad = -(margins > 0).astype(float) @ (y[:, None] * X) + lam * w_sub
    w_sub -= eta_primal(t) * subgrad
    f_val = np.maximum(0, 1 - y * (X @ w_sub)).sum() + 0.5 * lam * w_sub @ w_sub
    prim_sub_hist.append(f_val)

# ---------- 4. Plotting ----------------------------------------------------
fig, (ax1, ax2) = plt.subplots(
    2, 1, figsize=(6, 6), sharex=True, gridspec_kw={"height_ratios": [2, 1]}
)

ax1.plot(prim_hist,  label="Primal $P(w)$")
ax1.plot(dual_hist,  label="Dual $D(\\alpha)$")
ax1.set_ylabel("Objective value")
ax1.set_title("Primal vs Dual")
ax1.grid(True); ax1.legend()
ax1.set_ylim([0, 40])
ax1.set_xlim([0, 50])

gap = np.maximum(gap_hist, 1e-14)           # avoid log(0)
ax2.semilogy(gap, color="tab:red", label="Gap $P-D$")
ax2.set_xlabel("Iteration")
ax2.set_ylabel("Gap")
ax2.grid(True); ax2.legend(loc="upper right")

fig.tight_layout()
fig.savefig("prob3_dual_primal_gap.png", dpi=300)

# Optional: primal subgradient curve
plt.figure(figsize=(6,4))
plt.plot(prim_sub_hist, label="Primal subgradient")
plt.xlabel("Iteration"); plt.ylabel("Primal objective")
plt.grid(True); plt.legend(); plt.tight_layout()
plt.xlim([-1, 50])
plt.savefig("prob3_primal_subgrad.png", dpi=300)

print(f"Final duality gap  = {gap_hist[-1]:.3e}")
print(f"Step η_dual used   = {eta_dual:.3e}  (theory bound {(2*lam/lmax):.3e})")
