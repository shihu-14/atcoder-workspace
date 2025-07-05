#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <numeric>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <atcoder/all>

using namespace std;
using namespace atcoder;

// ------------------------------------------------------------
// 型エイリアス & マクロ
// ------------------------------------------------------------
using ll = long long;
#define rep(i, n)      for (ll i = 0; i < (ll)(n); ++i)
#define rep2(i, a, b)  for (ll i = (a); i < (ll)(b); ++i)
#define all(v)         (v).begin(), (v).end()

// ------------------------------------------------------------
// 色表現 & 距離
// ------------------------------------------------------------
struct Color {
    double c, m, y;
};
inline double dist2(const Color &a, const Color &b) {
    double dc = a.c - b.c;
    double dm = a.m - b.m;
    double dy = a.y - b.y;
    return dc * dc + dm * dm + dy * dy;
}

// ------------------------------------------------------------
// k‑means++ + Lloyd (マルチスタート)
// ------------------------------------------------------------
static std::mt19937 rng(12345);

pair<vector<int>, vector<Color>> k_means_once(const vector<Color> &pts, int K, int max_iter) {
    const int n = (int)pts.size();
    std::uniform_int_distribution<int> uni(0, n - 1);

    // k‑means++ 初期化
    vector<Color> cent; cent.reserve(K);
    cent.push_back(pts[uni(rng)]);
    vector<double> dists(n, 1e100);
    while ((int)cent.size() < K) {
        double sum = 0.0;
        rep(i, n) {
            dists[i] = min(dists[i], dist2(pts[i], cent.back()));
            sum     += dists[i];
        }
        std::uniform_real_distribution<double> ur(0.0, sum);
        double r   = ur(rng);
        double acc = 0.0;
        int    id  = 0;
        while (id < n && (acc += dists[id]) < r) ++id;
        cent.push_back(pts[id]);
    }

    vector<int> assign(n, -1);
    rep(iter, max_iter) {
        bool changed = false;
        // E‑step
        rep(i, n) {
            double best = 1e100; int gid = 0;
            rep(g, K) {
                double d = dist2(pts[i], cent[g]);
                if (d < best) { best = d; gid = g; }
            }
            if (assign[i] != gid) { assign[i] = gid; changed = true; }
        }
        if (!changed) break;
        // M‑step
        vector<double> sc(K), sm(K), sy(K); vector<int> cnt(K);
        rep(i, n) {
            int g = assign[i];
            sc[g] += pts[i].c;
            sm[g] += pts[i].m;
            sy[g] += pts[i].y;
            ++cnt[g];
        }
        rep(g, K) {
            if (cnt[g] == 0) {
                cent[g] = pts[uni(rng)];
            } else {
                cent[g].c = sc[g] / cnt[g];
                cent[g].m = sm[g] / cnt[g];
                cent[g].y = sy[g] / cnt[g];
            }
        }
    }
    return {assign, cent};
}

pair<vector<int>, vector<Color>> k_means(const vector<Color> &pts, int K, int reps = 10, int max_iter = 50) {
    double best_wcss = 1e100;
    pair<vector<int>, vector<Color>> best_res;
    rep(r, reps) {
        auto res = k_means_once(pts, K, max_iter);
        double wcss = 0.0;
        rep(i, pts.size()) wcss += dist2(pts[i], res.second[res.first[i]]);
        if (wcss < best_wcss) { best_wcss = wcss; best_res = move(res); }
    }
    return best_res;
}

// ------------------------------------------------------------
// 多重集合列挙 (重複あり, 昇順)
// ------------------------------------------------------------
void dfs_ms(int K, int S, int last, vector<int> &cur, vector<vector<int>> &out) {
    if ((int)cur.size() == S) return;
    for (int i = last; i < K; ++i) {
        cur.push_back(i);
        out.push_back(cur);
        dfs_ms(K, S, i, cur, out);
        cur.pop_back();
    }
}
vector<vector<int>> gen_ms(int K, int S) {
    vector<vector<int>> res; vector<int> cur; dfs_ms(K, S, 0, cur, res); return res;
}

// ------------------------------------------------------------
struct Recipe {
    Color       col;
    vector<int> tubes;  // 注入チューブ (重複あり)
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int    N, K, H, T;
    double D;
    if (!(cin >> N >> K >> H >> T >> D)) return 0;

    vector<Color> own(K); rep(i, K) cin >> own[i].c >> own[i].m >> own[i].y;
    vector<Color> tgt(H); rep(i, H) cin >> tgt[i].c >> tgt[i].m >> tgt[i].y;

    /* --------------------------------------------------------
       パレット初期状態 (全仕切り 0 = 1 大ウェル)
    -------------------------------------------------------- */
    rep(i, N)       { rep(j, N - 1) cout << 0 << (j + 1 == N - 1 ? '\n' : ' '); }
    rep(i, N - 1)   { rep(j, N)     cout << 0 << (j + 1 == N     ? '\n' : ' '); }

    /* --------------------------------------------------------
       レシピ生成 (≤ 6 注入, 種類 ≤ 3)
    -------------------------------------------------------- */
    int max_inj = min<int>(6, T / (2 * H));
    if (max_inj < 1) max_inj = 1;

    vector<Recipe> recs;
    for (int s = 1; s <= max_inj; ++s) {
        for (auto ms : gen_ms(K, s)) {
            vector<int> uniq = ms;
            uniq.erase(unique(all(uniq)), uniq.end());
            if ((int)uniq.size() > 3) continue;  // 3 本以内制限

            Color c{0, 0, 0};
            for (int id : ms) {
                c.c += own[id].c;
                c.m += own[id].m;
                c.y += own[id].y;
            }
            double denom = ms.size();
            c.c /= denom; c.m /= denom; c.y /= denom;
            recs.push_back({c, move(ms)});
        }
    }

    /* --------------------------------------------------------
       k‑means でターゲットをクラスタリング
    -------------------------------------------------------- */
    int CL = (D < 1000 ? min(12, H) : 0);
    vector<int>    cid(H, 0);
    vector<Color>  cent;
    if (CL) tie(cid, cent) = k_means(tgt, CL);

    /* --------------------------------------------------------
       クラスタごとに最良レシピ選択
    -------------------------------------------------------- */
    vector<Recipe> best(CL);
    if (CL) {
        rep(c, CL) {
            double  best_err = 1e100;
            Recipe  best_r   = recs[0];
            for (const auto &r : recs) {
                double d = dist2(r.col, cent[c]);
                if (d < best_err) { best_err = d; best_r = r; }
            }
            best[c] = best_r;
        }
    }

    /* --------------------------------------------------------
       操作出力 & スコア計算
    -------------------------------------------------------- */
    const int X = 0, Y = 0;
    ll   ops   = 0;          // 総操作数
    ll   Vinj  = 0;          // 注入回数 (操作 1)
    long double E_sum = 0.0; // 誤差平方和

    auto inject  = [&](int k) { cout << 1 << ' ' << X << ' ' << Y << ' ' << k << '\n'; ++ops; ++Vinj; };
    auto extract = [&]()      { cout << 2 << ' ' << X << ' ' << Y << '\n';         ++ops;               };
    auto discard = [&]()      { cout << 3 << ' ' << X << ' ' << Y << '\n';         ++ops;               };

    rep(i, H) {
        Color produced;
        if (D >= 1000) {
            // 単色近似
            double best_err = 1e100; int id = 0;
            rep(kx, K) {
                double d = dist2(own[kx], tgt[i]);
                if (d < best_err) { best_err = d; id = kx; }
            }
            inject(id);
            extract();
            produced = own[id];
        } else {
            const Recipe &r = best[cid[i]];
            for (int id : r.tubes) inject(id);
            extract();
            rep(disc, (int)r.tubes.size() - 1) discard();  // 余剰廃棄
            produced = r.col;
        }
        E_sum += dist2(produced, tgt[i]);
    }

    assert(ops <= T && "operation limit over");

    /* --------------------------------------------------------
       絶対スコア計算 & stderr 出力
    -------------------------------------------------------- */
    long long err_term = llround(1e4L * E_sum);
    long double abs_score = 1.0L + D * (long double)(Vinj - H) + (long double)err_term;

    cerr << fixed << setprecision(0)
         << "#V="    << Vinj << ' '
         << "E="    << setprecision(6) << (double)E_sum << ' ' << setprecision(0)
         << "score=" << abs_score << '\n';

    return 0;
}
