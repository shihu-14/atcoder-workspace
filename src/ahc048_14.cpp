/*************************************************
 *  AHC048 – safe single-well version (no <bits>)
 *  clang++ -std=c++17 -O2 でビルド可
 *************************************************/
#include <iostream>
#include <iomanip>
#include <vector>
#include <array>
#include <random>
#include <algorithm>
#include <cmath>
#include <cfloat>
#include <cassert>
#include <cstdint>
#include <atcoder/all>

using namespace std;

/* ---------- 型・マクロ ---------- */
using ll = long long;
#define rep(i,n) for (ll i = 0; i < (ll)(n); ++i)
#define all(v)   (v).begin(), (v).end()

/* ---------- 色ユーティリティ ---------- */
struct Col { double c, m, y; };
double dist2(const Col& a, const Col& b) {
    double dc = a.c - b.c, dm = a.m - b.m, dy = a.y - b.y;
    return dc*dc + dm*dm + dy*dy;
}
Col operator+(const Col& a, const Col& b) { return {a.c + b.c, a.m + b.m, a.y + b.y}; }
Col operator/(const Col& a, double d)     { return {a.c / d,   a.m / d,   a.y / d}; }

/* ---------- multiset 列挙 ---------- */
void dfs_ms(int K, int S, int last, std::vector<int>& cur, std::vector<std::vector<int>>& out) {
    if ((int)cur.size() == S) return;
    for (int i = last; i < K; ++i) {
        cur.push_back(i);
        out.push_back(cur);
        dfs_ms(K, S, i, cur, out);
        cur.pop_back();
    }
}
std::vector<std::vector<int>> gen_ms(int K, int S) {
    std::vector<std::vector<int>> res;
    std::vector<int> cur;
    dfs_ms(K, S, 0, cur, res);
    return res;
}

/* ---------- Recipe ---------- */
struct Recipe {
    Col col;
    std::vector<int> tubes;      // multiset, |tubes| = shots (1–6)
};
Col average(const std::vector<int>& t, const std::vector<Col>& ink) {
    Col sum{0, 0, 0};
    for (int id : t) sum = sum + ink[id];
    return sum / static_cast<double>(t.size());
}

/* ============================================================= */
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    /* ---- input ---- */
    int N, K, H, T; double D;
    if (!(cin >> N >> K >> H >> T >> D)) return 0;

    std::vector<Col> ink(K), tgt(H);
    rep(i, K) cin >> ink[i].c >> ink[i].m >> ink[i].y;
    rep(i, H) cin >> tgt[i].c >> tgt[i].m >> tgt[i].y;

    /* ---- palette: single big well (all zeros) ---- */
    rep(i, N)       { rep(j, N - 1) cout << 0 << (j + 1 == N - 1 ? '\n' : ' '); }
    rep(i, N - 1)   { rep(j, N)     cout << 0 << (j + 1 == N     ? '\n' : ' '); }

    /* ---- enumerate recipes (≤6 shots, ≤3 colors) ---- */
    const int MAX_SHOTS = std::min(6, T / (2 * H));
    std::vector<Recipe> recs;
    for (int s = 1; s <= MAX_SHOTS; ++s) {
        for (auto ms : gen_ms(K, s)) {
            auto uniq = ms;
            uniq.erase(std::unique(all(uniq)), uniq.end());
            if ((int)uniq.size() > 3) continue;
            recs.push_back({ average(ms, ink), std::move(ms) });
        }
    }
    if (recs.empty()) recs.push_back({ ink[0], {0} });   // safety

    /* ---- main operation loop ---- */
    const int X = 0, Y = 0;          // use only well (0,0)
    ll ops = 0, Vinj = 0;
    long double Esum = 0;
    const long double C_DIST = 1e4;

    auto inject = [&](int k) { cout << 1 << ' ' << X << ' ' << Y << ' ' << k << '\n'; ++ops; ++Vinj; };
    auto extract = [&]()     { cout << 2 << ' ' << X << ' ' << Y << '\n';             ++ops;        };
    auto discard = [&]()     { cout << 3 << ' ' << X << ' ' << Y << '\n';             ++ops;        };

    rep(idx, H) {
        int remain = H - idx - 1;
        ll min_future_ops = 2LL * remain;      // at least 2 ops per remaining target
        ll ops_left       = T - ops;

        /* best recipe that still fits */
        const Recipe* best = &recs[0];
        long double best_val = 1e100;

        for (const auto& r : recs) {
            int s = r.tubes.size();
            ll need = 2LL * s;                 // injects + discard + extract
            if (ops_left - need < min_future_ops) continue;
            long double val = D * (long double)(s - 1) + C_DIST * dist2(r.col, tgt[idx]);
            if (val < best_val) { best_val = val; best = &r; }
        }

        /* if even one-shot recipe cannot fit, use best single ink */
        if (ops_left - 2LL < min_future_ops) {
            double best_d = 1e100; int best_id = 0;
            rep(k, K) {
                double d = dist2(ink[k], tgt[idx]);
                if (d < best_d) { best_d = d; best_id = k; }
            }
            inject(best_id); extract();
            Esum += best_d;
            continue;
        }

        /* perform chosen recipe (safe sequence) */
        for (int id : best->tubes) inject(id);
        extract();
        rep(j, (int)best->tubes.size() - 1) discard();

        Esum += dist2(best->col, tgt[idx]);
    }

    assert(ops <= T && "operation budget exceeded");

    long long errTerm = llround(C_DIST * Esum);
    long double score = 1.0L + D * (long double)(Vinj - H) + (long double)errTerm;

    cerr << fixed << setprecision(0)
         << "#V=" << Vinj << ' '
         << "E=" << setprecision(6) << (double)Esum << ' '
         << setprecision(0) << "score=" << score << '\n';
    return 0;
}
