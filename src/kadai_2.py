#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Problem 2  : Lasso (平方損失 + L1) を近接勾配法で解く
- Task 1: A = [[3,0.5],[0.5,1]],   mu = (1,2)
- Task 2: A = [[300,0.5],[0.5,10]], mu = (1,2)
  いずれも λ > 0
"""

import numpy as np
import matplotlib.pyplot as plt

# ---------- 近接演算（ソフト閾値） ----------------------------------------
def prox_l1(v: np.ndarray, thresh: float) -> np.ndarray:
    """soft-thresholding: prox_{λ||·||_1}(v)"""
    return np.sign(v) * np.maximum(np.abs(v) - thresh, 0.0)


# ---------- 目的関数 -------------------------------------------------------
def obj(A, mu, lam, w):
    diff = w - mu
    return 0.5 * diff @ (A @ diff) + lam * np.linalg.norm(w, 1)


# ---------- Proximal Gradient --------------------------------------------
def pg_solver(A, mu, lam, w0, max_iter=10_000, tol=1e-10):
    """標準 PG（一定ステップ幅）"""
    # Lipschitz 定数 L = 最大固有値
    L = np.linalg.eigvalsh(A).max()
    eta = 1.0 / L

    w = w0.copy()
    history = []
    for _ in range(max_iter):
        grad = A @ (w - mu)                    # ∇(quadratic part)
        w_next = prox_l1(w - eta * grad, eta * lam)
        history.append(obj(A, mu, lam, w_next))
        if np.linalg.norm(w_next - w) < tol:
            break
        w = w_next
    return w, np.array(history)


# ---------- PG + AdaGrad ---------------------------------------------------
def pg_adagrad(A, mu, lam, w0, max_iter=10_000, eps=1e-8, tol=1e-10):
    """近接勾配 + AdaGrad"""
    w = w0.copy()
    g_sq_sum = np.zeros_like(w)      # 累積二乗勾配
    history = []
    for _ in range(max_iter):
        grad = A @ (w - mu)
        g_sq_sum += grad**2
        step = 1.0 / (np.sqrt(g_sq_sum) + eps)   # 要素ごと学習率
        w_next = prox_l1(w - step * grad, step * lam)
        history.append(obj(A, mu, lam, w_next))
        if np.linalg.norm(w_next - w) < tol:
            break
        w = w_next
    return w, np.array(history)


# ============================ Task 1 ======================================
A1 = np.array([[3, 0.5],
               [0.5, 1.0]])
mu = np.array([1.0, 2.0])
lam_demo = 0.1                       # 誤差曲線用の λ
w0 = np.zeros(2)

w_star, hist = pg_solver(A1, mu, lam_demo, w0)
J_star = obj(A1, mu, lam_demo, w_star)


# --- 誤差曲線を半対数で描画 -------------------------------------------
err = np.abs(hist - J_star)

plt.figure(figsize=(6,4))
plt.semilogy(err, label=f"PG  (λ={lam_demo})")

plt.xlabel("Iteration")
plt.ylabel(r"$|J(w^{(t)})-J(\hat w)|$")

# ------------- 軸範囲を指定 -------------
plt.xlim(0, 50)          # 横軸 0–50
plt.ylim(1e-16, err[0])  # 縦軸 10^{-16} 〜 初期誤差
# ----------------------------------------

plt.title("PG convergence")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("task1_convergence.png", dpi=300)
# # 誤差曲線を半対数で描画 -------------------------------------------
# err = np.abs(hist - J_star)
# plt.figure(figsize=(6,4))
# plt.semilogy(err, label=f"PG  (λ={lam_demo})")
# plt.xlabel("Iteration")
# plt.ylabel(r"$|J(w^{(t)})-J(\hat w)|$")
# plt.title("PG convergence (Task 1)")
# plt.grid(True)
# plt.legend()
# plt.tight_layout()
# plt.savefig("task1_convergence.png", dpi=300)

# λ = 0.01 ... 10.00 で解をプロット -------------------------------
lam_values = np.round(np.arange(0.01, 10.01, 0.01), 2)
w_lam = np.zeros((len(lam_values), 2))

for k, lam in enumerate(lam_values):
    w_lam[k], _ = pg_solver(A1, mu, lam, w0)

plt.figure(figsize=(5,5))
plt.scatter(w_lam[:,0], w_lam[:,1], c=lam_values, s=8, cmap='viridis')
plt.colorbar(label=r"$\lambda$")
plt.xlabel(r"$\hat w_1$")
plt.ylabel(r"$\hat w_2$")
plt.title(r"Solution path")
plt.axis('equal')
plt.tight_layout()
plt.savefig("task1_solution_path.png", dpi=300)


# ============================ Task 2 ======================================
A2 = np.array([[300, 0.5],
               [0.5, 10.0]])
lam2 = 0.1
w_star2, hist_pg = pg_solver(A2, mu, lam2, w0)
J_star2 = obj(A2, mu, lam2, w_star2)

_, hist_ada = pg_adagrad(A2, mu, lam2, w0)

err_pg  = np.abs(hist_pg  - J_star2)
err_ada = np.abs(hist_ada - J_star2)

plt.figure(figsize=(6,4))
plt.semilogy(err_pg,  label="PG")
plt.semilogy(err_ada, label="AdaGrad-PG")
plt.xlabel("Iteration")
plt.ylabel(r"$|J(w^{(t)})-J(\hat w)|$")
plt.title("PG vs AdaGrad-PG")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("task2_convergence.png", dpi=300)

print("Task 1: PG converged in", len(err), "iterations")
print("Task 2: PG  iterations =", len(err_pg),
      ", AdaGrad iterations =", len(err_ada))
