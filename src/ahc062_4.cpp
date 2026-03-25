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

uint32_t xor128() {
    static uint32_t x = 123456789, y = 362436069, z = 521288629, w = 88675123;
    uint32_t t = x ^ (x << 11);
    x = y; y = z; z = w;
    return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
}

double rand01() {
    return (double)(xor128() & 0xffffff) / (double)(1 << 24);
}

ll score_of_path(const vector<ll>& w, const vector<int>& p) {
    ll s = 0;
    rep(i, (int)p.size()) s += (ll)i * w[p[(int)i]];
    return s;
}

bool valid_path_grid(int N, const vector<int>& p) {
    int M = N * N;
    if ((int)p.size() != M) return false;
    vector<int> used(M, 0);
    rep(i, M) {
        int v = p[(int)i];
        if (v < 0 || v >= M || used[v]) return false;
        used[v] = 1;
        if (i + 1 < M) {
            int a = p[(int)i], b = p[(int)i + 1];
            int ax = a / N, ay = a % N;
            int bx = b / N, by = b % N;
            if (!(a != b && max(abs(ax - bx), abs(ay - by)) <= 1)) return false;
        }
    }
    return true;
}

vector<int> reversed_path(const vector<int>& p) {
    vector<int> q = p;
    reverse(rng(q));
    return q;
}

struct SAStats {
    long long iter = 0;
    long long tried2 = 0, valid2 = 0, acc2 = 0, imp2 = 0, no_cand2 = 0, worse2 = 0;
    long long tried1 = 0, valid1 = 0, acc1 = 0, imp1 = 0, no_cand1 = 0, worse1 = 0;
    long long tried_or2 = 0, valid_or2 = 0, acc_or2 = 0, imp_or2 = 0, no_cand_or2 = 0, worse_or2 = 0;
    long long tried_or3 = 0, valid_or3 = 0, acc_or3 = 0, imp_or3 = 0, no_cand_or3 = 0, worse_or3 = 0;
    long long cand2_sum = 0, cand1_sum = 0, cand_or2_sum = 0, cand_or3_sum = 0;
    long long bad_pick_removable_fail = 0;
};

struct SAConfig {
    double start_temp = 1e7;
    double end_temp = 1e3;
    int bad_sample = 8;
    int removable_sample = 8;
    int front_sample = 8;
    int move2_eval = 2;
    int or1_eval = 2;
    int or2_eval = 2;
    int or3_eval = 2;
    int or1_percent = 25;
    int or2_percent = 10;
    int or3_percent = 8;
    int front_prefix_percent = 70;
};

struct PathOptimizer {
    int grid_n = 0;
    int M = 0;
    vector<ll> w;
    vector<int> path, pos, rankv;
    ll cur_score = 0;
    ll best_score = -LINF;
    vector<int> best_path;
    SAStats stats;

    inline bool adjacent(int a, int b) const {
        int ax = a / grid_n, ay = a % grid_n;
        int bx = b / grid_n, by = b % grid_n;
        return a != b && max(abs(ax - bx), abs(ay - by)) <= 1;
    }

    inline ll score_of(const vector<int>& p) const {
        ll s = 0;
        rep(i, M) s += (ll)i * w[p[(int)i]];
        return s;
    }

    void init(int G_, const vector<ll>& weights, const vector<int>& init_path) {
        grid_n = G_;
        M = grid_n * grid_n;
        w = weights;
        path = init_path;
        pos.assign(M, -1);
        rep(i, M) pos[path[(int)i]] = (int)i;

        vector<int> ord(M);
        iota(rng(ord), 0);
        stable_sort(rng(ord), [&](int a, int b) {
            if (w[a] != w[b]) return w[a] < w[b];
            return a < b;
        });
        rankv.assign(M, 0);
        rep(i, M) rankv[ord[(int)i]] = (int)i;

        cur_score = score_of(path);
        best_score = cur_score;
        best_path = path;
        stats = SAStats();
    }

    inline ll badness_idx(int idx) const {
        return llabs((ll)idx - (ll)rankv[path[idx]]);
    }

    inline ll front_score_idx(int idx) const {
        return w[path[idx]];
    }

    inline bool removable(int idx) const {
        if (idx == 0 || idx == M - 1) return true;
        return adjacent(path[idx - 1], path[idx + 1]);
    }

    inline bool removable2(int idx) const {
        if (idx < 0 || idx + 1 >= M) return false;
        if (idx == 0) return true;
        if (idx + 1 == M - 1) return true;
        return adjacent(path[idx - 1], path[idx + 2]);
    }

    inline bool removable3(int idx) const {
        if (idx < 0 || idx + 2 >= M) return false;
        if (idx == 0) return true;
        if (idx + 2 == M - 1) return true;
        return adjacent(path[idx - 1], path[idx + 3]);
    }

    int pick_bad_index(int l, int r, int sample) const {
        int best = l + (int)(xor128() % (uint32_t)(r - l + 1));
        ll bestv = badness_idx(best);
        for (int s = 1; s < sample; ++s) {
            int idx = l + (int)(xor128() % (uint32_t)(r - l + 1));
            ll v = badness_idx(idx);
            if (v > bestv) bestv = v, best = idx;
        }
        return best;
    }

    int pick_bad_removable(int sample) {
        int best = -1;
        ll bestv = -1;
        for (int s = 0; s < sample; ++s) {
            int idx = (int)(xor128() % (uint32_t)M);
            if (!removable(idx)) continue;
            ll v = badness_idx(idx);
            if (best == -1 || v > bestv) bestv = v, best = idx;
        }
        if (best == -1) stats.bad_pick_removable_fail++;
        return best;
    }

    int pick_front_removable(int prefix_limit, int sample) {
        prefix_limit = max(0, min(M - 1, prefix_limit));
        int best = -1;
        ll bestv = -LINF;
        for (int s = 0; s < sample; ++s) {
            int idx = (int)(xor128() % (uint32_t)(prefix_limit + 1));
            if (!removable(idx)) continue;
            ll v = front_score_idx(idx);
            if (best == -1 || v > bestv) bestv = v, best = idx;
        }
        if (best == -1) return pick_bad_removable(sample);
        return best;
    }

    int pick_front_removable2(int prefix_limit, int sample) {
        prefix_limit = max(0, min(M - 2, prefix_limit));
        int best = -1;
        ll bestv = -LINF;
        for (int s = 0; s < sample; ++s) {
            int idx = (int)(xor128() % (uint32_t)(prefix_limit + 1));
            if (!removable2(idx)) continue;
            ll v = w[path[idx]] + w[path[idx + 1]];
            if (best == -1 || v > bestv) bestv = v, best = idx;
        }
        return best;
    }

    int pick_front_removable3(int prefix_limit, int sample) {
        prefix_limit = max(0, min(M - 3, prefix_limit));
        int best = -1;
        ll bestv = -LINF;
        for (int s = 0; s < sample; ++s) {
            int idx = (int)(xor128() % (uint32_t)(prefix_limit + 1));
            if (!removable3(idx)) continue;
            ll v = w[path[idx]] + w[path[idx + 1]] + w[path[idx + 2]];
            if (best == -1 || v > bestv) bestv = v, best = idx;
        }
        return best;
    }

    ll delta_2opt(int l, int r) const {
        ll delta = 0;
        while (l < r) {
            delta += (ll)(r - l) * (w[path[l]] - w[path[r]]);
            ++l; --r;
        }
        return delta;
    }

    void apply_2opt(int l, int r, ll delta) {
        while (l < r) {
            swap(path[l], path[r]);
            pos[path[l]] = l;
            pos[path[r]] = r;
            ++l; --r;
        }
        if (l == r) pos[path[l]] = l;
        cur_score += delta;
        if (cur_score > best_score) {
            best_score = cur_score;
            best_path = path;
        }
    }

    bool step_2opt(const SAConfig& cfg, double temp) {
        if (M < 2) return false;
        stats.tried2++;
        int u_idx = pick_bad_index(0, M - 2, cfg.bad_sample);
        int u = path[u_idx];
        int nu = path[u_idx + 1];
        int ux = u / grid_n, uy = u % grid_n;

        int cand[8], csz = 0;
        for (int d = 0; d < 8; ++d) {
            int vx = ux + (int)dxx[d], vy = uy + (int)dyy[d];
            if (vx < 0 || vx >= grid_n || vy < 0 || vy >= grid_n) continue;
            int j = pos[vx * grid_n + vy];
            if (j <= u_idx + 1) continue;
            if (j == M - 1 || adjacent(nu, path[j + 1])) cand[csz++] = j;
        }
        if (csz == 0) {
            stats.no_cand2++;
            return false;
        }

        stats.valid2++;
        stats.cand2_sum += csz;

        int best_j = cand[(int)(xor128() % (uint32_t)csz)];
        ll best_delta = delta_2opt(u_idx + 1, best_j);
        int eval = min(csz, cfg.move2_eval);
        for (int e = 1; e < eval; ++e) {
            int j = cand[(int)(xor128() % (uint32_t)csz)];
            ll delta = delta_2opt(u_idx + 1, j);
            if (delta > best_delta) {
                best_delta = delta;
                best_j = j;
            }
        }

        bool acc = false;
        if (best_delta >= 0) acc = true;
        else if (exp((double)best_delta / temp) > rand01()) acc = true;
        if (!acc) return false;

        apply_2opt(u_idx + 1, best_j, best_delta);
        stats.acc2++;
        if (best_delta > 0) stats.imp2++;
        else if (best_delta < 0) stats.worse2++;
        return true;
    }

    ll delta_or1(int m, int t) const {
        int x = path[m];
        ll ax = w[x], sum = 0;
        if (m < t) {
            for (int k = m + 1; k <= t; ++k) sum += w[path[k]];
            return (ll)(t - m) * ax - sum;
        } else {
            for (int k = t + 1; k <= m - 1; ++k) sum += w[path[k]];
            return sum - (ll)(m - t - 1) * ax;
        }
    }

    void apply_or1(int m, int t, ll delta) {
        int x = path[m];
        if (m < t) {
            for (int k = m; k < t; ++k) {
                path[k] = path[k + 1];
                pos[path[k]] = k;
            }
            path[t] = x;
            pos[x] = t;
        } else {
            for (int k = m; k > t + 1; --k) {
                path[k] = path[k - 1];
                pos[path[k]] = k;
            }
            path[t + 1] = x;
            pos[x] = t + 1;
        }
        cur_score += delta;
        if (cur_score > best_score) {
            best_score = cur_score;
            best_path = path;
        }
    }

    bool step_or1(const SAConfig& cfg, double temp) {
        if (M < 2) return false;
        stats.tried1++;
        int prefix = max(0, min(M - 1, (int)((long long)M * cfg.front_prefix_percent / 100LL)));
        int m;
        if ((int)(xor128() % 100U) < 70) m = pick_front_removable(prefix, cfg.front_sample);
        else m = pick_bad_removable(cfg.removable_sample);
        if (m < 0) {
            stats.no_cand1++;
            return false;
        }

        int x = path[m];
        int xx = x / grid_n, xy = x % grid_n;
        int cand[8], csz = 0;
        for (int d = 0; d < 8; ++d) {
            int vx = xx + (int)dxx[d], vy = xy + (int)dyy[d];
            if (vx < 0 || vx >= grid_n || vy < 0 || vy >= grid_n) continue;
            int t = pos[vx * grid_n + vy];
            if (t == m || t == m - 1) continue;
            if (t < m - 1) {
                if (adjacent(x, path[t + 1])) cand[csz++] = t;
            } else {
                if (t == M - 1 || adjacent(x, path[t + 1])) cand[csz++] = t;
            }
        }
        if (csz == 0) {
            stats.no_cand1++;
            return false;
        }

        stats.valid1++;
        stats.cand1_sum += csz;

        int best_t = cand[(int)(xor128() % (uint32_t)csz)];
        ll best_delta = delta_or1(m, best_t);
        int eval = min(csz, cfg.or1_eval);
        for (int e = 1; e < eval; ++e) {
            int t = cand[(int)(xor128() % (uint32_t)csz)];
            ll delta = delta_or1(m, t);
            if (delta > best_delta) {
                best_delta = delta;
                best_t = t;
            }
        }

        bool acc = false;
        if (best_delta >= 0) acc = true;
        else if (exp((double)best_delta / temp) > rand01()) acc = true;
        if (!acc) return false;

        apply_or1(m, best_t, best_delta);
        stats.acc1++;
        if (best_delta > 0) stats.imp1++;
        else if (best_delta < 0) stats.worse1++;
        return true;
    }

    ll delta_or2(int m, int t) const {
        ll seg = w[path[m]] + w[path[m + 1]];
        ll sum = 0;
        if (m < t) {
            for (int k = m + 2; k <= t; ++k) sum += w[path[k]];
            return (ll)(t - m - 1) * seg - 2LL * sum;
        } else {
            for (int k = t + 1; k <= m - 1; ++k) sum += w[path[k]];
            return 2LL * sum - (ll)(m - t - 1) * seg;
        }
    }

    void apply_or2(int m, int t, ll delta) {
        int x0 = path[m], x1 = path[m + 1];
        if (m < t) {
            for (int k = m; k + 2 <= t; ++k) {
                path[k] = path[k + 2];
                pos[path[k]] = k;
            }
            path[t - 1] = x0;
            path[t] = x1;
            pos[x0] = t - 1;
            pos[x1] = t;
        } else {
            for (int k = m + 1; k >= t + 3; --k) {
                path[k] = path[k - 2];
                pos[path[k]] = k;
            }
            path[t + 1] = x0;
            path[t + 2] = x1;
            pos[x0] = t + 1;
            pos[x1] = t + 2;
        }
        cur_score += delta;
        if (cur_score > best_score) {
            best_score = cur_score;
            best_path = path;
        }
    }

    bool step_or2(const SAConfig& cfg, double temp) {
        if (M < 4) return false;
        stats.tried_or2++;
        int prefix = max(0, min(M - 2, (int)((long long)M * cfg.front_prefix_percent / 100LL)));
        int m = pick_front_removable2(prefix, cfg.front_sample);
        if (m < 0) {
            stats.no_cand_or2++;
            return false;
        }

        int a = path[m], b = path[m + 1];
        int ax = a / grid_n, ay = a % grid_n;
        int cand[16], csz = 0;
        for (int d = 0; d < 8; ++d) {
            int vx = ax + (int)dxx[d], vy = ay + (int)dyy[d];
            if (vx < 0 || vx >= grid_n || vy < 0 || vy >= grid_n) continue;
            int t = pos[vx * grid_n + vy];
            if (t >= m - 1 && t <= m + 1) continue;
            if (t < m - 1) {
                if (adjacent(b, path[t + 1])) cand[csz++] = t;
            } else {
                if (t == M - 1 || adjacent(b, path[t + 1])) cand[csz++] = t;
            }
        }
        if (csz == 0) {
            stats.no_cand_or2++;
            return false;
        }
        sort(cand, cand + csz);
        csz = (int)(unique(cand, cand + csz) - cand);

        stats.valid_or2++;
        stats.cand_or2_sum += csz;

        int best_t = cand[(int)(xor128() % (uint32_t)csz)];
        ll best_delta = delta_or2(m, best_t);
        int eval = min(csz, cfg.or2_eval);
        for (int e = 1; e < eval; ++e) {
            int t = cand[(int)(xor128() % (uint32_t)csz)];
            ll delta = delta_or2(m, t);
            if (delta > best_delta) {
                best_delta = delta;
                best_t = t;
            }
        }

        bool acc = false;
        if (best_delta >= 0) acc = true;
        else if (exp((double)best_delta / temp) > rand01()) acc = true;
        if (!acc) return false;

        apply_or2(m, best_t, best_delta);
        stats.acc_or2++;
        if (best_delta > 0) stats.imp_or2++;
        else if (best_delta < 0) stats.worse_or2++;
        return true;
    }

    ll delta_or3(int m, int t) const {
        ll seg = w[path[m]] + w[path[m + 1]] + w[path[m + 2]];
        ll sum = 0;
        if (m < t) {
            for (int k = m + 3; k <= t; ++k) sum += w[path[k]];
            return (ll)(t - m - 2) * seg - 3LL * sum;
        } else {
            for (int k = t + 1; k <= m - 1; ++k) sum += w[path[k]];
            return 3LL * sum - (ll)(m - t - 1) * seg;
        }
    }

    void apply_or3(int m, int t, ll delta) {
        int x0 = path[m], x1 = path[m + 1], x2 = path[m + 2];
        if (m < t) {
            for (int k = m; k + 3 <= t; ++k) {
                path[k] = path[k + 3];
                pos[path[k]] = k;
            }
            path[t - 2] = x0;
            path[t - 1] = x1;
            path[t] = x2;
            pos[x0] = t - 2;
            pos[x1] = t - 1;
            pos[x2] = t;
        } else {
            for (int k = m + 2; k >= t + 4; --k) {
                path[k] = path[k - 3];
                pos[path[k]] = k;
            }
            path[t + 1] = x0;
            path[t + 2] = x1;
            path[t + 3] = x2;
            pos[x0] = t + 1;
            pos[x1] = t + 2;
            pos[x2] = t + 3;
        }
        cur_score += delta;
        if (cur_score > best_score) {
            best_score = cur_score;
            best_path = path;
        }
    }

    bool step_or3(const SAConfig& cfg, double temp) {
        if (M < 6) return false;
        stats.tried_or3++;
        int prefix = max(0, min(M - 3, (int)((long long)M * cfg.front_prefix_percent / 100LL)));
        int m = pick_front_removable3(prefix, cfg.front_sample);
        if (m < 0) {
            stats.no_cand_or3++;
            return false;
        }

        int a = path[m], c = path[m + 2];
        int ax = a / grid_n, ay = a % grid_n;
        int cand[8], csz = 0;
        for (int d = 0; d < 8; ++d) {
            int vx = ax + (int)dxx[d], vy = ay + (int)dyy[d];
            if (vx < 0 || vx >= grid_n || vy < 0 || vy >= grid_n) continue;
            int t = pos[vx * grid_n + vy];
            if (t >= m - 1 && t <= m + 2) continue;
            if (t < m - 1) {
                if (adjacent(c, path[t + 1])) cand[csz++] = t;
            } else {
                if (t == M - 1 || adjacent(c, path[t + 1])) cand[csz++] = t;
            }
        }
        if (csz == 0) {
            stats.no_cand_or3++;
            return false;
        }
        sort(cand, cand + csz);
        csz = (int)(unique(cand, cand + csz) - cand);

        stats.valid_or3++;
        stats.cand_or3_sum += csz;

        int best_t = cand[(int)(xor128() % (uint32_t)csz)];
        ll best_delta = delta_or3(m, best_t);
        int eval = min(csz, cfg.or3_eval);
        for (int e = 1; e < eval; ++e) {
            int t = cand[(int)(xor128() % (uint32_t)csz)];
            ll delta = delta_or3(m, t);
            if (delta > best_delta) {
                best_delta = delta;
                best_t = t;
            }
        }

        bool acc = false;
        if (best_delta >= 0) acc = true;
        else if (exp((double)best_delta / temp) > rand01()) acc = true;
        if (!acc) return false;

        apply_or3(m, best_t, best_delta);
        stats.acc_or3++;
        if (best_delta > 0) stats.imp_or3++;
        else if (best_delta < 0) stats.worse_or3++;
        return true;
    }

    void optimize_until(function<double()> elapsed, double end_time, const SAConfig& cfg) {
        if (M <= 1) {
            best_path = path;
            best_score = cur_score;
            return;
        }
        while (true) {
            double e = elapsed();
            if (e >= end_time) break;
            double prog = min(1.0, max(0.0, e / end_time));
            double temp = cfg.start_temp * pow(cfg.end_temp / cfg.start_temp, prog);
            stats.iter++;
            int r = (int)(xor128() % 100U);
            if (r < cfg.or3_percent) step_or3(cfg, temp);
            else if (r < cfg.or3_percent + cfg.or2_percent) step_or2(cfg, temp);
            else if (r < cfg.or3_percent + cfg.or2_percent + cfg.or1_percent) step_or1(cfg, temp);
            else step_2opt(cfg, temp);
        }
        path = best_path;
        rep(i, M) pos[path[(int)i]] = (int)i;
        cur_score = best_score;
    }
};

vector<vector<int>> build_serpentine_candidates(int N, const vector<ll>& w, int limit_keep) {
    int M = N * N;
    vector<pair<ll, vector<int>>> all;
    vector<int> cand(M);
    for (int dir = 0; dir < 2; ++dir) {
        for (int sr = 0; sr < 2; ++sr) {
            for (int sc = 0; sc < 2; ++sc) {
                int idx = 0;
                if (dir == 0) {
                    for (int i = 0; i < N; ++i) {
                        int r = (sr == 0 ? i : N - 1 - i);
                        for (int j = 0; j < N; ++j) {
                            int c = (sc == 0 ? j : N - 1 - j);
                            if (i & 1) c = N - 1 - c;
                            cand[idx++] = r * N + c;
                        }
                    }
                } else {
                    for (int j = 0; j < N; ++j) {
                        int c = (sc == 0 ? j : N - 1 - j);
                        for (int i = 0; i < N; ++i) {
                            int r = (sr == 0 ? i : N - 1 - i);
                            if (j & 1) r = N - 1 - r;
                            cand[idx++] = r * N + c;
                        }
                    }
                }
                all.push_back({score_of_path(w, cand), cand});
                vector<int> rev = reversed_path(cand);
                all.push_back({score_of_path(w, rev), rev});
            }
        }
    }
    sort(all.begin(), all.end(), [&](const auto& a, const auto& b) {
        if (a.first != b.first) return a.first > b.first;
        return a.second < b.second;
    });
    vector<vector<int>> res;
    for (auto &e : all) {
        bool dup = false;
        for (auto &p : res) if (p == e.second) { dup = true; break; }
        if (!dup) res.push_back(e.second);
        if ((int)res.size() >= limit_keep) break;
    }
    return res;
}

struct BeamState {
    ll score = -LINF;
    vector<int> path;
};

vector<BeamState> optimize_beam_candidates(
    int N,
    const vector<ll>& w,
    const vector<vector<int>>& init_cands,
    function<double()> elapsed,
    double end_time,
    const SAConfig& cfg,
    int beam_k
) {
    vector<vector<int>> cands;
    for (auto &p : init_cands) {
        if ((int)p.size() != N * N) continue;
        if (!valid_path_grid(N, p)) continue;
        bool dup = false;
        for (auto &q : cands) if (q == p) { dup = true; break; }
        if (!dup) cands.push_back(p);
    }
    if (cands.empty()) {
        cands = build_serpentine_candidates(N, w, 1);
    }

    vector<BeamState> all;
    double st = elapsed();
    double rem = max(0.0, end_time - st);
    int C = (int)cands.size();
    for (int i = 0; i < C; ++i) {
        PathOptimizer opt;
        opt.init(N, w, cands[i]);
        double sub_end = st + rem * (double)(i + 1) / (double)C;
        if (rem > 0.0) opt.optimize_until(elapsed, min(end_time, sub_end), cfg);
        BeamState b;
        b.score = opt.best_score;
        b.path = opt.best_path;
        all.push_back(std::move(b));
    }

    sort(all.begin(), all.end(), [&](const BeamState& a, const BeamState& b) {
        if (a.score != b.score) return a.score > b.score;
        return a.path < b.path;
    });
    vector<BeamState> res;
    for (auto &x : all) {
        bool dup = false;
        for (auto &y : res) if (y.path == x.path) { dup = true; break; }
        if (!dup) res.push_back(x);
        if ((int)res.size() >= beam_k) break;
    }
    return res;
}

struct ExpandResult {
    vector<int> child_path;
    ll local_bonus = -LINF;
    bool fallback = false;
};

ExpandResult expand_parent_path_2x2(int parentN, int childN, const vector<ll>& childW, const vector<int>& parent_path) {
    int PM = parentN * parentN;
    assert((int)parent_path.size() == PM);

    auto parent_to_children = [&](int b) {
        int bx = b / parentN, by = b % parentN;
        int x = bx * 2, y = by * 2;
        array<int, 4> c = {
            x * childN + y,
            x * childN + (y + 1),
            (x + 1) * childN + y,
            (x + 1) * childN + (y + 1)
        };
        return c;
    };

    auto adj_child = [&](int a, int b) {
        int ax = a / childN, ay = a % childN;
        int bx = b / childN, by = b % childN;
        return a != b && max(abs(ax - bx), abs(ay - by)) <= 1;
    };

    vector<array<array<ll, 4>, 4>> inner(PM);
    vector<array<array<array<int, 4>, 4>, 4>> best_order(PM);

    rep(b, PM) {
        auto c = parent_to_children((int)b);
        rep(e, 4) rep(x, 4) {
            inner[(int)b][(int)e][(int)x] = -LINF;
            best_order[(int)b][(int)e][(int)x] = array<int, 4>{-1, -1, -1, -1};
            if (e == x) continue;

            vector<int> rem;
            rep(k, 4) if (k != e && k != x) rem.push_back((int)k);
            int u = rem[0], v = rem[1];

            array<int, 4> ord1 = {(int)e, u, v, (int)x};
            array<int, 4> ord2 = {(int)e, v, u, (int)x};

            auto score_ord = [&](const array<int, 4>& ord) {
                ll s = 0;
                rep(t, 4) s += (ll)t * childW[c[ord[(int)t]]];
                return s;
            };

            ll s1 = score_ord(ord1), s2 = score_ord(ord2);
            if (s1 >= s2) {
                inner[(int)b][(int)e][(int)x] = s1;
                best_order[(int)b][(int)e][(int)x] = ord1;
            } else {
                inner[(int)b][(int)e][(int)x] = s2;
                best_order[(int)b][(int)e][(int)x] = ord2;
            }
        }
    }

    vector<array<array<char, 4>, 4>> ok(max(0, PM - 1));
    rep(t, PM - 1) {
        auto c1 = parent_to_children(parent_path[(int)t]);
        auto c2 = parent_to_children(parent_path[(int)t + 1]);
        rep(i, 4) rep(j, 4) ok[(int)t][(int)i][(int)j] = adj_child(c1[(int)i], c2[(int)j]);
    }

    const ll NEG = -LINF / 4;
    vector<array<ll, 4>> dp(PM);
    vector<array<int, 4>> prev_exit(PM), prev_entry(PM);
    rep(i, PM) rep(j, 4) dp[(int)i][(int)j] = NEG;

    int b0 = parent_path[0];
    rep(e, 4) rep(x, 4) if (e != x) {
        ll val = inner[b0][(int)e][(int)x];
        if (val > dp[0][(int)x]) {
            dp[0][(int)x] = val;
            prev_exit[0][(int)x] = -1;
            prev_entry[0][(int)x] = (int)e;
        }
    }

    for (int t = 1; t < PM; ++t) {
        rep(x, 4) dp[t][(int)x] = NEG;
        int b = parent_path[t];
        rep(pe, 4) if (dp[t - 1][(int)pe] > NEG / 2) {
            rep(e, 4) if (ok[t - 1][(int)pe][(int)e]) {
                rep(x, 4) if (e != x) {
                    ll val = dp[t - 1][(int)pe] + inner[b][(int)e][(int)x];
                    if (val > dp[t][(int)x]) {
                        dp[t][(int)x] = val;
                        prev_exit[t][(int)x] = (int)pe;
                        prev_entry[t][(int)x] = (int)e;
                    }
                }
            }
        }
    }

    ll best = NEG;
    int last_exit = -1;
    rep(x, 4) if (dp[PM - 1][(int)x] > best) {
        best = dp[PM - 1][(int)x];
        last_exit = (int)x;
    }

    ExpandResult res;
    if (last_exit == -1) {
        res.fallback = true;
        auto v = build_serpentine_candidates(childN, childW, 1);
        res.child_path = v[0];
        res.local_bonus = score_of_path(childW, res.child_path);
        return res;
    }

    vector<int> chosen_entry(PM), chosen_exit(PM);
    int cur_exit = last_exit;
    for (int t = PM - 1; t >= 0; --t) {
        chosen_exit[t] = cur_exit;
        chosen_entry[t] = prev_entry[t][cur_exit];
        cur_exit = prev_exit[t][cur_exit];
    }

    res.local_bonus = best;
    res.child_path.reserve(childN * childN);
    for (int t = 0; t < PM; ++t) {
        int b = parent_path[t];
        auto c = parent_to_children(b);
        auto ord = best_order[b][chosen_entry[t]][chosen_exit[t]];
        rep(k, 4) res.child_path.push_back(c[ord[(int)k]]);
    }
    return res;
}

vector<ll> aggregate_weights_cells(int N, const vector<int>& A, int blockSize) {
    int G = N / blockSize;
    vector<ll> w(G * G, 0);
    rep(bx, G) rep(by, G) {
        ll s = 0;
        rep(dx2, blockSize) rep(dy2, blockSize) {
            int x = (int)bx * blockSize + (int)dx2;
            int y = (int)by * blockSize + (int)dy2;
            s += A[x * N + y];
        }
        w[(int)bx * G + (int)by] = s;
    }
    return w;
}

string beam_scores_str(const vector<BeamState>& beam) {
    ostringstream oss;
    for (int i = 0; i < (int)beam.size(); ++i) {
        if (i) oss << ' ';
        oss << beam[i].score;
    }
    return oss.str();
}

void solve() {
    auto start_time = chrono::high_resolution_clock::now();
    auto elapsed = [&]() -> double {
        auto now = chrono::high_resolution_clock::now();
        return chrono::duration<double>(now - start_time).count();
    };

    const double TIME_LIMIT = 2.90;
    const double STAGE8_END = 0.28;
    const double STAGE4_END = 0.70;
    const double STAGE2_END = 1.08;

    int N;
    if (!(cin >> N)) return;
    vector<int> A(N * N);
    rep(i, N) rep(j, N) cin >> A[(int)i * N + (int)j];
    vector<ll> cellW(A.begin(), A.end());

    int G8 = N / 8;
    int G4 = N / 4;
    int G2 = N / 2;

    vector<ll> W8 = aggregate_weights_cells(N, A, 8);
    vector<ll> W4 = aggregate_weights_cells(N, A, 4);
    vector<ll> W2 = aggregate_weights_cells(N, A, 2);

    SAConfig cfg8;
    cfg8.start_temp = 1.0e8;
    cfg8.end_temp = 8.0e3;
    cfg8.bad_sample = 10;
    cfg8.removable_sample = 8;
    cfg8.front_sample = 14;
    cfg8.move2_eval = 3;
    cfg8.or1_eval = 3;
    cfg8.or2_eval = 2;
    cfg8.or3_eval = 2;
    cfg8.or1_percent = 26;
    cfg8.or2_percent = 10;
    cfg8.or3_percent = 10;
    cfg8.front_prefix_percent = 70;

    SAConfig cfg4;
    cfg4.start_temp = 8.0e7;
    cfg4.end_temp = 5.0e3;
    cfg4.bad_sample = 12;
    cfg4.removable_sample = 10;
    cfg4.front_sample = 16;
    cfg4.move2_eval = 3;
    cfg4.or1_eval = 3;
    cfg4.or2_eval = 3;
    cfg4.or3_eval = 2;
    cfg4.or1_percent = 28;
    cfg4.or2_percent = 16;
    cfg4.or3_percent = 8;
    cfg4.front_prefix_percent = 72;

    SAConfig cfg2;
    cfg2.start_temp = 5.0e7;
    cfg2.end_temp = 3.0e3;
    cfg2.bad_sample = 14;
    cfg2.removable_sample = 12;
    cfg2.front_sample = 18;
    cfg2.move2_eval = 4;
    cfg2.or1_eval = 4;
    cfg2.or2_eval = 3;
    cfg2.or3_eval = 2;
    cfg2.or1_percent = 28;
    cfg2.or2_percent = 18;
    cfg2.or3_percent = 10;
    cfg2.front_prefix_percent = 74;

    SAConfig cfg1;
    cfg1.start_temp = 2.8e7;
    cfg1.end_temp = 2.0e3;
    cfg1.bad_sample = 16;
    cfg1.removable_sample = 12;
    cfg1.front_sample = 20;
    cfg1.move2_eval = 4;
    cfg1.or1_eval = 4;
    cfg1.or2_eval = 3;
    cfg1.or3_eval = 2;
    cfg1.or1_percent = 24;
    cfg1.or2_percent = 24;
    cfg1.or3_percent = 14;
    cfg1.front_prefix_percent = 76;

    vector<vector<int>> init8 = build_serpentine_candidates(G8, W8, 6);
    vector<BeamState> beam8 = optimize_beam_candidates(G8, W8, init8, elapsed, STAGE8_END, cfg8, 3);

    vector<vector<int>> init4 = build_serpentine_candidates(G4, W4, 4);
    for (auto &b : beam8) {
        ExpandResult ex = expand_parent_path_2x2(G8, G4, W4, b.path);
        init4.push_back(ex.child_path);
        init4.push_back(reversed_path(ex.child_path));
    }
    vector<BeamState> beam4 = optimize_beam_candidates(G4, W4, init4, elapsed, STAGE4_END, cfg4, 3);

    vector<vector<int>> init2 = build_serpentine_candidates(G2, W2, 4);
    for (auto &b : beam4) {
        ExpandResult ex = expand_parent_path_2x2(G4, G2, W2, b.path);
        init2.push_back(ex.child_path);
        init2.push_back(reversed_path(ex.child_path));
    }
    vector<BeamState> beam2 = optimize_beam_candidates(G2, W2, init2, elapsed, STAGE2_END, cfg2, 2);

    vector<vector<int>> init1 = build_serpentine_candidates(N, cellW, 3);
    vector<ExpandResult> exp21s;
    for (auto &b : beam2) {
        ExpandResult ex = expand_parent_path_2x2(G2, N, cellW, b.path);
        exp21s.push_back(ex);
        init1.push_back(ex.child_path);
        init1.push_back(reversed_path(ex.child_path));
    }

    vector<int> best_init1;
    ll best_init1_score = -LINF;
    for (auto &p : init1) {
        if ((int)p.size() != N * N || !valid_path_grid(N, p)) continue;
        ll s = score_of_path(cellW, p);
        if (s > best_init1_score) {
            best_init1_score = s;
            best_init1 = p;
        }
    }
    if (best_init1.empty()) best_init1 = build_serpentine_candidates(N, cellW, 1)[0];

    PathOptimizer opt1;
    opt1.init(N, cellW, best_init1);
    opt1.optimize_until(elapsed, TIME_LIMIT, cfg1);

    if (!valid_path_grid(N, opt1.best_path)) {
        vector<int> fallback = build_serpentine_candidates(N, cellW, 1)[0];
        opt1.best_path = fallback;
        opt1.best_score = score_of_path(cellW, fallback);
    }

#ifdef LOCAL
    cerr << fixed << setprecision(6);
    cerr << "time_stage8_end=" << STAGE8_END
         << " time_stage4_end=" << STAGE4_END
         << " time_stage2_end=" << STAGE2_END
         << " time_limit=" << TIME_LIMIT << "\n";

    cerr << "beam8_scores=" << beam_scores_str(beam8) << "\n";
    cerr << "beam4_scores=" << beam_scores_str(beam4) << "\n";
    cerr << "beam2_scores=" << beam_scores_str(beam2) << "\n";

    if (!beam8.empty()) {
        cerr << "stage8_init_best=" << score_of_path(W8, init8[0])
             << " stage8_best=" << beam8[0].score << "\n";
    }
    if (!beam4.empty()) {
        cerr << "stage4_init_best=" << score_of_path(W4, init4[0])
             << " stage4_best=" << beam4[0].score << "\n";
    }
    if (!beam2.empty()) {
        cerr << "stage2_init_best=" << score_of_path(W2, init2[0])
             << " stage2_best=" << beam2[0].score << "\n";
    }
    for (int i = 0; i < (int)exp21s.size(); ++i) {
        cerr << "expand2to1_" << i << " bonus=" << exp21s[i].local_bonus
             << " fallback=" << exp21s[i].fallback
             << " score=" << score_of_path(cellW, exp21s[i].child_path) << "\n";
    }

    cerr << "stage1_init_score=" << best_init1_score
         << " stage1_best_score=" << opt1.best_score
         << " elapsed=" << elapsed() << "\n";

    auto dump_stats = [&](const string& name, const SAStats& s) {
        cerr << '[' << name << "] iter=" << s.iter
             << " 2opt=" << s.acc2 << '/' << s.tried2
             << " valid2=" << s.valid2
             << " imp2=" << s.imp2
             << " worse2=" << s.worse2
             << " no2=" << s.no_cand2
             << " avgCand2=" << (s.valid2 ? (double)s.cand2_sum / s.valid2 : 0.0)
             << " or1=" << s.acc1 << '/' << s.tried1
             << " valid1=" << s.valid1
             << " imp1=" << s.imp1
             << " worse1=" << s.worse1
             << " no1=" << s.no_cand1
             << " avgCand1=" << (s.valid1 ? (double)s.cand1_sum / s.valid1 : 0.0)
             << " or2=" << s.acc_or2 << '/' << s.tried_or2
             << " valid_or2=" << s.valid_or2
             << " imp_or2=" << s.imp_or2
             << " worse_or2=" << s.worse_or2
             << " no_or2=" << s.no_cand_or2
             << " avgCandOr2=" << (s.valid_or2 ? (double)s.cand_or2_sum / s.valid_or2 : 0.0)
             << " or3=" << s.acc_or3 << '/' << s.tried_or3
             << " valid_or3=" << s.valid_or3
             << " imp_or3=" << s.imp_or3
             << " worse_or3=" << s.worse_or3
             << " no_or3=" << s.no_cand_or3
             << " avgCandOr3=" << (s.valid_or3 ? (double)s.cand_or3_sum / s.valid_or3 : 0.0)
             << " removable_fail=" << s.bad_pick_removable_fail << '\n';
    };

    cerr << "valid_stage8=" << (!beam8.empty() ? valid_path_grid(G8, beam8[0].path) : 0)
         << " valid_stage4=" << (!beam4.empty() ? valid_path_grid(G4, beam4[0].path) : 0)
         << " valid_stage2=" << (!beam2.empty() ? valid_path_grid(G2, beam2[0].path) : 0)
         << " valid_stage1=" << valid_path_grid(N, opt1.best_path) << "\n";
#endif

    for (int v : opt1.best_path) {
        cout << v / N << ' ' << v % N << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    // cin >> t;
    while(t--) {
        solve();
    }
    return 0;
}


/*

chatgpt3-m


*/

