#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Problem 8 – Ridge bias/variance trade-off
"""

import numpy as np
import matplotlib.pyplot as plt

# ---------- 1. 実験設定 ----------------------------------------------------
d        = 20
lambdas  = np.array([0.0, 0.01, 0.1, 1.0, 10.0, 100.0])
sigma    = 1.0
trials   = 100
ns       = [10, 50, 100]

def ridge_estimator(X, y, lam):
    """リッジ回帰または最小二乗法でパラメータを推定する"""
    if lam == 0.0:
        # pinvよりもlstsqの方が推奨される
        return np.linalg.lstsq(X, y, rcond=None)[0]
    
    # λ > 0 の場合は変更なし
    A = X.T @ X + lam * np.eye(X.shape[1])
    return np.linalg.solve(A, X.T @ y)

# 再現性のための乱数生成器
rng = np.random.default_rng(0)

# ---------- メインループ ----------------------------------------------------
for n in ns:
    # 真のパラメータw*はnごとに固定
    w_star = rng.normal(size=d)
    
    # 結果を格納する配列: (λの数, 試行回数, パラメータ次元)
    Ws = np.empty((lambdas.size, trials, d))

    # --- モンテカルロシミュレーション ---
    for t in range(trials):
        X     = rng.normal(size=(n, d))
        eps   = rng.normal(scale=sigma, size=n)
        y     = X @ w_star + eps
        
        for j, lam in enumerate(lambdas):
            Ws[j, t] = ridge_estimator(X, y, lam)

    # --- バイアスとバリアンスの計算 ---
    W_mean = Ws.mean(axis=1)
    bias2  = np.sum((W_mean - w_star)**2, axis=1)
    var    = np.mean(np.sum((Ws - W_mean[:, None, :])**2, axis=2), axis=1)
    total  = bias2 + var

    # ---------- 2. プロット（λ>0 のみ） -------------------------------
    mask = lambdas > 0
    plt.figure(figsize=(6, 4))
    plt.loglog(lambdas[mask], bias2[mask], 'o-', label=r'Bias$^2$')
    plt.loglog(lambdas[mask], var[mask],   's-', label='Variance')
    plt.loglog(lambdas[mask], total[mask], '^-', label='Total')
    plt.xlabel(r'$\lambda$ (log scale)')
    plt.ylabel('Value')
    plt.title(f'Bias–Variance trade-off (n = {n})')
    
    # ★★★ ここを修正 ★★★
    # 横軸の表示範囲を少し広げて見やすくする
    plt.xlim(8e-3, 1.2e2) # 0.008 から 120 まで
    
    plt.grid(True, which='both', ls='--', alpha=0.6)
    plt.legend()
    plt.tight_layout()
    plt.savefig(f'prob8_bias_variance_n{n}_final.png', dpi=300)

print("Finished — plots saved as prob8_bias_variance_n*_final.png")