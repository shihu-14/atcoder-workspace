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

bool valid_path_grid(int G, const vector<int>& p) {
    int M = G * G;
    if ((int)p.size() != M) return false;
    vector<int> used(M, 0);
    rep(i, M) {
        int v = p[(int)i];
        if (v < 0 || v >= M || used[v]) return false;
        used[v] = 1;
        if (i + 1 < M) {
            int a = p[(int)i], b = p[(int)i + 1];
            int ax = a / G, ay = a % G;
            int bx = b / G, by = b % G;
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
    array<long long, 4> tried_or{};
    array<long long, 4> valid_or{};
    array<long long, 4> acc_or{};
    array<long long, 4> imp_or{};
    array<long long, 4> no_cand_or{};
    array<long long, 4> worse_or{};
    array<long long, 4> cand_or_sum{};
    long long cand2_sum = 0;
    long long removable_fail = 0;
};

struct SAConfig {
    double start_temp = 1e7;
    double end_temp = 1e3;
    int bad_sample = 8;
    int removable_sample = 8;
    int front_sample = 8;
    int move2_eval = 2;
    int or_eval = 2;
    int or_percent = 30;
    int front_prefix_percent = 70;
    array<int, 4> or_len_weight = {0, 100, 0, 0};
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

    inline ll front_score_idx(int idx, int len) const {
        ll s = 0;
        rep(k, len) s += w[path[idx + (int)k]];
        return s;
    }

    inline bool removable_seg(int idx, int len) const {
        if (idx < 0 || idx + len > M) return false;
        if (idx == 0 || idx + len == M) return true;
        return adjacent(path[idx - 1], path[idx + len]);
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

    int pick_removable_index(int len, int prefix_limit, int sample, bool prefer_front) {
        prefix_limit = max(0, min(M - len, prefix_limit));
        int best = -1;
        ll bestv = prefer_front ? -LINF : -1;
        for (int s = 0; s < sample; ++s) {
            int idx = (int)(xor128() % (uint32_t)(prefix_limit + 1));
            if (!removable_seg(idx, len)) continue;
            ll v = prefer_front ? front_score_idx(idx, len) : badness_idx(idx);
            if (best == -1 || v > bestv) bestv = v, best = idx;
        }
        if (best == -1) {
            for (int s = 0; s < sample; ++s) {
                int idx = (int)(xor128() % (uint32_t)(M - len + 1));
                if (!removable_seg(idx, len)) continue;
                ll v = prefer_front ? front_score_idx(idx, len) : badness_idx(idx);
                if (best == -1 || v > bestv) bestv = v, best = idx;
            }
        }
        if (best == -1) stats.removable_fail++;
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

    ll delta_or_len(int m, int len, int t) const {
        ll segsum = 0;
        rep(k, len) segsum += w[path[m + (int)k]];
        ll sum = 0;
        if (m < t) {
            for (int k = m + len; k <= t; ++k) sum += w[path[k]];
            return (ll)(t - (m + len) + 1) * segsum - (ll)len * sum;
        } else {
            for (int k = t + 1; k <= m - 1; ++k) sum += w[path[k]];
            return (ll)len * sum - (ll)(m - (t + 1)) * segsum;
        }
    }

    void apply_or_len(int m, int len, int t, ll delta) {
        int r = m + len - 1;
        array<int, 3> seg = {-1, -1, -1};
        rep(k, len) seg[(int)k] = path[m + (int)k];
        if (m < t) {
            for (int k = m; k + len <= t; ++k) {
                path[k] = path[k + len];
                pos[path[k]] = k;
            }
            for (int k = 0; k < len; ++k) {
                path[t - len + 1 + k] = seg[k];
                pos[seg[k]] = t - len + 1 + k;
            }
        } else {
            for (int k = r; k >= t + 1 + len; --k) {
                path[k] = path[k - len];
                pos[path[k]] = k;
            }
            for (int k = 0; k < len; ++k) {
                path[t + 1 + k] = seg[k];
                pos[seg[k]] = t + 1 + k;
            }
        }
        cur_score += delta;
        if (cur_score > best_score) {
            best_score = cur_score;
            best_path = path;
        }
    }

    bool step_or_len(const SAConfig& cfg, double temp, int len) {
        if (M < len + 1) return false;
        stats.tried_or[len]++;
        int prefix = max(0, min(M - len, (int)((long long)M * cfg.front_prefix_percent / 100LL)));
        int m;
        if ((int)(xor128() % 100U) < 70) m = pick_removable_index(len, prefix, cfg.front_sample, true);
        else m = pick_removable_index(len, M - len, cfg.removable_sample, false);
        if (m < 0) {
            stats.no_cand_or[len]++;
            return false;
        }

        int head = path[m];
        int tail = path[m + len - 1];
        int hx = head / grid_n, hy = head % grid_n;
        int cand[16], csz = 0;
        for (int d = 0; d < 8; ++d) {
            int vx = hx + (int)dxx[d], vy = hy + (int)dyy[d];
            if (vx < 0 || vx >= grid_n || vy < 0 || vy >= grid_n) continue;
            int t = pos[vx * grid_n + vy];
            if (t >= m - 1 && t <= m + len - 1) continue;
            if (t < m - 1) {
                if (adjacent(path[t], head) && adjacent(tail, path[t + 1])) cand[csz++] = t;
            } else {
                if (adjacent(path[t], head) && (t == M - 1 || adjacent(tail, path[t + 1]))) cand[csz++] = t;
            }
        }
        if (csz == 0) {
            stats.no_cand_or[len]++;
            return false;
        }
        sort(cand, cand + csz);
        csz = (int)(unique(cand, cand + csz) - cand);

        stats.valid_or[len]++;
        stats.cand_or_sum[len] += csz;

        int best_t = cand[(int)(xor128() % (uint32_t)csz)];
        ll best_delta = delta_or_len(m, len, best_t);
        int eval = min(csz, cfg.or_eval);
        for (int e = 1; e < eval; ++e) {
            int t = cand[(int)(xor128() % (uint32_t)csz)];
            ll delta = delta_or_len(m, len, t);
            if (delta > best_delta) {
                best_delta = delta;
                best_t = t;
            }
        }

        bool acc = false;
        if (best_delta >= 0) acc = true;
        else if (exp((double)best_delta / temp) > rand01()) acc = true;
        if (!acc) return false;

        apply_or_len(m, len, best_t, best_delta);
        stats.acc_or[len]++;
        if (best_delta > 0) stats.imp_or[len]++;
        else if (best_delta < 0) stats.worse_or[len]++;
        return true;
    }

    int select_or_len(const SAConfig& cfg) const {
        int sum = cfg.or_len_weight[1] + cfg.or_len_weight[2] + cfg.or_len_weight[3];
        if (sum <= 0) return 1;
        int r = (int)(xor128() % (uint32_t)sum);
        for (int len = 1; len <= 3; ++len) {
            if (r < cfg.or_len_weight[len]) return len;
            r -= cfg.or_len_weight[len];
        }
        return 1;
    }

    void optimize_until(function<double()> elapsed, double end_time, const SAConfig& cfg) {
        if (M <= 1) return;
        while (true) {
            double e = elapsed();
            if (e >= end_time) break;
            double prog = min(1.0, max(0.0, e / end_time));
            double temp = cfg.start_temp * pow(cfg.end_temp / cfg.start_temp, prog);
            stats.iter++;
            int r = (int)(xor128() % 100U);
            if (r < cfg.or_percent) {
                int len = select_or_len(cfg);
                step_or_len(cfg, temp, len);
            } else {
                step_2opt(cfg, temp);
            }
        }
        path = best_path;
        rep(i, M) pos[path[(int)i]] = (int)i;
        cur_score = best_score;
    }
};

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

struct ExpandResult {
    vector<int> child_path;
    ll local_bonus = -LINF;
    bool fallback = false;
};

ExpandResult expand_parent_path_2x2(int parentG, int childG, const vector<ll>& childW, const vector<int>& parent_path) {
    int PM = parentG * parentG;
    assert((int)parent_path.size() == PM);

    auto parent_to_children = [&](int b) {
        int bx = b / parentG, by = b % parentG;
        int x = bx * 2, y = by * 2;
        array<int, 4> c = {
            x * childG + y,
            x * childG + (y + 1),
            (x + 1) * childG + y,
            (x + 1) * childG + (y + 1)
        };
        return c;
    };

    auto adj_child = [&](int a, int b) {
        int ax = a / childG, ay = a % childG;
        int bx = b / childG, by = b % childG;
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
    res.child_path.reserve(childG * childG);
    for (int t = 0; t < PM; ++t) {
        int b = parent_path[t];
        auto c = parent_to_children(b);
        auto ord = best_order[b][chosen_entry[t]][chosen_exit[t]];
        rep(k, 4) res.child_path.push_back(c[ord[(int)k]]);
    }
    return res;
}

pair<int,int> transform_xy(int G, int x, int y, int mode) {
    if (mode & 4) swap(x, y);
    if (mode & 1) x = G - 1 - x;
    if (mode & 2) y = G - 1 - y;
    return {x, y};
}

vector<int> transform_path_coords(int G, const vector<pair<int,int>>& coords, int mode) {
    vector<int> p;
    p.reserve(coords.size());
    for (auto [x, y] : coords) {
        auto [tx, ty] = transform_xy(G, x, y, mode);
        p.push_back(tx * G + ty);
    }
    return p;
}

vector<pair<int,int>> canonical_row_serp_coords(int G) {
    vector<pair<int,int>> c;
    c.reserve(G * G);
    for (int x = 0; x < G; ++x) {
        if ((x & 1) == 0) for (int y = 0; y < G; ++y) c.push_back({x, y});
        else for (int y = G - 1; y >= 0; --y) c.push_back({x, y});
    }
    return c;
}

vector<pair<int,int>> canonical_col_serp_coords(int G) {
    vector<pair<int,int>> c;
    c.reserve(G * G);
    for (int y = 0; y < G; ++y) {
        if ((y & 1) == 0) for (int x = 0; x < G; ++x) c.push_back({x, y});
        else for (int x = G - 1; x >= 0; --x) c.push_back({x, y});
    }
    return c;
}

vector<pair<int,int>> canonical_diag_snake_coords(int G) {
    vector<pair<int,int>> c;
    c.reserve(G * G);
    for (int d = 0; d <= 2 * G - 2; ++d) {
        vector<pair<int,int>> tmp;
        for (int x = max(0, d - (G - 1)); x <= min(G - 1, d); ++x) {
            int y = d - x;
            tmp.push_back({x, y});
        }
        if (d & 1) reverse(rng(tmp));
        for (auto &p : tmp) c.push_back(p);
    }
    return c;
}

vector<pair<int,int>> canonical_spiral_coords(int G) {
    vector<pair<int,int>> c;
    c.reserve(G * G);
    int top = 0, bottom = G - 1, left = 0, right = G - 1;
    while (top <= bottom && left <= right) {
        for (int y = left; y <= right; ++y) c.push_back({top, y});
        ++top;
        for (int x = top; x <= bottom; ++x) c.push_back({x, right});
        --right;
        if (top <= bottom) {
            for (int y = right; y >= left; --y) c.push_back({bottom, y});
            --bottom;
        }
        if (left <= right) {
            for (int x = bottom; x >= top; --x) c.push_back({x, left});
            ++left;
        }
    }
    return c;
}

vector<int> best_family_variant(int G, const vector<ll>& w, const vector<pair<int,int>>& canonical, const vector<int>& modes) {
    ll best = -LINF;
    vector<int> ans;
    for (int mode : modes) {
        vector<int> p = transform_path_coords(G, canonical, mode);
        if (!valid_path_grid(G, p)) continue;
        ll sc = score_of_path(w, p);
        if (sc > best) {
            best = sc;
            ans = p;
        }
    }
    return ans;
}

struct CandidateInfo {
    vector<int> path;
    ll init_score = -LINF;
    ll best_score = -LINF;
};

vector<CandidateInfo> build_block_candidates(int G, const vector<ll>& w) {
    vector<CandidateInfo> res;
    auto rowC = canonical_row_serp_coords(G);
    auto colC = canonical_col_serp_coords(G);
    auto diagC = canonical_diag_snake_coords(G);
    auto spiralC = canonical_spiral_coords(G);

    vector<int> row_best = best_family_variant(G, w, rowC, {0,1,2,3});
    vector<int> col_best = best_family_variant(G, w, colC, {0,1,2,3});
    vector<int> diag_best = best_family_variant(G, w, diagC, {0,1,2,3,4,5,6,7});
    vector<int> spiral_best = best_family_variant(G, w, spiralC, {0,1,2,3,4,5,6,7});

    vector<vector<int>> cand_paths;
    cand_paths.push_back(row_best);
    cand_paths.push_back(reversed_path(row_best));
    cand_paths.push_back(col_best);
    cand_paths.push_back(reversed_path(col_best));
    cand_paths.push_back(diag_best);
    cand_paths.push_back(reversed_path(diag_best));
    cand_paths.push_back(spiral_best);

    for (auto &p : cand_paths) {
        CandidateInfo ci;
        ci.path = p;
        ci.init_score = score_of_path(w, p);
        ci.best_score = ci.init_score;
        res.push_back(ci);
    }
    return res;
}

struct ScreenResult {
    vector<CandidateInfo> infos;
    int selected_id = -1;
    vector<int> best_path;
    ll screen_best_score = -LINF;
};

ScreenResult screen_block_candidates(int G, const vector<ll>& w, const vector<CandidateInfo>& base,
                                     function<double()> elapsed, double screen_end, const SAConfig& cfg) {
    ScreenResult sr;
    sr.infos = base;
    int C = (int)base.size();
    for (int i = 0; i < C; ++i) {
        double target = screen_end * (double)(i + 1) / (double)C;
        PathOptimizer opt;
        opt.init(G, w, sr.infos[i].path);
        opt.optimize_until(elapsed, target, cfg);
        sr.infos[i].best_score = opt.best_score;
        sr.infos[i].path = opt.best_path;
        if (opt.best_score > sr.screen_best_score) {
            sr.screen_best_score = opt.best_score;
            sr.best_path = opt.best_path;
            sr.selected_id = i;
        }
    }
    return sr;
}

int count_valid(const vector<int>& p, int G) { return valid_path_grid(G, p) ? 1 : 0; }

void solve()
{
    auto start_time = chrono::high_resolution_clock::now();
    auto elapsed = [&]() -> double {
        auto now = chrono::high_resolution_clock::now();
        return chrono::duration<double>(now - start_time).count();
    };

    const double TIME_LIMIT = 2.86;
    const double BLOCK_SCREEN_END = 0.18;
    const double BLOCK_END = 0.52;

    int N;
    if (!(cin >> N)) return;
    vector<int> A(N * N);
    rep(i, N) rep(j, N) cin >> A[(int)i * N + (int)j];
    vector<ll> cellW(A.begin(), A.end());

    int G2 = N / 2;
    vector<ll> W2 = aggregate_weights_cells(N, A, 2);

    vector<CandidateInfo> base_cands = build_block_candidates(G2, W2);
    SAConfig cfg_block_screen;
    cfg_block_screen.start_temp = 8.0e7;
    cfg_block_screen.end_temp = 4.0e3;
    cfg_block_screen.bad_sample = 12;
    cfg_block_screen.removable_sample = 10;
    cfg_block_screen.front_sample = 14;
    cfg_block_screen.move2_eval = 2;
    cfg_block_screen.or_eval = 2;
    cfg_block_screen.or_percent = 62;
    cfg_block_screen.front_prefix_percent = 62;
    cfg_block_screen.or_len_weight = {0, 34, 38, 28};

    ScreenResult screen = screen_block_candidates(G2, W2, base_cands, elapsed, BLOCK_SCREEN_END, cfg_block_screen);

    PathOptimizer opt2;
    opt2.init(G2, W2, screen.best_path);
    SAConfig cfg2;
    cfg2.start_temp = 5.0e7;
    cfg2.end_temp = 2.5e3;
    cfg2.bad_sample = 14;
    cfg2.removable_sample = 12;
    cfg2.front_sample = 18;
    cfg2.move2_eval = 3;
    cfg2.or_eval = 3;
    cfg2.or_percent = 62;
    cfg2.front_prefix_percent = 60;
    cfg2.or_len_weight = {0, 34, 38, 28};
    opt2.optimize_until(elapsed, BLOCK_END, cfg2);

    ExpandResult exp21 = expand_parent_path_2x2(G2, N, cellW, opt2.best_path);
    if (exp21.fallback || !valid_path_grid(N, exp21.child_path)) {
        exp21.fallback = true;
        auto coords = canonical_row_serp_coords(N);
        exp21.child_path = transform_path_coords(N, coords, 0);
        exp21.local_bonus = score_of_path(cellW, exp21.child_path);
    }

    auto ser1 = transform_path_coords(N, canonical_row_serp_coords(N), 0);
    vector<vector<int>> cell_inits = {
        exp21.child_path,
        reversed_path(exp21.child_path),
        ser1,
        reversed_path(ser1)
    };
    ll best_init_score = -LINF;
    vector<int> stage1_init;
    for (auto &p : cell_inits) {
        if (!valid_path_grid(N, p)) continue;
        ll sc = score_of_path(cellW, p);
        if (sc > best_init_score) {
            best_init_score = sc;
            stage1_init = p;
        }
    }
    if (stage1_init.empty()) stage1_init = exp21.child_path;

    PathOptimizer opt1;
    opt1.init(N, cellW, stage1_init);
    SAConfig cfg1;
    cfg1.start_temp = 3.0e7;
    cfg1.end_temp = 2.0e3;
    cfg1.bad_sample = 16;
    cfg1.removable_sample = 12;
    cfg1.front_sample = 20;
    cfg1.move2_eval = 4;
    cfg1.or_eval = 4;
    cfg1.or_percent = 34;
    cfg1.front_prefix_percent = 76;
    cfg1.or_len_weight = {0, 100, 0, 0};
    opt1.optimize_until(elapsed, TIME_LIMIT, cfg1);

#ifdef LOCAL
    cerr << fixed << setprecision(6);
    cerr << "block_init_score=" << base_cands[screen.selected_id].init_score << "\n";
    cerr << "block_screen_best=" << screen.screen_best_score << "\n";
    cerr << "block_best_score=" << opt2.best_score << "\n";
    cerr << "expand_bonus=" << exp21.local_bonus << "\n";
    cerr << "cell_init_score=" << score_of_path(cellW, stage1_init) << "\n";
    cerr << "cell_best_score=" << opt1.best_score << " elapsed=" << elapsed() << "\n";
    cerr << "[block_candidates] count=" << (int)screen.infos.size() << " selected=" << screen.selected_id << "\n";
    for (int i = 0; i < (int)screen.infos.size(); ++i) {
        cerr << "  id=" << i
             << " init=" << base_cands[i].init_score
             << " best=" << screen.infos[i].best_score
             << " gain=" << (screen.infos[i].best_score - base_cands[i].init_score)
             << "\n";
    }
    auto dump_block_stats = [&](const SAStats& s) {
        long long acc_or = s.acc_or[1] + s.acc_or[2] + s.acc_or[3];
        long long tried_or = s.tried_or[1] + s.tried_or[2] + s.tried_or[3];
        long long imp_or = s.imp_or[1] + s.imp_or[2] + s.imp_or[3];
        cerr << "[block_sa] iter=" << s.iter
             << " 2opt=" << s.acc2 << '/' << s.tried2
             << " imp2=" << s.imp2
             << " or=" << acc_or << '/' << tried_or
             << " imp_or=" << imp_or
             << " len_acc={" << s.acc_or[1] << ',' << s.acc_or[2] << ',' << s.acc_or[3] << '}'
             << " len_imp={" << s.imp_or[1] << ',' << s.imp_or[2] << ',' << s.imp_or[3] << '}'
             << " removable_fail=" << s.removable_fail << '\n';
    };
    auto dump_cell_stats = [&](const SAStats& s) {
        long long acc_or = s.acc_or[1] + s.acc_or[2] + s.acc_or[3];
        long long tried_or = s.tried_or[1] + s.tried_or[2] + s.tried_or[3];
        long long imp_or = s.imp_or[1] + s.imp_or[2] + s.imp_or[3];
        cerr << "[cell_sa] iter=" << s.iter
             << " 2opt=" << s.acc2 << '/' << s.tried2
             << " imp2=" << s.imp2
             << " or1=" << acc_or << '/' << tried_or
             << " imp1=" << imp_or
             << " removable_fail=" << s.removable_fail << '\n';
    };
    dump_block_stats(opt2.stats);
    dump_cell_stats(opt1.stats);
    cerr << "params: TIME_LIMIT=" << TIME_LIMIT
         << " BLOCK_SCREEN_END=" << BLOCK_SCREEN_END
         << " BLOCK_END=" << BLOCK_END
         << " block_or_len_weight={" << cfg2.or_len_weight[1] << ',' << cfg2.or_len_weight[2] << ',' << cfg2.or_len_weight[3] << '}'
         << " block_or_percent=" << cfg2.or_percent
         << " cell_or_percent=" << cfg1.or_percent << "\n";
    cerr << "valid=" << count_valid(opt1.best_path, N) << "\n";
#endif

    for (int v : opt1.best_path) {
        cout << v / N << ' ' << v % N << '\n';
    }
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


/*

gpt1-imm

*/