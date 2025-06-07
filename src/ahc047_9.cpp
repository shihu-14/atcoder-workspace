/****************************************************************************************
 * AHC028 - A : Lovely Language Model  (Gradient+Momentum + Simulated-Annealing Hybrid)
 * ------------------------------------------------------------------------------------
 * 改良点:
 *   1. 勾配＋モメンタムによる 6D 確率ベクトルの高速探索
 *   2. シミュレーテッドアニーリングによる微調整
 *   3. 全体 1.9 秒以内に勾配と SA を両立
 *
 * コンパイル例 (GCC 17):
 *   g++ -std=c++17 -O2 -pipe -static -s llm_hybrid2.cpp
 ****************************************************************************************/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <climits>
#include <cfloat>
#include <cassert>
#include <ctime>
#include <cctype>
#include <cwctype>
#include <cstdint>
#include <type_traits>
#include <initializer_list>
#include <utility>
#include <bitset>
#include <vector>
#include <deque>
#include <list>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <functional>
#include <numeric>
#include <iterator>
#include <tuple>
#include <array>
#include <new>
#include <memory>
#include <limits>
#include <random>
#include <exception>
#include <stdexcept>
#include <regex>
#include <complex>
#include <chrono>
#include <future>
#include <thread>
#include <mutex>
#include <atomic>
#include <atcoder/all>
// #include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace atcoder;
// using mint = modint;
using mint = modint998244353;
// using mint = modint1000000007;
// using namespace boost::multiprecision;
using uint = unsigned int;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T3 = tuple<int, int, int>;
using G = vector<vector<int>>;
#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define rep2(i, a, b) for (ll i = a; i < (b); ++i)
#define rrep2(i, a, b) for (ll i = a-1; i >= (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
// #define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcount_ll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(rng(v))
#define MAX(v) *max_element(rng(v))
#define SUM(v) accumulate(rng(v),0LL)
#define IN(v, x) (find(rng(v),x) != v.end())
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 3001002003004005006ll;
const int INF = 1001001001;

// --- 定数 ---
constexpr int    M     = 12;         // 状態数
constexpr int    ALPHA = 6;          // 文字 a–f
constexpr long long L = 1'000'000;   // 生成文字列の長さ
constexpr double TL = 1.9;           // 制限時間（秒）

// --- パターン格納 ---
struct Pattern {
    string s;                         // S_i
    int    p;                         // P_i
    int    len;                       // |S_i|
    array<int,ALPHA> cnt{};           // 各文字出現数
};

// --- 期待スコア計算（Poisson 近似） ---
double expected_score(const vector<Pattern>& pats,
                      const array<double,ALPHA>& prob) {
    double score = 0.0;
    for (auto &pat : pats) {
        // log(Π p_c^{cnt})
        double logp = 0.0;
        for (int c = 0; c < ALPHA; ++c)
            if (pat.cnt[c]) logp += pat.cnt[c] * log(prob[c]);
        double single = exp(logp);               // 固定位置一致確率
        if (single == 0.0) continue;
        double lambda = (L - pat.len + 1) * single;
        score += pat.p * (1.0 - exp(-lambda));   // 重み付き (1 - e^-λ)
    }
    return score;
}

// --- 勾配計算 dE/dp_c（合計が 0 になるよう正規化） ---
void compute_grad(const vector<Pattern>& pats,
                  const array<double,ALPHA>& prob,
                  array<double,ALPHA>& grad) {
    grad.fill(0.0);
    for (auto &pat : pats) {
        // log・Π 計算
        double logp = 0.0;
        for (int c = 0; c < ALPHA; ++c)
            if (pat.cnt[c]) logp += pat.cnt[c] * log(prob[c]);
        double prod = exp(logp);
        if (prod == 0.0) continue;
        double lambda = (L - pat.len + 1) * prod;
        double expm   = exp(-lambda);
        // ∂/∂p_c [ P_i (1 - e^{-λ}) ] = P_i e^{-λ} (L-len+1) prod * cnt[c] / p_c
        double factor = pat.p * expm * (L - pat.len + 1) * prod;
        for (int c = 0; c < ALPHA; ++c) {
            if (pat.cnt[c])
                grad[c] += factor * (double)pat.cnt[c] / prob[c];
        }
    }
    // 合計ゼロ制約：平均を引く
    double mean = accumulate(grad.begin(), grad.end(), 0.0) / ALPHA;
    for (int c = 0; c < ALPHA; ++c) grad[c] -= mean;
}

// --- 整数遷移行列生成（largest‐remainder） ---
void build_matrix(const array<double,ALPHA>& prob,
                  array<array<int,M>,M>&     A) {
    array<double,M> base{};
    for (int j = 0; j < M; ++j)
        base[j] = prob[j % ALPHA] / 2.0;         // 各文字 2 状態分
    array<int,M> row{};
    array<pair<double,int>,M> frac;              // (小数部, idx)
    int sum = 0;
    for (int j = 0; j < M; ++j) {
        double x = base[j] * 100.0;
        row[j]   = int(floor(x));
        frac[j]  = {x - row[j], j};
        sum     += row[j];
    }
    int rem = 100 - sum;
    sort(frac.begin(), frac.end(),
         [](auto &a, auto &b){ return a.first > b.first; });
    for (int k = 0; k < rem; ++k)
        row[frac[k].second]++;
    // 全行同一
    for (int i = 0; i < M; ++i)
        for (int j = 0; j < M; ++j)
            A[i][j] = row[j];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // --- 入力読み込み ---
    int N, dummyM, dummyL;
    cin >> N >> dummyM >> dummyL;
    vector<Pattern> pats(N);
    for (int i = 0; i < N; ++i) {
        cin >> pats[i].s >> pats[i].p;
        pats[i].len = (int)pats[i].s.size();
        for (char ch : pats[i].s)
            pats[i].cnt[ch - 'a']++;
    }

    // --- 乱数生成器 & 分布 ---
    mt19937_64 rng((uint64_t)chrono::steady_clock::now()
                   .time_since_epoch().count());
    uniform_real_distribution<double> unif(0.0, 1.0);

    // --- タイマー START ---
    const auto t0 = chrono::steady_clock::now();

    // --- 初期確率 (一様) & ベスト解 ---
    array<double,ALPHA> bestP, curP;
    bestP.fill(1.0 / ALPHA);
    curP = bestP;
    double bestScore = expected_score(pats, bestP);

    // --- 勾配＋モメンタム探索 ---
    array<double,ALPHA> grad{}, momentum{};
    const int GRAD_ITERS = 200;
    for (int it = 0; it < GRAD_ITERS; ++it) {
        double elapsed = chrono::duration<double>(
            chrono::steady_clock::now() - t0).count();
        if (elapsed > TL * 0.6) break;             // 時間配分 60%

        // 勾配計算
        compute_grad(pats, curP, grad);

        // 学習率・モメンタム
        double lr   = 0.1 * (1.0 - double(it) / GRAD_ITERS) + 0.01;
        double beta = 0.9;
        for (int c = 0; c < ALPHA; ++c) {
            momentum[c] = beta * momentum[c] + (1 - beta) * grad[c];
            curP[c]    += lr * momentum[c];
        }

        // 正規化＆下限保証
        double sumP = 0.0;
        for (int c = 0; c < ALPHA; ++c) {
            curP[c] = max(curP[c], 1e-6);
            sumP += curP[c];
        }
        for (int c = 0; c < ALPHA; ++c)
            curP[c] /= sumP;

        // スコア評価
        double sc = expected_score(pats, curP);
        if (sc > bestScore) {
            bestScore = sc;
            bestP     = curP;
        }
    }

    // --- Simulated Annealing 微調整 ---
    double curScore = bestScore;
    curP = bestP;
    while (true) {
        double elapsed = chrono::duration<double>(
            chrono::steady_clock::now() - t0).count();
        if (elapsed > TL) break;

        double progress = elapsed / TL;
        double T        = 0.1 * (1.0 - progress) + 1e-9;
        double step_mag = 0.05 * (1.0 - progress) + 0.005;

        // 新候補生成
        array<double,ALPHA> cand = curP;
        int x = rng() % ALPHA, y;
        do { y = rng() % ALPHA; } while (y == x);
        double delta = (unif(rng) - 0.5) * 2.0 * step_mag;
        delta = max(-cand[x] + 1e-9, min(delta, cand[y] - 1e-9));
        cand[x] += delta;
        cand[y] -= delta;

        // 正規化
        double sumC = 0;
        for (int c = 0; c < ALPHA; ++c) {
            cand[c] = max(cand[c], 1e-9);
            sumC   += cand[c];
        }
        for (int c = 0; c < ALPHA; ++c)
            cand[c] /= sumC;

        // 評価＆受容判定
        double sc = expected_score(pats, cand);
        bool accept = (sc >= curScore)
                   || (unif(rng) < exp((sc - curScore) / T));
        if (accept) {
            curP      = cand;
            curScore  = sc;
            if (sc > bestScore) {
                bestScore = sc;
                bestP     = cand;
            }
        }
    }

    // --- 遷移行列を整数化して出力 ---
    array<array<int,M>,M> A{};
    build_matrix(bestP, A);
    for (int i = 0; i < M; ++i) {
        char Ci = char('a' + (i % ALPHA));
        cout << Ci;
        for (int j = 0; j < M; ++j)
            cout << ' ' << A[i][j];
        cout << '\n';
    }
    return 0;
}
