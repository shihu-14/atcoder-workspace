import numpy as np
import matplotlib.pyplot as plt

# --- Dataset (Toy Dataset IV) ------
# 

# n = 200;
# x = 3 * (rand(n, 4) - 0.5);
# y = (2 * x(:, 1) - 1 * x(:,2) + 0.5 + 0.5 * randn(n, 1)) > 0;
# y = 2 * y -1;
# 
# -------------------------------------------
np.random.seed(0)          # reproducibility
n = 200
X_raw = 3 * (np.random.rand(n, 4) - 0.5)           # shape (n,4)

# generate labels y ∈ {-1,1}
y = 2 * ((2 * X_raw[:, 0] - 1 * X_raw[:, 1] + 0.5 +
          0.5 * np.random.randn(n)) > 0).astype(int) - 1

# append bias term (last coordinate = 1)
X = np.hstack([X_raw, np.ones((n, 1))])            # shape (n,5)
d = X.shape[1]

# --- Hyper‑parameters ---------------------------------------------------------
lam = 1.0                # L2 regularisation strength
max_it_gd = 1000         # iterations for gradient descent
max_it_newton = 50       # safety cap for Newton
tol = 1e-12              # convergence threshold

# --- Helper functions ---------------------------------------------------------
def logistic_loss(w):
    """J(w): regularised logistic loss (y in {-1,1})."""
    z = y * (X @ w)                         # shape (n,)
    return np.sum(np.log1p(np.exp(-z))) + 0.5 * lam * np.dot(w, w)

def grad_J(w):
    """∇J(w)."""
    z = y * (X @ w)
    sigma = 1 / (1 + np.exp(z))            # sigmoid(-z)
    g = -(y * sigma) @ X + lam * w
    return g

def hess_J(w):
    """Hessian of J(w)."""
    z = y * (X @ w)
    sigma = 1 / (1 + np.exp(z))
    D = sigma * (1 - sigma)                # shape (n,)
    H = X.T @ (D[:, None] * X) + lam * np.eye(d)
    return H

# --- Optimal solution (w_hat) via Newton until convergence --------------------
w_hat = np.zeros(d)
for _ in range(100):                       # ample iterations for convergence
    g = grad_J(w_hat)
    if np.linalg.norm(g) < tol:            # gradient vanishes -> optimum found
        break
    H = hess_J(w_hat)
    delta = np.linalg.solve(H, g)
    w_hat -= delta

J_opt = logistic_loss(w_hat)

# --- Batch Gradient Descent (BGD) --------------------------------------------
# Lipschitz constant upper bound for step size: 0.25 * ||X||^2 + λ
L = 0.25 * (np.linalg.norm(X, 2)**2) + lam
eta = 1.0 / L

w_gd = np.zeros(d)
hist_gd = []

for _ in range(max_it_gd):
    hist_gd.append(logistic_loss(w_gd) - J_opt)
    g = grad_J(w_gd)
    w_gd -= eta * g

# --- Newton's Method ----------------------------------------------------------
w_nt = np.zeros(d)
hist_nt = []

for _ in range(max_it_newton):
    hist_nt.append(logistic_loss(w_nt) - J_opt)
    g = grad_J(w_nt)
    if np.linalg.norm(g) < tol:
        break
    H = hess_J(w_nt)
    delta = np.linalg.solve(H, g)
    w_nt -= delta

plt.figure()

# 200 step ぶんだけ表示（Gradient Descent が 200 回で収束と分かるよう拡大）
plt.semilogy(hist_gd[:200], label="Batch steepest gradient")
plt.semilogy(hist_nt,       label="Newton")

plt.xlabel("Iteration")
plt.ylabel(r"$|J(w^{(t)}) - J(\hat{w})|$")

plt.xlim(0, 100)
plt.xticks(np.arange(0, 101, 10))   # 0, 20, 40, …, 200

plt.title("Convergence comparison (Dataset IV)")
plt.legend()
plt.grid(True)
plt.show()