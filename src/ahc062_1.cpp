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
#if __has_include(<atcoder/all>)
#include <atcoder/all>
#else
namespace atcoder {
struct modint998244353 {};
}
#endif
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

struct MoveStats {
    long long tried = 0;
    long long valid = 0;
    long long accepted = 0;
    long long improved = 0;
    long long worsened = 0;
    long long no_candidate = 0;
    long long cand_sum = 0;
};

struct SAStats {
    long long iter = 0;
    MoveStats two_opt;
    array<MoveStats, 5> ork;
    MoveStats repair;
    long long bad_pick_removable_fail = 0;
    long long repair_no_improve = 0;
};

struct SAConfig {
    double start_temp = 1e7;
    double end_temp = 1e3;
    int bad_sample = 8;
    int removable_sample = 8;
    int front_sample = 8;
    int move2_eval = 2;
    array<int, 5> ork_eval = {0, 2, 2, 2, 2};
    int front_prefix_percent = 70;
    int repair_prefix_percent = 82;
    int repair_sample = 8;
    int repair_min_len = 6;
    int repair_max_len = 8;
    int or1_percent = 20;
    int or2_percent = 12;
    int or3_percent = 8;
    int or4_percent = 5;
    int repair_percent = 5;
};

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

    inline bool removable_k(int idx, int K) const {
        if (idx < 0 || idx + K > M) return false;
        if (idx == 0 || idx + K == M) return true;
        return adjacent(path[idx - 1], path[idx + K]);
    }

    inline ll segment_sum(int idx, int K) const {
        ll s = 0;
        for (int i = 0; i < K; ++i) s += w[path[idx + i]];
        return s;
    }

    inline ll segment_badness(int idx, int K) const {
        ll s = 0;
        for (int i = 0; i < K; ++i) s += badness_idx(idx + i);
        return s;
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

    int pick_bad_removable_k(int sample, int K) {
        if (M < K) return -1;
        int best = -1;
        ll bestv = -1;
        for (int s = 0; s < sample; ++s) {
            int idx = (int)(xor128() % (uint32_t)(M - K + 1));
            if (!removable_k(idx, K)) continue;
            ll v = segment_badness(idx, K);
            if (best == -1 || v > bestv) bestv = v, best = idx;
        }
        if (best == -1) stats.bad_pick_removable_fail++;
        return best;
    }

    int pick_front_removable_k(int prefix_limit, int sample, int K) {
        if (M < K) return -1;
        prefix_limit = max(0, min(M - K, prefix_limit));
        int best = -1;
        ll bestv = -LINF;
        for (int s = 0; s < sample; ++s) {
            int idx = (int)(xor128() % (uint32_t)(prefix_limit + 1));
            if (!removable_k(idx, K)) continue;
            ll v = segment_sum(idx, K);
            if (best == -1 || v > bestv) bestv = v, best = idx;
        }
        if (best == -1) return pick_bad_removable_k(sample, K);
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
        auto& st = stats.two_opt;
        st.tried++;
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
            st.no_candidate++;
            return false;
        }

        st.valid++;
        st.cand_sum += csz;

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
        st.accepted++;
        if (best_delta > 0) st.improved++;
        else if (best_delta < 0) st.worsened++;
        return true;
    }

    ll delta_or_k(int m, int t, int K) const {
        ll seg = segment_sum(m, K);
        ll sum = 0;
        if (t < m - 1) {
            for (int k = t + 1; k <= m - 1; ++k) sum += w[path[k]];
            return (ll)K * sum + (ll)(t + 1 - m) * seg;
        } else if (t > m + K - 1) {
            for (int k = m + K; k <= t; ++k) sum += w[path[k]];
            return (ll)(t - m - K + 1) * seg - (ll)K * sum;
        }
        return -LINF;
    }

    void apply_or_k(int m, int t, int K, ll delta) {
        vector<int> seg(K);
        for (int i = 0; i < K; ++i) seg[i] = path[m + i];
        if (t < m - 1) {
            for (int k = m - 1; k >= t + 1; --k) {
                path[k + K] = path[k];
                pos[path[k + K]] = k + K;
            }
            for (int i = 0; i < K; ++i) {
                path[t + 1 + i] = seg[i];
                pos[seg[i]] = t + 1 + i;
            }
        } else {
            for (int k = m; k + K <= t; ++k) {
                path[k] = path[k + K];
                pos[path[k]] = k;
            }
            int start = t - K + 1;
            for (int i = 0; i < K; ++i) {
                path[start + i] = seg[i];
                pos[seg[i]] = start + i;
            }
        }
        cur_score += delta;
        if (cur_score > best_score) {
            best_score = cur_score;
            best_path = path;
        }
    }

    bool valid_insert_position(int m, int K, int t) const {
        if (t >= m - 1 && t <= m + K - 1) return false;
        int first = path[m];
        int last = path[m + K - 1];
        if (t < m - 1) {
            if (t >= 0 && !adjacent(path[t], first)) return false;
            if (!adjacent(last, path[t + 1])) return false;
            return true;
        } else {
            if (!adjacent(path[t], first)) return false;
            if (t + 1 < M && !adjacent(last, path[t + 1])) return false;
            return true;
        }
    }

    bool step_or_k(const SAConfig& cfg, double temp, int K) {
        if (M < K + 1) return false;
        auto& st = stats.ork[K];
        st.tried++;
        int prefix = max(0, min(M - K, (int)((long long)M * cfg.front_prefix_percent / 100LL)));
        int m;
        if ((int)(xor128() % 100U) < 72) m = pick_front_removable_k(prefix, cfg.front_sample, K);
        else m = pick_bad_removable_k(cfg.removable_sample, K);
        if (m < 0) {
            st.no_candidate++;
            return false;
        }

        int first = path[m], last = path[m + K - 1];
        int fx = first / grid_n, fy = first % grid_n;
        int lx = last / grid_n, ly = last % grid_n;
        vector<int> cand;
        cand.reserve(24);

        for (int d = 0; d < 8; ++d) {
            int vx = fx + (int)dxx[d], vy = fy + (int)dyy[d];
            if (vx < 0 || vx >= grid_n || vy < 0 || vy >= grid_n) continue;
            int t = pos[vx * grid_n + vy];
            if (valid_insert_position(m, K, t)) cand.push_back(t);
        }
        for (int d = 0; d < 8; ++d) {
            int vx = lx + (int)dxx[d], vy = ly + (int)dyy[d];
            if (vx < 0 || vx >= grid_n || vy < 0 || vy >= grid_n) continue;
            int idx = pos[vx * grid_n + vy];
            int t = idx - 1;
            if (t < 0) {
                if (m > 0 && adjacent(last, path[0])) cand.push_back(-1);
            } else if (valid_insert_position(m, K, t)) {
                cand.push_back(t);
            }
        }
        if (m > 0 && adjacent(last, path[0])) cand.push_back(-1);
        if (m + K < M && adjacent(path[M - 1], first)) cand.push_back(M - 1);

        sort(rng(cand));
        cand.erase(unique(rng(cand)), cand.end());
        if (cand.empty()) {
            st.no_candidate++;
            return false;
        }

        st.valid++;
        st.cand_sum += (long long)cand.size();

        int best_t = cand[(int)(xor128() % (uint32_t)cand.size())];
        ll best_delta = delta_or_k(m, best_t, K);
        int eval = min((int)cand.size(), cfg.ork_eval[K]);
        for (int e = 1; e < eval; ++e) {
            int t = cand[(int)(xor128() % (uint32_t)cand.size())];
            ll delta = delta_or_k(m, t, K);
            if (delta > best_delta) {
                best_delta = delta;
                best_t = t;
            }
        }

        bool acc = false;
        if (best_delta >= 0) acc = true;
        else if (exp((double)best_delta / temp) > rand01()) acc = true;
        if (!acc) return false;

        apply_or_k(m, best_t, K, best_delta);
        st.accepted++;
        if (best_delta > 0) st.improved++;
        else if (best_delta < 0) st.worsened++;
        return true;
    }

    int pick_repair_start(int max_start, int sample, int K) const {
        max_start = max(0, min(M - K, max_start));
        int best = (int)(xor128() % (uint32_t)(max_start + 1));
        ll bestv = segment_badness(best, K);
        for (int s = 1; s < sample; ++s) {
            int idx = (int)(xor128() % (uint32_t)(max_start + 1));
            ll v = segment_badness(idx, K);
            if (v > bestv) bestv = v, best = idx;
        }
        return best;
    }

    bool step_repair(const SAConfig& cfg) {
        if (M <= 3) return false;
        auto& st = stats.repair;
        st.tried++;
        int minK = max(2, cfg.repair_min_len);
        int maxK = min(cfg.repair_max_len, M);
        if (minK > maxK) {
            st.no_candidate++;
            return false;
        }
        int K = minK;
        if (maxK > minK) K += (int)(xor128() % (uint32_t)(maxK - minK + 1));
        int prefix = max(0, min(M - K, (int)((long long)M * cfg.repair_prefix_percent / 100LL)));
        int l = pick_repair_start(prefix, cfg.repair_sample, K);
        int r = l + K - 1;

        vector<int> seg(K);
        for (int i = 0; i < K; ++i) seg[i] = path[l + i];
        int left = (l > 0 ? path[l - 1] : -1);
        int right = (r + 1 < M ? path[r + 1] : -1);

        vector<char> okL(K, 0), okR(K, 0);
        vector<vector<char>> adjm(K, vector<char>(K, 0));
        for (int i = 0; i < K; ++i) {
            okL[i] = (left == -1 || adjacent(left, seg[i]));
            okR[i] = (right == -1 || adjacent(seg[i], right));
            for (int j = 0; j < K; ++j) if (i != j) adjm[i][j] = adjacent(seg[i], seg[j]);
        }

        const ll NEG = -LINF / 4;
        int SZ = 1 << K;
        vector<vector<ll>> dp(SZ, vector<ll>(K, NEG));
        vector<vector<short>> prv(SZ, vector<short>(K, -1));
        for (int i = 0; i < K; ++i) {
            if (!okL[i]) continue;
            dp[1 << i][i] = (ll)l * w[seg[i]];
        }
        for (int mask = 1; mask < SZ; ++mask) {
            int cnt = popcount((unsigned)mask);
            for (int last = 0; last < K; ++last) {
                if (dp[mask][last] <= NEG / 2) continue;
                for (int nxt = 0; nxt < K; ++nxt) {
                    if ((mask >> nxt) & 1) continue;
                    if (!adjm[last][nxt]) continue;
                    int nmask = mask | (1 << nxt);
                    ll val = dp[mask][last] + (ll)(l + cnt) * w[seg[nxt]];
                    if (val > dp[nmask][nxt]) {
                        dp[nmask][nxt] = val;
                        prv[nmask][nxt] = (short)last;
                    }
                }
            }
        }

        ll cur_local = 0;
        for (int i = 0; i < K; ++i) cur_local += (ll)(l + i) * w[path[l + i]];

        ll best_local = NEG;
        int last_idx = -1;
        int full = SZ - 1;
        for (int i = 0; i < K; ++i) {
            if (!okR[i]) continue;
            if (dp[full][i] > best_local) {
                best_local = dp[full][i];
                last_idx = i;
            }
        }
        if (last_idx == -1) {
            st.no_candidate++;
            return false;
        }

        st.valid++;
        st.cand_sum += 1;
        ll delta = best_local - cur_local;
        if (delta <= 0) {
            stats.repair_no_improve++;
            return false;
        }

        vector<int> best_order(K);
        int mask = full;
        int cur = last_idx;
        for (int i = K - 1; i >= 0; --i) {
            best_order[i] = seg[cur];
            int p = prv[mask][cur];
            mask ^= 1 << cur;
            cur = p;
        }

        for (int i = 0; i < K; ++i) {
            path[l + i] = best_order[i];
            pos[best_order[i]] = l + i;
        }
        cur_score += delta;
        if (cur_score > best_score) {
            best_score = cur_score;
            best_path = path;
        }
        st.accepted++;
        st.improved++;
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
            if (r < cfg.repair_percent) step_repair(cfg);
            else if (r < cfg.repair_percent + cfg.or4_percent) step_or_k(cfg, temp, 4);
            else if (r < cfg.repair_percent + cfg.or4_percent + cfg.or3_percent) step_or_k(cfg, temp, 3);
            else if (r < cfg.repair_percent + cfg.or4_percent + cfg.or3_percent + cfg.or2_percent) step_or_k(cfg, temp, 2);
            else if (r < cfg.repair_percent + cfg.or4_percent + cfg.or3_percent + cfg.or2_percent + cfg.or1_percent) step_or_k(cfg, temp, 1);
            else step_2opt(cfg, temp);
        }
        path = best_path;
        rep(i, M) pos[path[(int)i]] = (int)i;
        cur_score = best_score;
    }
};

vector<int> best_serpentine_path(int G, const vector<ll>& w) {
    int M = G * G;
    vector<int> best(M), cand(M);
    ll best_sc = -LINF;
    for (int dir = 0; dir < 2; ++dir) {
        for (int sr = 0; sr < 2; ++sr) {
            for (int sc = 0; sc < 2; ++sc) {
                int idx = 0;
                if (dir == 0) {
                    for (int i = 0; i < G; ++i) {
                        int r = (sr == 0 ? i : G - 1 - i);
                        for (int j = 0; j < G; ++j) {
                            int c = (sc == 0 ? j : G - 1 - j);
                            if (i & 1) c = G - 1 - c;
                            cand[idx++] = r * G + c;
                        }
                    }
                } else {
                    for (int j = 0; j < G; ++j) {
                        int c = (sc == 0 ? j : G - 1 - j);
                        for (int i = 0; i < G; ++i) {
                            int r = (sr == 0 ? i : G - 1 - i);
                            if (j & 1) r = G - 1 - r;
                            cand[idx++] = r * G + c;
                        }
                    }
                }
                ll scv = score_of_path(w, cand);
                if (scv > best_sc) {
                    best_sc = scv;
                    best = cand;
                }
            }
        }
    }
    return best;
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
        res.child_path = best_serpentine_path(childG, childW);
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
    res.child_path.reserve(childG * childG);
    for (int t = 0; t < PM; ++t) {
        int b = parent_path[t];
        auto c = parent_to_children(b);
        auto ord = best_order[b][chosen_entry[t]][chosen_exit[t]];
        rep(k, 4) res.child_path.push_back(c[ord[(int)k]]);
    }
    return res;
}

vector<int> choose_best_init(const vector<ll>& w, const vector<vector<int>>& cands, int G) {
    ll best = -LINF;
    vector<int> ans;
    for (const auto& p : cands) {
        if ((int)p.size() != G * G) continue;
        if (!valid_path_grid(G, p)) continue;
        ll s = score_of_path(w, p);
        if (s > best) {
            best = s;
            ans = p;
        }
    }
    if (ans.empty()) ans = best_serpentine_path(G, w);
    return ans;
}

void solve()
{
    auto start_time = chrono::high_resolution_clock::now();
    auto elapsed = [&]() -> double {
        auto now = chrono::high_resolution_clock::now();
        return chrono::duration<double>(now - start_time).count();
    };

    const double TIME_LIMIT = 2.86;
    const double STAGE8_END = 0.05;
    const double STAGE4_END = 0.24;
    const double STAGE2_END = 0.72;

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

    vector<int> ser8 = best_serpentine_path(G8, W8);
    vector<int> stage8_init = choose_best_init(W8, {ser8, reversed_path(ser8)}, G8);

    PathOptimizer opt8;
    opt8.init(G8, W8, stage8_init);
    SAConfig cfg8;
    cfg8.start_temp = 1.0e8;
    cfg8.end_temp = 8.0e3;
    cfg8.bad_sample = 10;
    cfg8.removable_sample = 8;
    cfg8.front_sample = 12;
    cfg8.move2_eval = 3;
    cfg8.ork_eval = {0, 3, 3, 2, 2};
    cfg8.front_prefix_percent = 68;
    cfg8.repair_prefix_percent = 75;
    cfg8.repair_sample = 6;
    cfg8.repair_min_len = 5;
    cfg8.repair_max_len = 6;
    cfg8.or1_percent = 18;
    cfg8.or2_percent = 10;
    cfg8.or3_percent = 5;
    cfg8.or4_percent = 2;
    cfg8.repair_percent = 2;
    opt8.optimize_until(elapsed, STAGE8_END, cfg8);

    ExpandResult exp84 = expand_parent_path_2x2(G8, G4, W4, opt8.best_path);
    vector<int> ser4 = best_serpentine_path(G4, W4);
    vector<int> stage4_init = choose_best_init(
        W4,
        {exp84.child_path, reversed_path(exp84.child_path), ser4, reversed_path(ser4)},
        G4
    );

    PathOptimizer opt4;
    opt4.init(G4, W4, stage4_init);
    SAConfig cfg4;
    cfg4.start_temp = 8.0e7;
    cfg4.end_temp = 5.0e3;
    cfg4.bad_sample = 12;
    cfg4.removable_sample = 10;
    cfg4.front_sample = 16;
    cfg4.move2_eval = 3;
    cfg4.ork_eval = {0, 3, 3, 3, 2};
    cfg4.front_prefix_percent = 72;
    cfg4.repair_prefix_percent = 80;
    cfg4.repair_sample = 8;
    cfg4.repair_min_len = 6;
    cfg4.repair_max_len = 7;
    cfg4.or1_percent = 20;
    cfg4.or2_percent = 12;
    cfg4.or3_percent = 8;
    cfg4.or4_percent = 4;
    cfg4.repair_percent = 4;
    opt4.optimize_until(elapsed, STAGE4_END, cfg4);

    ExpandResult exp42 = expand_parent_path_2x2(G4, G2, W2, opt4.best_path);
    vector<int> ser2 = best_serpentine_path(G2, W2);
    vector<int> stage2_init = choose_best_init(
        W2,
        {exp42.child_path, reversed_path(exp42.child_path), ser2, reversed_path(ser2)},
        G2
    );

    PathOptimizer opt2;
    opt2.init(G2, W2, stage2_init);
    SAConfig cfg2;
    cfg2.start_temp = 5.0e7;
    cfg2.end_temp = 3.0e3;
    cfg2.bad_sample = 14;
    cfg2.removable_sample = 12;
    cfg2.front_sample = 18;
    cfg2.move2_eval = 4;
    cfg2.ork_eval = {0, 4, 4, 3, 3};
    cfg2.front_prefix_percent = 74;
    cfg2.repair_prefix_percent = 83;
    cfg2.repair_sample = 10;
    cfg2.repair_min_len = 6;
    cfg2.repair_max_len = 8;
    cfg2.or1_percent = 22;
    cfg2.or2_percent = 14;
    cfg2.or3_percent = 9;
    cfg2.or4_percent = 5;
    cfg2.repair_percent = 5;
    opt2.optimize_until(elapsed, STAGE2_END, cfg2);

    ExpandResult exp21 = expand_parent_path_2x2(G2, N, cellW, opt2.best_path);
    vector<int> ser1 = best_serpentine_path(N, cellW);
    vector<int> stage1_init = choose_best_init(
        cellW,
        {exp21.child_path, reversed_path(exp21.child_path), ser1, reversed_path(ser1)},
        N
    );

    PathOptimizer opt1;
    opt1.init(N, cellW, stage1_init);
    SAConfig cfg1;
    cfg1.start_temp = 3.0e7;
    cfg1.end_temp = 2.0e3;
    cfg1.bad_sample = 16;
    cfg1.removable_sample = 12;
    cfg1.front_sample = 20;
    cfg1.move2_eval = 4;
    cfg1.ork_eval = {0, 4, 4, 4, 3};
    cfg1.front_prefix_percent = 78;
    cfg1.repair_prefix_percent = 86;
    cfg1.repair_sample = 12;
    cfg1.repair_min_len = 6;
    cfg1.repair_max_len = 8;
    cfg1.or1_percent = 18;
    cfg1.or2_percent = 14;
    cfg1.or3_percent = 11;
    cfg1.or4_percent = 7;
    cfg1.repair_percent = 8;
    opt1.optimize_until(elapsed, TIME_LIMIT, cfg1);

#ifdef LOCAL
    cerr << fixed << setprecision(6);
    cerr << "time_stage8_end=" << STAGE8_END
         << " time_stage4_end=" << STAGE4_END
         << " time_stage2_end=" << STAGE2_END
         << " time_limit=" << TIME_LIMIT << "\n";

    cerr << "stage8_init_score=" << score_of_path(W8, stage8_init)
         << " stage8_best_score=" << opt8.best_score << "\n";
    cerr << "expand8to4_bonus=" << exp84.local_bonus
         << " fallback=" << exp84.fallback
         << " expand_score=" << score_of_path(W4, exp84.child_path) << "\n";
    cerr << "stage4_init_score=" << score_of_path(W4, stage4_init)
         << " stage4_best_score=" << opt4.best_score << "\n";
    cerr << "expand4to2_bonus=" << exp42.local_bonus
         << " fallback=" << exp42.fallback
         << " expand_score=" << score_of_path(W2, exp42.child_path) << "\n";
    cerr << "stage2_init_score=" << score_of_path(W2, stage2_init)
         << " stage2_best_score=" << opt2.best_score << "\n";
    cerr << "expand2to1_bonus=" << exp21.local_bonus
         << " fallback=" << exp21.fallback
         << " expand_score=" << score_of_path(cellW, exp21.child_path) << "\n";
    cerr << "stage1_init_score=" << score_of_path(cellW, stage1_init)
         << " stage1_best_score=" << opt1.best_score
         << " elapsed=" << elapsed() << "\n";

    auto dump_move = [&](const string& name, const MoveStats& s) {
        cerr << name
             << " acc=" << s.accepted << '/' << s.tried
             << " valid=" << s.valid
             << " imp=" << s.improved
             << " worse=" << s.worsened
             << " no=" << s.no_candidate
             << " avgCand=" << (s.valid ? (double)s.cand_sum / s.valid : 0.0);
    };

    auto dump_stats = [&](const string& name, const SAStats& s) {
        cerr << '[' << name << "] iter=" << s.iter << ' ';
        dump_move("2opt", s.two_opt); cerr << ' ';
        dump_move("or1", s.ork[1]); cerr << ' ';
        dump_move("or2", s.ork[2]); cerr << ' ';
        dump_move("or3", s.ork[3]); cerr << ' ';
        dump_move("or4", s.ork[4]); cerr << ' ';
        dump_move("repair", s.repair);
        cerr << " repair_no_improve=" << s.repair_no_improve
             << " removable_fail=" << s.bad_pick_removable_fail << '\n';
    };

    dump_stats("stage8", opt8.stats);
    dump_stats("stage4", opt4.stats);
    dump_stats("stage2", opt2.stats);
    dump_stats("stage1", opt1.stats);

    cerr << "valid_stage8=" << valid_path_grid(G8, opt8.best_path)
         << " valid_stage4=" << valid_path_grid(G4, opt4.best_path)
         << " valid_stage2=" << valid_path_grid(G2, opt2.best_path)
         << " valid_stage1=" << valid_path_grid(N, opt1.best_path) << "\n";
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