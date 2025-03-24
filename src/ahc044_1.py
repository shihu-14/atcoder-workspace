import sys
import math
import random

def computeSteadyState(N, A, B, maxIter=300, eps=1e-14):
    """
    (a_i,b_i) に基づき, 各頂点から a_i or b_i へ1/2ずつの確率で遷移する
    マルコフ連鎖の定常分布 p をパワーイテレーションで求める。
    
    N: 社員数
    A, B: サイズNのリスト, A[i], B[i] は i番社員の (a_i, b_i)
    maxIter: 反復の上限
    eps: 収束判定閾値
    戻り値: サイズNのリスト p, sum(p)=1程度になる想定
    """
    # M[i][j] = 1/2 if j in {A[i], B[i]}, ただし A[i]==B[i]なら M[i][j]=1.0
    M = [[0.0]*N for _ in range(N)]
    for i in range(N):
        if A[i] == B[i]:
            # 完全自己ループ
            M[i][A[i]] = 1.0
        else:
            M[i][A[i]] += 0.5
            M[i][B[i]] += 0.5

    # 初期p: 一様分布
    p = [1.0/N]*N

    for _ in range(maxIter):
        newp = [0.0]*N
        # p * M
        for i in range(N):
            pi = p[i]
            for j in range(N):
                newp[j] += pi * M[i][j]
        # 収束判定
        diff = 0.0
        for j in range(N):
            diff += abs(newp[j] - p[j])
        p = newp
        if diff < eps:
            break
    return p

def simulate_or_compute_dist(N, L, T, A, B):
    """
    定常分布を求め、t_i ≈ p_i * L とみなす
    戻り値: t_approx (サイズN)
    """
    p = computeSteadyState(N, A, B)
    t_approx = [p[i]*L for i in range(N)]
    return t_approx

def evaluate(N, L, T, A, B):
    """
    t_i ≈ p_i*L で割り当て回数を推定し、score = sum(|t_i - T_i|)
    """
    t_approx = simulate_or_compute_dist(N, L, T, A, B)
    score = 0.0
    for i in range(N):
        score += abs(t_approx[i] - T[i])
    return score

def crossover_and_mutate(par1, par2, N, mutation_rate=0.02):
    """
    2つの親(par1, par2) = (A1,B1), (A2,B2) から子を作り、突然変異を行う
    pivotを使った1点交叉の例。
    """
    A1, B1 = par1
    A2, B2 = par2
    pivot = random.randint(0, N)  # 0 ~ N の間でランダムピボット
    childA = A1[:pivot] + A2[pivot:]
    childB = B1[:pivot] + B2[pivot:]

    # 突然変異
    for i in range(N):
        if random.random() < mutation_rate:
            childA[i] = random.randrange(N)
        if random.random() < mutation_rate:
            childB[i] = random.randrange(N)

    return (childA, childB)

def genetic_algorithm(N, L, T, population_size=20, generations=50):
    """
    GAで (a_i, b_i) を最適化する
    N, L, T: 問題の入力
    population_size: 集団サイズ
    generations: 世代数
    戻り値: (bestA, bestB, best_score)
    """
    # 1) 初期集団
    population = []
    for _ in range(population_size):
        A = [random.randrange(N) for _ in range(N)]
        B = [random.randrange(N) for _ in range(N)]
        population.append((A, B))

    best_score = float('inf')
    best_A = None
    best_B = None

    for gen in range(generations):
        # 2) 評価
        scored_pop = []
        for (A, B) in population:
            s = evaluate(N, L, T, A, B)
            scored_pop.append(((A,B), s))
            if s < best_score:
                best_score = s
                best_A = A[:]
                best_B = B[:]

        # スコアが小さい順にソート
        scored_pop.sort(key=lambda x: x[1])

        # 3) 選択 + 交叉
        new_population = []
        # エリート保存: 上位2割ぐらいをそのまま残す
        elite_num = max(1, population_size//5)
        for i in range(elite_num):
            new_population.append(scored_pop[i][0])

        # 残りを交叉で作成
        while len(new_population) < population_size:
            # 親をランダムに選ぶ (あるいは上位半分から選ぶなど)
            p1 = scored_pop[random.randint(0, population_size//2)][0]
            p2 = scored_pop[random.randint(0, population_size//2)][0]
            childA, childB = crossover_and_mutate(p1, p2, N)
            new_population.append((childA, childB))

        population = new_population

    return best_A, best_B, best_score

def main():
    import sys
    input_data = sys.stdin.read().strip().split()
    # 入力
    # 先頭: N, L
    # 続く: T[0], ..., T[N-1]
    N = int(input_data[0])
    L = int(input_data[1])
    T = list(map(float, input_data[2:2+N]))  # T[i]が 0..10000, intだがfloatで受けてもOK

    # GA実行
    #  - population_size, generations は適宜調整
    bestA, bestB, best_score = genetic_algorithm(N, L, T,
                                                 population_size=30,
                                                 generations=50)

    # 結果出力
    # 問題文形式: i行目に "a_i b_i"
    for i in range(N):
        # 0 <= a_i,b_i < N
        a_i = int(bestA[i])
        b_i = int(bestB[i])
        print(a_i, b_i)

if __name__=="__main__":
    main()
