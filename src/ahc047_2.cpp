//  ------------------------------------------------------------
//  AHC028  –  Cyclic-String Simulated Annealing
//  ------------------------------------------------------------
//  長さ 12 の周期語 W を最適化し，W の無限繰り返し列 (W^∞) に
//  含まれる好きな文字列 S_i の重み総和を最大化する。
//  W が決まれば状態 0..11 に W[0]..W[11] を割り当て，
//  i → (i+1)mod12 へ遷移確率 100% とするだけで制約を満たす。
//
//  • 文字列長 ≤12 なので 2 周分の長さ 24 文字を調べれば
//    S_i が出現するか判定できる。
//  • Simulated Annealing で 1.9 秒間，W の 1 文字をランダム変異。
//    温度は線形減衰。評価は 36 語×substring 検索で軽量。
//
//  終了後，確率 1.0 で含まれる S_i の P_i 合計がスコアとなる。
//  最も重い単語だけを繰り返す貪欲法より高スコアになる
//  ケースが多い（複数語を同時に 100% 出現させられるため）。
//  ------------------------------------------------------------
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
constexpr int M = 12;                  // 状態数
constexpr int N_FIXED = 36;            // S_i 個数
constexpr double TL = 1.9;             // 制限時間 (秒)

// ---------------- 評価関数 ----------------
inline int score(const string& W,
                 const array<string,N_FIXED>& S,
                 const array<int,N_FIXED>& P,
                 array<int,N_FIXED>& hitTmp)
{
    string W2 = W + W;                 // 長さ 24
    int total = 0;
    for (int i = 0; i < N_FIXED; ++i) {
        if (hitTmp[i]) { total += P[i]; continue; }   // キャッシュ
        if (W2.find(S[i]) != string::npos) {
            hitTmp[i] = 1;
            total += P[i];
        }
    }
    return total;
}
// ------------- main -------------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, dummyM, dummyL;
    if (!(cin >> N >> dummyM >> dummyL)) return 0;     // 読めない場合は終了
    array<string,N_FIXED> S{};
    array<int,N_FIXED>    P{};
    for (int i = 0; i < N_FIXED; ++i) cin >> S[i] >> P[i];

    // -------- Simulated Annealing 初期化 --------
    std::mt19937_64 rng(
        (uint64_t)chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> posDist(0,11);
    uniform_int_distribution<int> charDist(0,5);
    uniform_real_distribution<double> uni01(0.0,1.0);

    // 初期 W: P 最大の文字列を先頭に詰め，残りを 'a'
    int bestIdx = max_element(P.begin(), P.end()) - P.begin();
    string W = S[bestIdx];
    if ((int)W.size() < M) W += string(M - W.size(), 'a');
    if ((int)W.size() > M) W = W.substr(0,M);          // 保険

    array<int,N_FIXED> hit{}, bestHit{};
    int bestScore = score(W, S, P, hit);
    bestHit = hit;

    const auto t0 = chrono::steady_clock::now();
    const double T0 = 1.0;             // 初期温度
    const double T1 = 0.001;           // 終了温度

    // -------- SA ループ --------
    while (true) {
        double elapsed = chrono::duration<double>(
            chrono::steady_clock::now() - t0).count();
        if (elapsed > TL) break;
        double prog = elapsed / TL;
        double T = T0 * pow(T1/T0, prog);             // 幾何減衰

        string cand = W;
        int pos = posDist(rng);
        char newCh = 'a' + charDist(rng);
        if (newCh == cand[pos]) continue;             // 変わらないならスキップ
        cand[pos] = newCh;

        array<int,N_FIXED> tmpHit = hit;              // 部分コピーで更新
        // 影響を受ける語だけ再判定 (高速化不要なので全判定で OK)
        int candScore = score(cand, S, P, tmpHit);

        bool accept = false;
        if (candScore >= bestScore) accept = true;
        else if (uni01(rng) < exp((candScore - bestScore)/T)) accept = true;

        if (accept) {
            W = cand;
            hit = tmpHit;
            bestScore = candScore;
            bestHit   = tmpHit;
        }
    }

    // -------- 12×12 決定論的行列を出力 --------
    for (int i = 0; i < M; ++i) {
        cout << W[i];
        for (int j = 0; j < M; ++j)
            cout << ' ' << ( (j == (i+1)%M) ? 100 : 0 );
        cout << '\n';
    }
    return 0;
}
