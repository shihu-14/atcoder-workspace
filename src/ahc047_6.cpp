/****************************************************************************************
 * AHC028 - A : Lovely Language Model  (Cross-Entropy + SA Hybrid)
 * ------------------------------------------------------------------------------------
 * 改良点:
 *   1. Cross-Entropy Method (CEM) で大まかに 6 次元確率ベクトルを探索
 *   2. Simulated Annealing (SA) で最終微調整
 *   3. 計算量を抑えつつ，多様な候補を評価して局所解脱出を強化
 *
 * コンパイル例 (GCC 17):
 *   g++ -std=c++17 -O2 -pipe -static -s llm_cem_sa.cpp
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

constexpr int    M      = 12;          // 状態数
constexpr int    ALPHA  = 6;           // 文字 a-f
constexpr long long L   = 1'000'000;   // 出力長
constexpr double  TL    = 1.9;         // 時間制限 (秒)

// 入力パターン格納
struct Pattern {
    string s;
    int    p;
    int    len;
    array<int,ALPHA> cnt{};
};

// 期待スコア計算（Poisson 近似）
double expected_score(const array<double,ALPHA>& prob,
                      const vector<Pattern>&     pats) {
    double score = 0.0;
    for (auto &pat : pats) {
        double logp = 0.0;
        for (int c = 0; c < ALPHA; ++c)
            if (pat.cnt[c]) logp += pat.cnt[c] * log(prob[c]);
        double single = exp(logp);
        if (single == 0.0) continue;
        double lambda = (L - pat.len + 1) * single;
        score += pat.p * (1.0 - exp(-lambda));
    }
    return score;
}

// 行列生成（largest-remainder により整数％化）
void build_matrix(const array<double,ALPHA>& prob,
                  array<array<int,M>,M>&     A) {
    array<double,M> base{};
    for (int j = 0; j < M; ++j)
        base[j] = prob[j % ALPHA] / 2.0;
    array<int,M> row{};
    array<pair<double,int>,M> frac;
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
    for (int i = 0; i < M; ++i)
        for (int j = 0; j < M; ++j)
            A[i][j] = row[j];
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 入力読み込み
    int N, dummyM, dummyL;
    cin >> N >> dummyM >> dummyL;
    vector<Pattern> pats(N);
    for (int i = 0; i < N; ++i) {
        cin >> pats[i].s >> pats[i].p;
        pats[i].len = (int)pats[i].s.size();
        for (char ch : pats[i].s)
            pats[i].cnt[ch - 'a']++;
    }

    // 乱数初期化
    mt19937_64 rng((uint64_t)chrono::steady_clock::now()
                   .time_since_epoch().count());
    uniform_real_distribution<double> unif01(0.0,1.0);
    normal_distribution<double> norm01(0.0,1.0);

    const auto t0 = chrono::steady_clock::now();

    // === 1) Cross-Entropy Method (CEM) ===
    const int POPSIZE   = 200;
    const int ELITESZ   = 20;
    const int CEM_ITERS = 12;

    array<double,ALPHA> meanP, sigmaP;
    meanP.fill(1.0/ALPHA);
    sigmaP.fill(0.1);

    array<double,ALPHA> bestP = meanP;
    double bestScore = expected_score(bestP, pats);

    vector<array<double,ALPHA>> population(POPSIZE);
    vector<pair<double,int>> pop_scores(POPSIZE);

    for (int iter = 0; iter < CEM_ITERS; ++iter) {
        // 時間チェック
        double elapsed = chrono::duration<double>(
            chrono::steady_clock::now() - t0).count();
        if (elapsed > TL * 0.6) break;  // CE: 時間の60%まで

        // サンプリング
        for (int i = 0; i < POPSIZE; ++i) {
            auto &cand = population[i];
            double sumP = 0.0;
            for (int c = 0; c < ALPHA; ++c) {
                cand[c] = meanP[c] + sigmaP[c] * norm01(rng);
                sumP   += (cand[c] = max(1e-9, cand[c]));
            }
            for (int c = 0; c < ALPHA; ++c) cand[c] /= sumP;
            pop_scores[i].first = expected_score(cand, pats);
            pop_scores[i].second = i;
        }
        // 上位 ELITESZ を選抜
        nth_element(pop_scores.begin(),
                    pop_scores.begin() + ELITESZ,
                    pop_scores.end(),
                    greater<>());
        // 平均 & 分散更新
        array<double,ALPHA> newMean{}, newVar{};
        for (int k = 0; k < ELITESZ; ++k) {
            auto &cand = population[pop_scores[k].second];
            for (int c = 0; c < ALPHA; ++c) newMean[c] += cand[c];
        }
        for (int c = 0; c < ALPHA; ++c) newMean[c] /= ELITESZ;
        for (int k = 0; k < ELITESZ; ++k) {
            auto &cand = population[pop_scores[k].second];
            for (int c = 0; c < ALPHA; ++c) {
                double d = cand[c] - newMean[c];
                newVar[c] += d*d;
            }
        }
        for (int c = 0; c < ALPHA; ++c) {
            sigmaP[c] = sqrt(newVar[c]/ELITESZ) + 1e-6;
            meanP[c]  = newMean[c];
        }
        // ベスト解更新
        if (pop_scores[0].first > bestScore) {
            bestScore = pop_scores[0].first;
            bestP     = population[pop_scores[0].second];
        }
    }

    // === 2) Simulated Annealing (SA) ===
    array<double,ALPHA> curP = bestP;
    double curScore = bestScore;
    int stagnant = 0;

    while (true) {
        double elapsed = chrono::duration<double>(
            chrono::steady_clock::now() - t0).count();
        if (elapsed > TL) break;

        double progress = elapsed / TL;
        double T = 0.1 * (1.0 - progress) + 1e-9;
        double step_mag = 0.05 * (1.0 - progress) + 0.005;

        // 新候補生成
        array<double,ALPHA> cand = curP;
        int x = rng()%ALPHA, y;
        do { y = rng()%ALPHA; } while(y==x);
        double delta = (unif01(rng)-0.5)*2.0*step_mag;
        delta = max(-cand[x]+1e-9, min(delta, cand[y]-1e-9));
        cand[x] += delta; cand[y] -= delta;
        double sumP = 0;
        for (int c = 0; c < ALPHA; ++c)
            sumP += (cand[c] = max(1e-9, cand[c]));
        for (int c = 0; c < ALPHA; ++c) cand[c] /= sumP;

        double sc = expected_score(cand, pats);
        bool accept = (sc >= curScore)
                   || (unif01(rng) < exp((sc-curScore)/T));
        if (accept) {
            curP = cand;
            curScore = sc;
            stagnant = 0;
            if (sc > bestScore) {
                bestScore = sc;
                bestP     = cand;
            }
        } else {
            stagnant++;
        }
        if (stagnant > 300) {
            stagnant = 0;
            curP = bestP;
            for (int c = 0; c < ALPHA; ++c)
                curP[c] *= 1.0 + (unif01(rng)-0.5)*0.02;
            double s2 = accumulate(curP.begin(), curP.end(), 0.0);
            for (int c = 0; c < ALPHA; ++c) curP[c] /= s2;
            curScore = expected_score(curP, pats);
        }
    }

    // 行列を整数化して出力
    array<array<int,M>,M> A{};
    build_matrix(bestP, A);
    for (int i = 0; i < M; ++i) {
        char Ci = char('a' + (i % ALPHA));
        cout << Ci;
        for (int j = 0; j < M; ++j) cout << ' ' << A[i][j];
        cout << '\n';
    }
    return 0;
}
