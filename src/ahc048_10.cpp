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

/* ------------------------------------------------------------
 * 型エイリアス & マクロ
 * ---------------------------------------------------------- */
using ll = long long;
#define rep(i, n)      for (ll i = 0; i < (ll)(n); ++i)
#define rep2(i, a, b)  for (ll i = (a); i < (ll)(b); ++i)
#define all(v)         (v).begin(), (v).end()

/* ------------------------------------------------------------
 * 色表現 & 距離
 * ---------------------------------------------------------- */
struct Color {
    double c, m, y;
};
inline double dist2(const Color &a, const Color &b) {
    const double dc = a.c - b.c;
    const double dm = a.m - b.m;
    const double dy = a.y - b.y;
    return dc * dc + dm * dm + dy * dy;
}

/* ------------------------------------------------------------
 * k-means++ + Lloyd (マルチスタート)
 *   - pts.size() が少数でも動作させるため K > pts.size() のときは
 *     K = pts.size() に切り詰める
 * ---------------------------------------------------------- */
static std::mt19937 rng(123456789);

pair<vector<int>, vector<Color>>
k_means_once(const vector<Color> &pts, int K, int max_iter = 30) {

    const int n = (int)pts.size();
    K          = min(K, n);
    std::uniform_int_distribution<int> uni(0, n - 1);

    /* k-means++ initialisation */
    vector<Color> cent;
    cent.reserve(K);
    cent.push_back(pts[uni(rng)]);

    vector<double> dists(n, 1e100);
    while ((int)cent.size() < K) {
        double sum = 0.0;
        rep(i, n) {
            dists[i] = min(dists[i], dist2(pts[i], cent.back()));
            sum     += dists[i];
        }
        if (sum == 0.0) break;               // すべて同一点
        std::uniform_real_distribution<double> ur(0.0, sum);
        double r   = ur(rng), acc = 0.0;
        int    id  = 0;
        while (id < n && (acc += dists[id]) < r) ++id;
        cent.push_back(pts[id]);
    }

    vector<int> assign(n, -1);

    rep(iter, max_iter) {
        bool changed = false;

        /* ------- E-step ------- */
        rep(i, n) {
            double best = 1e100; int gid = 0;
            rep(g, (int)cent.size()) {
                const double d = dist2(pts[i], cent[g]);
                if (d < best) { best = d; gid = g; }
            }
            if (assign[i] != gid) { assign[i] = gid; changed = true; }
        }
        if (!changed) break;

        /* ------- M-step ------- */
        const int KK = (int)cent.size();
        vector<double> sc(KK), sm(KK), sy(KK);
        vector<int>    cnt(KK);
        rep(i, n) {
            const int g = assign[i];
            sc[g] += pts[i].c;
            sm[g] += pts[i].m;
            sy[g] += pts[i].y;
            ++cnt[g];
        }
        rep(g, KK) {
            if (cnt[g] == 0) {                // empty cluster -> re-sample
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

pair<vector<int>, vector<Color>>
k_means(const vector<Color> &pts, int K, int reps = 8, int max_iter = 30) {
    double best_wcss = 1e100;
    pair<vector<int>, vector<Color>> best;
    rep(r, reps) {
        auto res = k_means_once(pts, K, max_iter);
        double wcss = 0.0;
        rep(i, pts.size()) wcss += dist2(pts[i], res.second[res.first[i]]);
        if (wcss < best_wcss) { best_wcss = wcss; best = move(res); }
    }
    return best;
}

/* ------------------------------------------------------------
 * 多重集合列挙 (重複あり, 昇順)
 * ---------------------------------------------------------- */
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

/* ------------------------------------------------------------
 * レシピ
 * ---------------------------------------------------------- */
struct Recipe {
    Color       col;      // 平均色
    vector<int> tubes;    // 使用インク (重複あり)
};

/* ************************************************************
 *                            main
 * ********************************************************** */
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    /* ---------------- 入力 ---------------- */
    int    N, K, H, T;
    double D;
    if (!(cin >> N >> K >> H >> T >> D)) return 0;

    vector<Color> own(K); rep(i, K) cin >> own[i].c >> own[i].m >> own[i].y;
    vector<Color> tgt(H); rep(i, H) cin >> tgt[i].c >> tgt[i].m >> tgt[i].y;

    /* ---------------- パレット設計図 (全部 0) ---------------- */
    rep(i, N)       { rep(j, N - 1) cout << 0 << (j + 1 == N - 1 ? '\n' : ' '); }
    rep(i, N - 1)   { rep(j, N)     cout << 0 << (j + 1 == N     ? '\n' : ' '); }

    /* ---------------- レシピ生成 ---------------- */
    int max_inj = min<int>(6, T / (2 * H));
    max_inj = max(max_inj, 1);

    vector<Recipe> recs;
    for (int s = 1; s <= max_inj; ++s) {
        for (auto ms : gen_ms(K, s)) {
            vector<int> uniq = ms;
            uniq.erase(unique(all(uniq)), uniq.end());
            if ((int)uniq.size() > 3) continue;          // ≤ 3 種類
            Color c{0, 0, 0};
            for (int id : ms) {
                c.c += own[id].c; c.m += own[id].m; c.y += own[id].y;
            }
            const double denom = (double)ms.size();
            c.c /= denom; c.m /= denom; c.y /= denom;
            recs.push_back({c, move(ms)});
        }
    }

    /* ---------------- 大域クラスタリング ---------------- */
    int CL = (D < 1000 ? min(12, H) : 0);
    vector<int>   cid(H, 0);
    vector<Color> cent;
    if (CL) tie(cid, cent) = k_means(tgt, CL);

    /* ========================================================
     * クラスタ → サブクラスタ (≤ 3) で代表レシピ選択
     * ====================================================== */
    vector<const Recipe*> rep_of_sub;           // subcluster id → representative
    vector<int>           subid(H, 0);          // target i → subcluster id
    if (CL) {
        int subClusterCounter = 0;
        rep(c, CL) {
            /* --- このクラスタに含まれる index 一覧 --- */
            vector<int> idx;
            rep(i, H) if (cid[i] == c) idx.push_back(i);
            const int SZ = (int)idx.size();
            if (SZ == 0) continue;

            const int SUBK = min(3, SZ);        // ≤3 サブクラスタ
            if (SUBK == 1) {
                /* 1 サブクラスタ : best recipe = recs で最短 */
                double bestE = 1e100;
                const Recipe* bestR = &recs[0];
                rep(r, recs.size()) {
                    const double d = dist2(recs[r].col, cent[c]);
                    if (d < bestE) { bestE = d; bestR = &recs[r]; }
                }
                /* subcluster id を付ける */
                rep(i, SZ)    subid[idx[i]] = subClusterCounter;
                rep_of_sub.push_back(bestR);
                ++subClusterCounter;
            } else {
                /* ---------- サブクラスタリング ---------- */
                vector<Color> local; local.reserve(SZ);
                for (int id : idx) local.push_back(tgt[id]);
                auto [loc_cid, loc_cent] = k_means(local, SUBK);

                /* 各サブクラスタの代表レシピ (=最近レシピ) 決定 */
                vector<const Recipe*> subRep(SUBK, nullptr);
                rep(sc, SUBK) {
                    double bestE = 1e100;
                    const Recipe* bestR = &recs[0];
                    for (const Recipe &r : recs) {
                        const double d = dist2(r.col, loc_cent[sc]);
                        if (d < bestE) { bestE = d; bestR = &r; }
                    }
                    subRep[sc] = bestR;
                }
                /* target へ subcluster id と代表レシピを割当 */
                rep(i, SZ) {
                    const int sc = loc_cid[i];               // 0..SUBK-1
                    const int globalSubId = subClusterCounter + sc;
                    subid[idx[i]] = globalSubId;
                }
                rep(sc, SUBK) rep_of_sub.push_back(subRep[sc]);
                subClusterCounter += SUBK;
            }
        }
    } else {
        /* クラスタリングを行わない (D ≥ 1000) 場合はダミー 1 サブクラスタ */
        rep_of_sub.push_back(nullptr);   // placeholder
    }

    /* ========================================================
     * 操作生成
     * ====================================================== */
    const int X = 0, Y = 0;
    ll   ops  = 0;          // 総操作数
    ll   Vinj = 0;          // 注入回数
    long double E_sum = 0;  // 誤差平方和

    auto inject  = [&](int k) { cout << 1 << ' ' << X << ' ' << Y << ' ' << k << '\n'; ++ops; ++Vinj; };
    auto extract = [&]()      { cout << 2 << ' ' << X << ' ' << Y << '\n'; ++ops; };
    auto discard = [&]()      { cout << 3 << ' ' << X << ' ' << Y << '\n'; ++ops; };

    rep(i, H) {
        Color produced;
        if (D >= 1000) {
            /* --------- 単色近似 --------- */
            double best_err = 1e100; int best_id = 0;
            rep(kx, K) {
                const double d = dist2(own[kx], tgt[i]);
                if (d < best_err) { best_err = d; best_id = kx; }
            }
            inject(best_id); extract();
            produced = own[best_id];
        } else {
            /* --------- サブクラスタ代表レシピ --------- */
            const Recipe *r = rep_of_sub[subid[i]];
            for (int id : r->tubes) inject(id);
            extract();
            rep(disc, (int)r->tubes.size() - 1) discard();
            produced = r->col;
        }
        E_sum += dist2(produced, tgt[i]);
    }

    assert(ops <= T && "operation limit exceeded");

    /* ---------------- スコア計算 & stderr ---------------- */
    const long long err_term = llround(1e4L * E_sum);
    const long double abs_score = 1.0L + D * (long double)(Vinj - H) + (long double)err_term;

    cerr << fixed << setprecision(0)
         << "#V=" << Vinj << ' '
         << "E="  << setprecision(6) << (double)E_sum << ' ' << setprecision(0)
         << "score=" << abs_score << '\n';

    return 0;
}
