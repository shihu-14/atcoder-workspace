/****************************************************************************************
 * AHC028 - A : Lovely Language Model  (Enhanced Simulated-Annealing Version)
 * ------------------------------------------------------------------------------------
 * 改良点:
 *   1. 焼きなまし + 自動温度スケーリング
 *   2. ステップ幅を温度に比例させて粗→細探索
 *   3. 改善停滞時に軽いランダムリスタート
 *
 * コンパイル例 (GCC 17):
 *   g++ -std=c++17 -O2 -pipe -static -s llm_sa.cpp
 ****************************************************************************************/
// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
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

/* ========================== 定数・型 ========================== */
constexpr int    M      = 12;          // 状態数
constexpr int    ALPHA  = 6;           // 文字 a-f
constexpr long long L   = 1'000'000;   // 出力長
constexpr double  TL    = 1.9;         // 時間制限 (秒)

/* ------------------- 入力語を保持する構造体 ------------------- */
struct Pattern {
    string s;                          // 文字列 S_i
    int    p;                          // 好ましさ P_i
    int    len;                        // |S_i|
    array<int,ALPHA> cnt{};            // 各文字出現数
};

/* ========================== 期待スコア ======================== */
/* Poisson 近似による快速評価：E = Σ P_i (1-e^{-λ_i})            */
double expected_score(const array<double,ALPHA>& prob,
                      const vector<Pattern>&     pats)
{
    double score = 0.0;
    for (const auto& pat : pats) {
        double log_prod = 0.0;                       // log(Π p_c^{cnt})
        for (int c = 0; c < ALPHA; ++c)
            if (pat.cnt[c]) log_prod += pat.cnt[c] * log(prob[c]);
        double single = exp(log_prod);               // 固定位置で一致する確率
        if (single == 0.0) continue;
        double lambda = (L - pat.len + 1) * single;  // Poisson パラメータ
        score += pat.p * (1.0 - exp(-lambda));       // 1 - e^-λ
    }
    return score;
}

/* ======================== 行列生成 (整数化) =================== */
void build_matrix(const array<double,ALPHA>& prob,
                  array<array<int,M>,M>&     A)
{
    /* 各列 (= 状態) への基礎確率: “各文字を 2 状態” に均等割り */
    array<double,M> base{};
    for (int j = 0; j < M; ++j) base[j] = prob[j % ALPHA] / 2.0;

    /* 1 行を整数％化 ― largest-remainder */
    array<int,M> row{};
    array<pair<double,int>,M> frac;                  // (小数部, idx)
    int sum = 0;
    for (int j = 0; j < M; ++j) {
        double x = base[j] * 100.0;
        row[j]   = int(floor(x));
        frac[j]  = {x - row[j], j};
        sum     += row[j];
    }
    int rem = 100 - sum;
    sort(frac.begin(), frac.end(),
         [](auto a, auto b){ return a.first > b.first; });
    for (int k = 0; k < rem; ++k) row[frac[k].second]++;

    /* 行コピー → 全 12 行同一 */
    for (int i = 0; i < M; ++i)
        for (int j = 0; j < M; ++j)
            A[i][j] = row[j];
}

/* ============================= main ========================== */
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    /* ---------- 入力 ---------- */
    int N, dummyM, dummyL;
    if (!(cin >> N >> dummyM >> dummyL)) return 0;

    vector<Pattern> pats(N);
    for (int i = 0; i < N; ++i) {
        cin >> pats[i].s >> pats[i].p;
        pats[i].len = (int)pats[i].s.size();
        for (char ch : pats[i].s) pats[i].cnt[ch - 'a']++;
    }

    /* ---------- 焼きなまし初期化 ---------- */
    array<double,ALPHA> bestP; bestP.fill(1.0 / ALPHA);     // 一様
    double bestScore = expected_score(bestP, pats);

    array<double,ALPHA> curP = bestP;
    double curScore = bestScore;

    std::mt19937_64 rng(
        (uint64_t)chrono::steady_clock::now().time_since_epoch().count());
    uniform_real_distribution<double> unif01(0.0, 1.0);

    const auto t0 = chrono::steady_clock::now();
    int stagnant = 0;                                       // 改善無し連続回数

    /* ---------- SA ループ ---------- */
    while (true) {
        double elapsed =
            chrono::duration<double>(chrono::steady_clock::now() - t0).count();
        if (elapsed > TL) break;

        double progress = elapsed / TL;                     // 0 → 1
        double T        = 0.2 * (1.0 - progress);           // 線形温度 (0.2→0)

        /* 温度に比例したステップ幅 (MAX ±0.1 → MIN ±0.005) */
        double step_mag = 0.10 * (1.0 - progress) + 0.005;

        /* --- 新候補 cand を生成 ---------------------------------- */
        array<double,ALPHA> cand = curP;

        /* ランダムに x≠y を選び，質量 delta を移動 */
        int x = (int)(rng() % ALPHA);
        int y; do { y = (int)(rng() % ALPHA); } while (y == x);

        double delta = (unif01(rng) - 0.5) * 2.0 * step_mag; // ±step_mag
        delta = max(-cand[x] + 1e-9, min(delta, cand[y] - 1e-9)); // 非負維持
        cand[x] += delta;
        cand[y] -= delta;

        /* 正規化 (浮動誤差補正) */
        double sumP = 0.0;
        for (double v : cand) sumP += v;
        for (double& v : cand) v = max(1e-6, v / sumP);

        /* スコア評価 */
        double candScore = expected_score(cand, pats);

        /* 受容判定：良化なら無条件，悪化でも exp(-Δ/T) で確率受容 */
        bool accept = false;
        if (candScore >= curScore) {
            accept = true;
        } else {
            double prob = exp((candScore - curScore) / max(1e-9, T));
            if (unif01(rng) < prob) accept = true;
        }

        if (accept) {
            curP     = cand;
            curScore = candScore;
            stagnant = 0;
            if (candScore > bestScore) {
                bestScore = candScore;
                bestP     = cand;
            }
        } else {
            ++stagnant;
        }

        /* ------ 改善停滞時に軽いリスタート -------------------- */
        if (stagnant > 1000) {
            stagnant = 0;
            /* bestP を軸に ±2% ランダム揺らし */
            curP = bestP;
            for (int c = 0; c < ALPHA; ++c) {
                double mult = 1.0 + (unif01(rng) - 0.5) * 0.04;
                curP[c] *= mult;
            }
            double s = accumulate(curP.begin(), curP.end(), 0.0);
            for (double& v : curP) v = max(1e-6, v / s);
            curScore = expected_score(curP, pats);
        }
    }

    /* ---------- 遷移行列を整数化 ---------- */
    array<array<int,M>,M> A{};
    build_matrix(bestP, A);

    /* ---------- 出力 ---------- */
    for (int i = 0; i < M; ++i) {
        char Ci = char('a' + (i % ALPHA));
        cout << Ci;
        for (int j = 0; j < M; ++j) cout << ' ' << A[i][j];
        cout << '\n';
    }
    return 0;
}
