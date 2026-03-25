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
#define rrep2(i, a, b) for (ll i = int(a)-1; i >= (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
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

struct XorShift64
{
    ull x;
    explicit XorShift64(ull seed = 88172645463393265ULL) : x(seed) {}

    inline ull next_u64()
    {
        x ^= x << 7;
        x ^= x >> 9;
        return x;
    }

    inline int next_int(int l, int r)
    {
        return l + int(next_u64() % (ull)(r - l + 1));
    }

    inline double next_double()
    {
        return (next_u64() >> 11) * (1.0 / 9007199254740992.0);
    }
};

struct ReverseMove
{
    int l = -1;
    int r = -1;
    ll delta = -LINF;
    bool valid = false;
};

struct Solver
{
    int N = 0, M = 0;
    vector<int> A;

    vector<array<int, 8>> nei;
    vector<int> deg;

    XorShift64 xrnd;

    vector<int> path, pos;
    ll cur_score = 0;

    vector<int> best_path;
    ll best_score = -LINF;

    chrono::steady_clock::time_point run_start;

#ifdef LOCAL
    vector<ll> init_candidate_scores;
#endif

    long long propose_count = 0;
    long long accept_count = 0;
    long long improve_count = 0;
    long long batch_count = 0;

    array<long long, 6> len_proposed{};
    array<long long, 6> len_accepted{};
    array<long long, 6> pos_proposed{};
    array<long long, 6> pos_accepted{};
    long long accepted_len_sum = 0;
    long long improved_len_sum = 0;

    Solver() : xrnd((ull)chrono::steady_clock::now().time_since_epoch().count()) {}

    inline int id(int i, int j) const
    {
        return i * N + j;
    }

    inline int row(int v) const
    {
        return v / N;
    }

    inline int col(int v) const
    {
        return v % N;
    }

    inline bool adjacent_cell(int a, int b) const
    {
        int ai = a / N, aj = a % N;
        int bi = b / N, bj = b % N;
        int di = ai - bi; if (di < 0) di = -di;
        int dj = aj - bj; if (dj < 0) dj = -dj;
        return max(di, dj) == 1;
    }

    inline int bucket_len(int len) const
    {
        if (len <= 8) return 0;
        if (len <= 16) return 1;
        if (len <= 32) return 2;
        if (len <= 64) return 3;
        if (len <= 128) return 4;
        return 5;
    }

    inline int bucket_pos(int l) const
    {
        int t = (int)((6LL * l) / max(1, M));
        if (t < 0) t = 0;
        if (t > 5) t = 5;
        return t;
    }

    void build_neighbors()
    {
        nei.assign(M, array<int, 8>{});
        deg.assign(M, 0);

        rep(i, N) rep(j, N)
        {
            int v = id((int)i, (int)j);
            int d = 0;
            rep(dir, 8)
            {
                int ni = (int)i + (int)dxx[dir];
                int nj = (int)j + (int)dyy[dir];
                if (0 <= ni && ni < N && 0 <= nj && nj < N)
                {
                    nei[v][d++] = id(ni, nj);
                }
            }
            deg[v] = d;
            while (d < 8) nei[v][d++] = -1;
        }
    }

    ll calc_score(const vector<int>& p) const
    {
        ll s = 0;
        rep(k, M) s += (ll)k * (ll)A[p[(int)k]];
        return s;
    }

    int transform_rc(int x, int y, int sym) const
    {
        int nx = x, ny = y;
        if (sym == 0) { nx = x;         ny = y;         }
        else if (sym == 1) { nx = y;         ny = N - 1 - x; }
        else if (sym == 2) { nx = N - 1 - x; ny = N - 1 - y; }
        else if (sym == 3) { nx = N - 1 - y; ny = x;         }
        else if (sym == 4) { nx = x;         ny = N - 1 - y; }
        else if (sym == 5) { nx = N - 1 - y; ny = N - 1 - x; }
        else if (sym == 6) { nx = N - 1 - x; ny = y;         }
        else               { nx = y;         ny = x;         }
        return id(nx, ny);
    }

    vector<int> make_row_snake_sym(int sym) const
    {
        vector<int> p;
        p.reserve(M);

        rep(i, N)
        {
            if ((int)i % 2 == 0)
            {
                rep(j, N) p.push_back(transform_rc((int)i, (int)j, sym));
            }
            else
            {
                rrep2(j, N, 0) p.push_back(transform_rc((int)i, (int)j, sym));
            }
        }
        return p;
    }

    vector<int> make_col_snake_sym(int sym) const
    {
        vector<int> p;
        p.reserve(M);

        rep(j, N)
        {
            if ((int)j % 2 == 0)
            {
                rep(i, N) p.push_back(transform_rc((int)i, (int)j, sym));
            }
            else
            {
                rrep2(i, N, 0) p.push_back(transform_rc((int)i, (int)j, sym));
            }
        }
        return p;
    }

    void set_path(const vector<int>& p)
    {
        path = p;
        pos.assign(M, -1);
        rep(k, M) pos[path[(int)k]] = (int)k;
        cur_score = calc_score(path);

        if (best_path.empty() || cur_score > best_score)
        {
            best_path = path;
            best_score = cur_score;
        }
    }

    ll delta_reverse(int l, int r) const
    {
        ll delta = 0;
        int s = l + r;
        for (int k = l; k <= r; ++k)
        {
            delta += (ll)(s - 2 * k) * (ll)A[path[k]];
        }
        return delta;
    }

    void apply_reverse(int l, int r, ll delta)
    {
        reverse(path.begin() + l, path.begin() + r + 1);
        for (int k = l; k <= r; ++k) pos[path[k]] = k;
        cur_score += delta;

        if (cur_score > best_score)
        {
            best_score = cur_score;
            best_path = path;
        }
    }

    inline bool can_reverse(int l, int r) const
    {
        if (!(0 <= l && l < r && r < M)) return false;
        if (l > 0 && !adjacent_cell(path[l - 1], path[r])) return false;
        if (r + 1 < M && !adjacent_cell(path[l], path[r + 1])) return false;
        return true;
    }

    bool validate_path(const vector<int>& p) const
    {
        if ((int)p.size() != M) return false;

        vector<int> used(M, 0);
        rep(k, M)
        {
            int v = p[(int)k];
            if (v < 0 || v >= M) return false;
            if (used[v]) return false;
            used[v] = 1;
        }

        rep(k, M - 1)
        {
            if (!adjacent_cell(p[(int)k], p[(int)k + 1])) return false;
        }
        return true;
    }

    bool check_internal_state() const
    {
        if ((int)path.size() != M || (int)pos.size() != M) return false;

        rep(k, M)
        {
            int v = path[(int)k];
            if (v < 0 || v >= M) return false;
            if (pos[v] != k) return false;
        }

        if (!validate_path(path)) return false;
        if (calc_score(path) != cur_score) return false;
        return true;
    }

    inline ll badness_of_pos(int k) const
    {
        int v = path[k];
        return llabs((ll)k - (ll)(A[v] - 1));
    }

    int pick_l(bool biased)
    {
        if (!biased || M <= 1) return xrnd.next_int(0, M - 2);

        const int SAMPLE = 28;
        int best_k = xrnd.next_int(0, M - 2);
        ll best_bad = -1;

        for (int t = 0; t < SAMPLE; ++t)
        {
            int k = xrnd.next_int(0, M - 2);
            ll bad = badness_of_pos(k);
            if (bad > best_bad)
            {
                best_bad = bad;
                best_k = k;
            }
        }
        return best_k;
    }

    ReverseMove propose_reverse(int max_len, bool biased)
    {
        ReverseMove best;

        int l = pick_l(biased);
        int left = (l > 0 ? path[l - 1] : -1);
        int first = path[l];

        int cand[24];
        int csz = 0;

        if (left != -1)
        {
            for (int t = 0; t < deg[left]; ++t)
            {
                int v = nei[left][t];
                int r = pos[v];
                if (r <= l) continue;
                if (r - l + 1 > max_len) continue;
                if (r + 1 < M && !adjacent_cell(first, path[r + 1])) continue;

                bool dup = false;
                for (int z = 0; z < csz; ++z) if (cand[z] == r) dup = true;
                if (!dup) cand[csz++] = r;
            }
        }

        for (int t = 0; t < deg[first]; ++t)
        {
            int v = nei[first][t];
            int p = pos[v];
            if (p <= l + 1) continue;
            int r = p - 1;
            if (r <= l) continue;
            if (r - l + 1 > max_len) continue;
            if (l > 0 && !adjacent_cell(left, path[r])) continue;

            bool dup = false;
            for (int z = 0; z < csz; ++z) if (cand[z] == r) dup = true;
            if (!dup) cand[csz++] = r;
        }

        if (csz == 0)
        {
            int r = min(M - 1, l + xrnd.next_int(1, max(1, max_len - 1)));
            if (r > l && can_reverse(l, r))
            {
                ll delta = delta_reverse(l, r);
                best = {l, r, delta, true};
            }
            return best;
        }

        for (int z = 0; z < csz; ++z)
        {
            int r = cand[z];
            if (!can_reverse(l, r)) continue;
            ll delta = delta_reverse(l, r);
            if (!best.valid || delta > best.delta)
            {
                best = {l, r, delta, true};
            }
        }

        return best;
    }

    void record_proposed(const ReverseMove& mv)
    {
        ++propose_count;
        int len = mv.r - mv.l + 1;
        ++len_proposed[bucket_len(len)];
        ++pos_proposed[bucket_pos(mv.l)];
    }

    void record_accepted(const ReverseMove& mv, bool improved)
    {
        ++accept_count;
        int len = mv.r - mv.l + 1;
        ++len_accepted[bucket_len(len)];
        ++pos_accepted[bucket_pos(mv.l)];
        accepted_len_sum += len;

        if (improved)
        {
            ++improve_count;
            improved_len_sum += len;
        }
    }

    bool try_apply_best_of_batch(int batch_size, int max_len, double temp, bool allow_worse, bool biased)
    {
        ReverseMove best_mv;
        ++batch_count;

        for (int t = 0; t < batch_size; ++t)
        {
            ReverseMove mv = propose_reverse(max_len, biased);
            if (!mv.valid) continue;
            if (!best_mv.valid || mv.delta > best_mv.delta)
            {
                best_mv = mv;
            }
        }

        if (!best_mv.valid) return false;

        record_proposed(best_mv);

        if (best_mv.delta >= 0)
        {
            apply_reverse(best_mv.l, best_mv.r, best_mv.delta);
            record_accepted(best_mv, best_mv.delta > 0);
            return true;
        }

        if (!allow_worse || temp <= 1e-12) return false;

        double prob = exp((double)best_mv.delta / temp);
        if (xrnd.next_double() < prob)
        {
            apply_reverse(best_mv.l, best_mv.r, best_mv.delta);
            record_accepted(best_mv, false);
            return true;
        }
        return false;
    }

    inline double elapsed_time() const
    {
        return chrono::duration<double>(chrono::steady_clock::now() - run_start).count();
    }

    vector<int> build_initial_path()
    {
        vector<int> best;
        ll best_s = -LINF;

#ifdef LOCAL
        init_candidate_scores.clear();
#endif

        auto consider = [&](vector<int> p)
        {
            ll s = calc_score(p);
#ifdef LOCAL
            init_candidate_scores.push_back(s);
#endif
            if (best.empty() || s > best_s)
            {
                best = p;
                best_s = s;
            }
        };

        rep(sym, 8)
        {
            vector<int> p1 = make_row_snake_sym((int)sym);
            consider(p1);
            reverse(rng(p1));
            consider(p1);

            vector<int> p2 = make_col_snake_sym((int)sym);
            consider(p2);
            reverse(rng(p2));
            consider(p2);
        }

        return best;
    }

    void greedy_improve(double end_time_sec)
    {
        while (elapsed_time() < end_time_sec)
        {
            try_apply_best_of_batch(24, 80, 0.0, false, true);
        }
    }

    void anneal(double end_time_sec)
    {
        double start_sec = elapsed_time();
        double span = max(1e-9, end_time_sec - start_sec);

        const double T0 = 1.5e7;
        const double T1 = 2.0e3;

        while (true)
        {
            double now = elapsed_time();
            if (now >= end_time_sec) break;

            double progress = (now - start_sec) / span;
            if (progress < 0.0) progress = 0.0;
            if (progress > 1.0) progress = 1.0;

            double temp = pow(T0, 1.0 - progress) * pow(T1, progress);
            int max_len = 16 + (int)((1.0 - progress) * 144.0);
            int batch = (progress < 0.35 ? 12 : 8);
            bool biased = (xrnd.next_double() < 0.82);

            try_apply_best_of_batch(batch, max_len, temp, true, biased);
        }
    }

    void output_best() const
    {
        rep(k, M)
        {
            int v = best_path[(int)k];
            cout << row(v) << ' ' << col(v) << '\n';
        }
    }

#ifdef LOCAL
    void dump_debug() const
    {
        cerr << "best_score=" << best_score << '\n';
        cerr << "propose=" << propose_count
             << " accept=" << accept_count
             << " improve=" << improve_count
             << " batches=" << batch_count << '\n';
        cerr << "accepted_avg_len=" << (accept_count ? (double)accepted_len_sum / accept_count : 0.0)
             << " improved_avg_len=" << (improve_count ? (double)improved_len_sum / improve_count : 0.0) << '\n';

        cerr << "len_proposed:";
        rep(i, 6) cerr << ' ' << len_proposed[(int)i];
        cerr << '\n';

        cerr << "len_accepted:";
        rep(i, 6) cerr << ' ' << len_accepted[(int)i];
        cerr << '\n';

        cerr << "pos_proposed:";
        rep(i, 6) cerr << ' ' << pos_proposed[(int)i];
        cerr << '\n';

        cerr << "pos_accepted:";
        rep(i, 6) cerr << ' ' << pos_accepted[(int)i];
        cerr << '\n';

        if (!init_candidate_scores.empty())
        {
            ll mn = *min_element(rng(init_candidate_scores));
            ll mx = *max_element(rng(init_candidate_scores));
            cerr << "init_candidates=" << init_candidate_scores.size()
                 << " min=" << mn
                 << " max=" << mx << '\n';
        }
    }
#endif

    void run()
    {
        build_neighbors();
        run_start = chrono::steady_clock::now();

        vector<int> init = build_initial_path();
        set_path(init);

#ifdef LOCAL
        if (!check_internal_state())
        {
            cerr << "initial state invalid\n";
            exit(1);
        }
#endif

        const double TOTAL_TIME = 2.85;
        const double GREEDY_END = 0.30;

        greedy_improve(GREEDY_END);
        anneal(TOTAL_TIME);

        path = best_path;
        pos.assign(M, -1);
        rep(k, M) pos[path[(int)k]] = (int)k;
        cur_score = best_score;

#ifdef LOCAL
        if (!check_internal_state())
        {
            cerr << "final state invalid\n";
            exit(1);
        }
        dump_debug();
#endif

        output_best();
    }
};

void solve()
{
    Solver solver;
    cin >> solver.N;
    solver.M = solver.N * solver.N;
    solver.A.assign(solver.M, 0);

    rep(i, solver.N) rep(j, solver.N)
    {
        cin >> solver.A[(int)i * solver.N + (int)j];
    }

    solver.run();
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    // cin >> t;
    while(t--)
    {
        solve();
    }
    return 0;
}