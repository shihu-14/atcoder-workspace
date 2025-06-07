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
using namespace std;
using namespace atcoder;
// using mint = modint;
using mint = modint998244353;
// using mint = modint1000000007;
using uint = unsigned int;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T3 = tuple<int, int, int>;
using G = vector<vector<int>>;
#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define rep2(i, a, b) for (ll i = (a); i < (b); ++i)
#define rrep2(i, a, b) for (ll i = (a)-1; i >= (b); --i)
#define rep3(i, a, b, c) for (ll i = (a); i < (b); i += (c))
#define rng(a) a.begin(), a.end()
#define rrng(a) a.rbegin(), a.rend()
#define popcount __builtin_popcount
#define popcount_ll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(rng(v))
#define MAX(v) *max_element(rng(v))
#define SUM(v) accumulate(rng(v), 0LL)
#define IN(v, x) (find(rng(v), x) != v.end())
template <class T> bool chmin(T &a, T b) { if (a > b) { a = b; return 1; } return 0; }
template <class T> bool chmax(T &a, T b) { if (a < b) { a = b; return 1; } return 0; }
template <class T> void printv(vector<T> &v) { rep(i, v.size()) cout << v[i] << " \n"[i == (ll)v.size() - 1]; }
template <class T> void printvv(vector<vector<T>> &v) { rep(i, v.size()) rep(j, v[i].size()) cout << v[i][j] << " \n"[j == (ll)v[i].size() - 1]; cout << endl; }
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 3001002003004005006ll;
const int INF = 1001001001;

/* ---------- ユーティリティ ---------- */
struct Color {
    double c, m, y;
};
inline double dist2(const Color &a, const Color &b) {
    double dc = a.c - b.c, dm = a.m - b.m, dy = a.y - b.y;
    return dc * dc + dm * dm + dy * dy;
}
inline Color avg(const Color &a, const Color &b) {
    return {(a.c + b.c) * 0.5, (a.m + b.m) * 0.5, (a.y + b.y) * 0.5};
}

/* ---------- メイン ---------- */
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    /* 入力 */
    int N, K, H;
    int T_limit, D;
    cin >> N >> K >> H >> T_limit >> D;

    vector<Color> tube(K);
    rep(i, K) cin >> tube[i].c >> tube[i].m >> tube[i].y;
    vector<Color> target(H);
    rep(i, H) cin >> target[i].c >> target[i].m >> target[i].y;

    /* --- 仕切り出力 --- */
    // 横 1 行につき偶数 j で v=0（merge）, 奇数 j で v=1（壁）
    rep(i, N) {
        rep(j, N - 1) {
            cout << (j % 2 == 0 ? 0 : 1) << (j == N - 2 ? '\n' : ' ');
        }
    }
    // 横方向の壁はすべて立てる
    rep(i, N - 1) {
        rep(j, N) {
            cout << 1 << (j == N - 1 ? '\n' : ' ');
        }
    }

    /* --- 操作列生成 --- */
    vector<string> ops;
    ops.reserve(4 * H); // 上限 4H

    const int pairs_per_row = N / 2;          // 10
    int pair_idx = 0;                         // 使用ウェルを順に回す

    rep(idx, H) {
        /* 1本近似 */
        int best1 = 0;
        double err1 = 1e100;
        rep(k, K) {
            double d = dist2(target[idx], tube[k]);
            if (d < err1) { err1 = d; best1 = k; }
        }

        /* 1:1 2本近似 */
        int bestA = 0, bestB = 1;
        double err2 = err1; // 初期値
        rep(a, K) rep2(b, a + 1, K) {
            Color mid = avg(tube[a], tube[b]);
            double d = dist2(target[idx], mid);
            if (d < err2) { err2 = d; bestA = a; bestB = b; }
        }

        /* どちらを採用？  Δscore = D + 1e4*(err2 - err1) < 0 なら 2本 */
        bool use_two = (1e4 * (err1 - err2) > D);

        /* 座標決定 */
        int row = pair_idx / pairs_per_row;
        int col_pair = pair_idx % pairs_per_row;
        int i = row;
        int j = col_pair * 2; // 左側セルが座標
        pair_idx = (pair_idx + 1) % (pairs_per_row * N);

        if (use_two) {
            /* 2本: add k1, add k2, take, discard */
            {
                ostringstream ss; ss << 1 << ' ' << i << ' ' << j << ' ' << bestA;
                ops.emplace_back(ss.str());
            }
            {
                ostringstream ss; ss << 1 << ' ' << i << ' ' << j << ' ' << bestB;
                ops.emplace_back(ss.str());
            }
            {
                ostringstream ss; ss << 2 << ' ' << i << ' ' << j;
                ops.emplace_back(ss.str());
            }
            {
                ostringstream ss; ss << 3 << ' ' << i << ' ' << j;
                ops.emplace_back(ss.str());
            }
        } else {
            /* 1本: add k, take */
            {
                ostringstream ss; ss << 1 << ' ' << i << ' ' << j << ' ' << best1;
                ops.emplace_back(ss.str());
            }
            {
                ostringstream ss; ss << 2 << ' ' << i << ' ' << j;
                ops.emplace_back(ss.str());
            }
        }
    }

    /* --- 操作列出力 (T_limit 超過なら切り捨て) --- */
    if ((int)ops.size() > T_limit) ops.resize(T_limit);
    for (auto &s : ops) cout << s << '\n';

    return 0;
}
