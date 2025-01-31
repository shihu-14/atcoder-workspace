import numpy as np
import matplotlib.pyplot as plt

# 最適化する関数を定義
def f1(x):
    x1, x2 = x
    return x1**2 - x1*x2 + x2**2 - x1 - x2 + 1

def f2(x):
    x1, x2 = x
    return x1**2 + x1*x2 + x2**2 + 2*x1 + 4*x2 + 4

# 勾配ベクトルを計算
def grad(x, w):
    x1, x2 = x

    grad_f1 = np.array([
        2*x1 - x2 - 1,
        -x1 + 2*x2 - 1
    ])
    grad_f2 = np.array([
        2*x1 + x2 + 2,
        x1 + 2*x2 + 4
    ])
    return w*grad_f1 + (1.0-w)*grad_f2

# ヘッセ行列を計算
def hess(w):
    return np.array([
        [2.0, 1.0-2.0*w],
        [1.0-2.0*w, 2.0]
    ])

# ニュートン法
def newton_method(w, x_init, tol=1e-12, max_iter=100): # tol: 許容誤差, max_iter: 最大反復回数
    x = x_init.copy()
    # 探索方向d (= −∇^2f(xk)^(−1)∇f(xk))を決定するためのヘッセ行列の逆行列を求める。
    H = hess(w)
    H_inv = np.linalg.inv(H)

    for _ in range(max_iter):
        g = grad(x, w) # 勾配
        if np.linalg.norm(g) < tol: # 終了条件 (||∇f(xk)||^2 = 0)
            break
        x = x - H_inv@g # 解の更新を行う (xk+1 = xk + dk)

    return x

# main
w_list = np.linspace(0.0, 1.0, 101) # f1に対する重みリスト
solve_x = [] # 近似解を保持するためのリスト
solve_f = [] # 目的関数値を保持するためのリスト

# 初期値をx=[0,0]とし，前の解を次の初期値とする。
cur_x = np.array([0.0, 0.0])

# 線形荷重和法の関数f1に対する重みを変化させ、ニュートン法で最適化
for w in w_list:
    print(w)
    # ニュートン法で最適化
    x_star = newton_method(w, cur_x, tol=1e-12, max_iter=100)
    cur_x = x_star
    # 近似解を記録する
    solve_x.append(x_star)
    solve_f.append([f1(x_star), f2(x_star)])

solve_x = np.array(solve_x)
solve_f = np.array(solve_f)

# プロットのための記述
plt.figure(figsize=(12, 5))

# x1-x2 平面
plt.subplot(1, 2, 1)
plt.scatter(solve_x[:, 0], solve_x[:, 1], c=w_list, cmap='viridis', s=30)
plt.colorbar(label='w')
plt.xlabel('x1')
plt.ylabel('x2')
plt.title('x1 - x2 plane')

# f1-f2 平面
plt.subplot(1, 2, 2)
plt.scatter(solve_f[:, 0], solve_f[:, 1], c=w_list, cmap='viridis', s=30)
plt.colorbar(label='w')
plt.xlabel('f1')
plt.ylabel('f2')
plt.title('f1 - f2 plane')

plt.tight_layout()
plt.show()
