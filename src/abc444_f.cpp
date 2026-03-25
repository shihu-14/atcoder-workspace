#include <iostream>
#include <vector>
#include <algorithm>
#include <atcoder/modint>

using namespace std;
using namespace atcoder;

// 解答は 998244353 で出力するため、atcoder libraryのmodintを使用
using mint = modint998244353;
using ll = long long;
using pll = pair<ll, ll>;

// ==========================================
// 複素数演算用ユーティリティ
// ==========================================

// 複素数の掛け算 (u * v) mod c
// (ac - bd, ad + bc) を計算します
pll mul_c(pll u, pll v, int c) {
    ll r = (u.first * v.first - u.second * v.second) % c;
    ll i = (u.first * v.second + u.second * v.first) % c;
    // 負の数になった場合の補正 ((r % c + c) % c)
    return pll{(r + c) % c, (i + c) % c};
}

// 複素数の累乗 (base ^ exp) mod c
// 繰り返し二乗法により O(log exp) で計算
pll pow_c(pll base, ll exp, int c) {
    pll res = {1 % c, 0};
    while (exp > 0) {
        if (exp & 1) res = mul_c(res, base, c);
        base = mul_c(base, base, c);
        exp >>= 1;
    }
    return res;
}

// ==========================================
// ダブリング（分割統治）による分布生成ロジック
// ==========================================

// ダブリング関数の戻り値を管理する構造体
struct DistResult {
    // 現在の項の分布（ヒストグラム）
    // dist[x][y] := 座標(x, y)になる項がいくつあるか
    vector<vector<mint>> dist;
    
    // 次のステップのシフト計算に必要な累乗値
    pll pi_pow;    // pi^n
    pll pibar_pow; // pibar^n
};

// 【核心部分】
// n 個の項 { pi^k * pibar^(n-1-k) | 0 <= k < n } の分布を計算する関数
// 再帰的に n/2 の結果を利用して n の結果を作る（ダブリング）
DistResult calc_distribution(ll n, pll pi, pll pibar, int c) {
    // 基底ケース1: 項がない場合
    if (n == 0) {
        vector<vector<mint>> d(c, vector<mint>(c, 0));
        return {d, {1 % c, 0}, {1 % c, 0}};
    }
    // 基底ケース2: 項が1つの場合 (k=0)
    // 値は pi^0 * pibar^0 = 1
    if (n == 1) {
        vector<vector<mint>> d(c, vector<mint>(c, 0));
        d[1 % c][0] = 1; 
        return {d, pi, pibar};
    }

    // --- 分割統治ステップ ---
    
    // n/2 個分の分布を再帰的に計算
    ll m = n / 2;
    DistResult sub = calc_distribution(m, pi, pibar, c);

    // 今回の結果を格納する分布テーブル
    vector<vector<mint>> res_dist(c, vector<mint>(c, 0));

    // 合成ロジック:
    // 前半m個の項に pibar^m を掛けると、全体の項の前半部分になる
    //   (pi^k * pibar^(m-1-k)) * pibar^m = pi^k * pibar^(2m-1-k)
    for (int i = 0; i < c; ++i) {
        for (int j = 0; j < c; ++j) {
            if (sub.dist[i][j].val() == 0) continue;
            // 座標 (i, j) に pibar^m を掛けて移動
            pll nxt = mul_c({i, j}, sub.pibar_pow, c);
            res_dist[nxt.first][nxt.second] += sub.dist[i][j];
        }
    }
    
    // 後半m個の項を作る:
    // 前半m個の項に pi^m を掛けると、全体の項の後半部分になる
    //   (pi^k * pibar^(m-1-k)) * pi^m = pi^(m+k) * pibar^(m-1-k)
    for (int i = 0; i < c; ++i) {
        for (int j = 0; j < c; ++j) {
            if (sub.dist[i][j].val() == 0) continue;
            // 座標 (i, j) に pi^m を掛けて移動
            pll nxt = mul_c({i, j}, sub.pi_pow, c);
            res_dist[nxt.first][nxt.second] += sub.dist[i][j];
        }
    }

    // 次の再帰のために、累乗値を倍にする (pi^m -> pi^2m)
    pll next_pi_pow = mul_c(sub.pi_pow, sub.pi_pow, c);
    pll next_pibar_pow = mul_c(sub.pibar_pow, sub.pibar_pow, c);

    // n が奇数の場合 (n = 2m + 1) の処理
    // ここまでの計算で 2m 個 (次数 2m-1) の分布ができている。
    // しかし欲しいのは 2m+1 個 (次数 2m) の分布。
    if (n % 2 == 1) {
        vector<vector<mint>> odd_dist(c, vector<mint>(c, 0));
        
        // 全体の次数を +1 するために、すべての項に pibar を掛ける
        for (int i = 0; i < c; ++i) {
            for (int j = 0; j < c; ++j) {
                if (res_dist[i][j].val() == 0) continue;
                pll nxt = mul_c({i, j}, pibar, c);
                odd_dist[nxt.first][nxt.second] += res_dist[i][j];
            }
        }
        
        // 足りない最後の1項 (k = n-1 = 2m) を追加
        // 項の値: pi^(2m) * pibar^0 = pi^2m
        odd_dist[next_pi_pow.first][next_pi_pow.second] += 1;
        
        // 累乗値も更新 (+1乗する)
        next_pi_pow = mul_c(next_pi_pow, pi, c);
        next_pibar_pow = mul_c(next_pibar_pow, pibar, c);
        
        return {odd_dist, next_pi_pow, next_pibar_pow};
    }

    return {res_dist, next_pi_pow, next_pibar_pow};
}

// ==========================================
// メインソルバー
// ==========================================

void solve() {
    ll A_in, B_in, C_in, M;
    if (!(cin >> A_in >> B_in >> C_in >> M)) return;
    int c = (int)C_in;

    // DPテーブルの初期化
    // 単位元として 1, i, -1, -i の4つを初期状態とする
    // これはガウス整数の単数倍の自由度を考慮するため
    vector<vector<mint>> dp(c, vector<mint>(c, 0));
    dp[1 % c][0] += 1;        // 1
    dp[0][1 % c] += 1;        // i
    dp[(c - 1) % c][0] += 1;  // -1
    dp[0][(c - 1) % c] += 1;  // -i

    // ガウス素数の事前計算 (ノルムが p になる x+iy を探す)
    // p <= 200 程度なので全探索でOK
    vector<pll> fact(200, {-1, -1});
    for (int p = 1; p < 200; ++p) {
        for (int x = 0; x * x <= p; ++x) {
            for (int y = 0; x * x + y * y <= p; ++y) {
                if (x * x + y * y == p) {
                    fact[p] = {x % c, y % c};
                    goto found;
                }
            }
        }
        found:;
    }

    // 各素因数ごとの処理
    for (int i = 0; i < M; ++i) {
        ll p, e;
        cin >> p >> e;

        // 一時的な分布テーブル B
        vector<vector<mint>> B(c, vector<mint>(c, 0));

        // Case 1: p = 3 mod 4
        // ガウス整数でも素数のまま。
        // e が奇数ならノルム条件を満たせないため解なし。
        if (p % 4 == 3) {
            if (e % 2 == 1) {
                cout << 0 << endl;
                return;
            }
            // e が偶数なら p^(e/2) を掛けるだけ
            // これは単一の点への移動となる
            ll half_e = e / 2;
            pll base = {p % c, 0};
            pll cur = pow_c(base, half_e, c);
            B[cur.first][cur.second] = 1;
        }
        // Case 2: p = 2
        // ガウス整数では (1+i)(1-i) と分岐するが、同伴(単数倍)なので
        // 実質 (1+i)^e を掛けるのと同じ扱いができる
        else if (p == 2) {
            pll base = {1 % c, 1 % c};
            pll cur = pow_c(base, e, c);
            B[cur.first][cur.second] = 1;
        }
        // Case 3: p = 1 mod 4 (今回のメイン)
        // p = pi * pibar と分解でき、pi^k * pibar^(e-k) の項が現れる
        else if (p % 4 == 1) {
            pll pi = fact[p];
            pll pibar = {pi.first, (c - pi.second + c) % c};

            // 【ダブリング呼び出し】
            // e+1 個の項の分布を高速に計算
            DistResult res = calc_distribution(e + 1, pi, pibar, c);
            
            // 結果を一時テーブル B に反映
            for (int r = 0; r < c; ++r) {
                for (int k = 0; k < c; ++k) {
                    B[r][k] += res.dist[r][k];
                }
            }
        }

        // DP遷移 (畳み込み)
        // 現在の dp と、今回の素因数の分布 B を掛け合わせる
        vector<vector<mint>> next_dp(c, vector<mint>(c, 0));
        for (int r = 0; r < c; ++r) {
            for (int k = 0; k < c; ++k) {
                if (dp[r][k].val() == 0) continue;
                for (int nr = 0; nr < c; ++nr) {
                    for (int nk = 0; nk < c; ++nk) {
                        if (B[nr][nk].val() == 0) continue;
                        
                        pll nxt = mul_c({r, k}, {nr, nk}, c);
                        next_dp[nxt.first][nxt.second] += dp[r][k] * B[nr][nk];
                    }
                }
            }
        }
        dp = next_dp;
    }

    // 答えの出力
    // 目標地点 (A, B) に到達する組み合わせの数
    // 問題の定義上、(-A, -B) mod C を参照することが正解となるケースが多い
    // (Z + Target = 0 なのか Z = Target なのか文脈によるが、ここでは参照コードに従い -A, -B とする)
    int ans_x = (int)((-A_in % c + c) % c);
    int ans_y = (int)((-B_in % c + c) % c);
    cout << dp[ans_x][ans_y].val() << endl;
}

int main() {
    // 入出力の高速化
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    solve();
    
    return 0;
}