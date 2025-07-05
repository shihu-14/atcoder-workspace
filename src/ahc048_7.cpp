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
// ------------------------------------------------------------
// 型エイリアス & マクロ
// ------------------------------------------------------------
using ll  = long long;
using ull = unsigned long long;
using ld  = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using uint = unsigned int;

#define rep(i, n)       for (ll i = 0; i < (ll)(n); ++i)
#define rep2(i, a, b)   for (ll i = (a); i < (ll)(b); ++i)
#define rrep(i, n)      for (ll i = (ll)(n) - 1; i >= 0; --i)
#define rrep2(i, a, b)  for (ll i = (ll)(a) - 1; i >= (ll)(b); --i)
#define rep3(i, a, b, c) for (ll i = (a); i < (ll)(b); i += (c))
#define all(v)   (v).begin(), (v).end()
#define rall(v)  (v).rbegin(), (v).rend()

// ------------------------------------------------------------
// ユーティリティ
// ------------------------------------------------------------
struct Color {
    double c, m, y;
};
inline double dist2(const Color &a, const Color &b) {
    double dc = a.c - b.c, dm = a.m - b.m, dy = a.y - b.y;
    return dc * dc + dm * dm + dy * dy;
}

// ------------------------------------------------------------
// k‑means クラスタリング (Lloyd 法)
// 返値: <各点のクラスタ ID, 重心リスト>
// ------------------------------------------------------------
pair<vector<int>, vector<Color>> k_means(const vector<Color> &pts, int k, int max_iter = 100, int seed = 42) {
    const int n = (int)pts.size();
    assert(k >= 1 && k <= n);

    mt19937 engine(seed);
    vector<int> ord(n);
    iota(ord.begin(), ord.end(), 0);
    shuffle(ord.begin(), ord.end(), engine);

    vector<Color> cent(k);
    rep(i, k) cent[i] = pts[ord[i]];   // 初期重心

    vector<int> assign(n, -1);

    rep(iter, max_iter) {
        bool changed = false;
        // ------- E step: 割り当て --------
        rep(i, n) {
            double best = 1e100;
            int best_j = 0;
            rep(j, k) {
                double d = dist2(pts[i], cent[j]);
                if (d < best) best = d, best_j = j;
            }
            if (assign[i] != best_j) {
                assign[i] = best_j;
                changed = true;
            }
        }
        if (!changed) break; // 収束

        // ------- M step: 重心再計算 --------
        vector<double> sum_c(k, 0), sum_m(k, 0), sum_y(k, 0);
        vector<int> cnt(k, 0);
        rep(i, n) {
            int g = assign[i];
            sum_c[g] += pts[i].c;
            sum_m[g] += pts[i].m;
            sum_y[g] += pts[i].y;
            ++cnt[g];
        }
        rep(j, k) {
            if (cnt[j] == 0) {
                // 空クラスタ→ランダム再配置
                int idx = engine() % n;
                cent[j] = pts[idx];
            } else {
                cent[j].c = sum_c[j] / cnt[j];
                cent[j].m = sum_m[j] / cnt[j];
                cent[j].y = sum_y[j] / cnt[j];
            }
        }
    }
    return {assign, cent};
}

// ------------------------------------------------------------
// メイン
// ------------------------------------------------------------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K, H, T;   // サイズ / チューブ数 / ターゲット数 / 最大ターン
    double D;         // 投入コスト
    if (!(cin >> N >> K >> H >> T >> D)) return 0;

    vector<Color> tube(K);
    rep(i, K) cin >> tube[i].c >> tube[i].m >> tube[i].y;

    vector<Color> target(H);
    rep(i, H) cin >> target[i].c >> target[i].m >> target[i].y;

    /* ---------------------------------------------------------
       1. 初期仕切り (全部 0)  => 1 大ウェル
       --------------------------------------------------------- */
    rep(i, N) {
        rep(j, N - 1) cout << 0 << (j + 1 == N - 1 ? '\n' : ' ');
    }
    rep(i, N - 1) {
        rep(j, N) cout << 0 << (j + 1 == N ? '\n' : ' ');
    }

    /* ---------------------------------------------------------
       2. 色候補作成 (単色 + 2 色平均)
       --------------------------------------------------------- */
    struct Cand { Color col; int k1, k2; int injections; };

    vector<Cand> base;
    rep(k, K) base.push_back({tube[k], static_cast<int>(k), -1, 1});

    if (D < 1000) {
        rep(k1, K) rep2(k2, k1 + 1, K) {
            Color mid{(tube[k1].c + tube[k2].c) * 0.5,
                      (tube[k1].m + tube[k2].m) * 0.5,
                      (tube[k1].y + tube[k2].y) * 0.5};
            base.push_back({mid, static_cast<int>(k1), static_cast<int>(k2), 2});
        }
    }

    /* ---------------------------------------------------------
       3. k‑means (低コストモードのみ)
       --------------------------------------------------------- */
    int CL = 0;
    vector<int> cluster_id(H, 0);
    vector<Color> centroids;
    if (D < 1000) {
        CL = min(10, H);                  // クラスタ数: 最大 10
        auto res = k_means(target, CL, 50, 12345);
        cluster_id = move(res.first);
        centroids  = move(res.second);
    }

    // 代表色 -> 最も近い候補を選択
    vector<Cand> rep_cand(CL);
    if (D < 1000) {
        rep(c, CL) {
            double best = 1e100; Cand best_c = base[0];
            for (const auto &cand : base) {
                double e = dist2(cand.col, centroids[c]);
                if (e < best) best = e, best_c = cand;
            }
            rep_cand[c] = best_c;
        }
    }

    /* ---------------------------------------------------------
       4. 操作生成 & 出力
       --------------------------------------------------------- */
    const int X = 0, Y = 0; // 使うマス (0,0)
    ll ops = 0;

    auto inject  = [&](int k) { cout << 1 << ' ' << X << ' ' << Y << ' ' << k << '\n'; ++ops; };
    auto extract = [&]()     { cout << 2 << ' ' << X << ' ' << Y << '\n';         ++ops; };
    auto discard = [&]()     { cout << 3 << ' ' << X << ' ' << Y << '\n';         ++ops; };

    rep(idx, H) {
        if (D >= 1000) {
            // 単色のみ: V = H
            double best = 1e100; int kbest = 0;
            rep(k, K) {
                double d = dist2(tube[k], target[idx]);
                if (d < best) best = d, kbest = static_cast<int>(k);
            }
            inject(kbest);
            extract();
        } else {
            int cid = cluster_id[idx];
            const Cand &plan = rep_cand[cid];
            if (plan.injections == 1) {
                inject(plan.k1);
                extract();
            } else {
                inject(plan.k1);
                inject(plan.k2);
                extract();
                discard(); // 残り 1 g
            }
        }
    }

    assert(ops <= (ll)T && "生成した操作数が T を超えています");
    return 0;
}
