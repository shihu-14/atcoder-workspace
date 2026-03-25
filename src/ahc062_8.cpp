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

struct MoveStats {
    long long tried = 0, valid = 0, accepted = 0, improved = 0;
    long long accepted_non_improve = 0, accepted_negative = 0;
    long long invalid_bridge = 0, invalid_no_candidate = 0;
    long long candidate_sum = 0, eval_sum = 0;
    long long accepted_span_sum = 0, accepted_delta_sum = 0, improved_delta_sum = 0;

    array<long long, 3> tried_phase{}, valid_phase{}, accepted_phase{}, improved_phase{}, negative_phase{};
    array<long long, 3> invalid_bridge_phase{}, invalid_no_candidate_phase{};
    array<long long, 3> accepted_delta_phase{}, improved_delta_phase{};

    // 8 bins: <=1, <=2, <=4, <=8, <=16, <=32, <=64, >64
    array<long long, 8> accepted_span_bucket{}, improved_span_bucket{};

    // final mode stats: 0=free, 1=rem_front, 2=rem_bad
    array<long long, 3> valid_mode{}, accepted_mode{}, improved_mode{};
    array<long long, 3> accepted_delta_mode{}, improved_delta_mode{}, accepted_span_mode{};
    array<array<long long, 3>, 3> accepted_phase_mode{}, improved_phase_mode{};
    array<array<long long, 3>, 3> improved_delta_phase_mode{};
    array<array<long long, 8>, 3> accepted_span_bucket_mode{}, improved_span_bucket_mode{};

    // primary choice counts per invocation
    array<long long, 3> primary_tried{};

    // attempt calls per mode inside proposer (including fallback)
    array<long long, 3> mode_attempt_calls{};
    array<long long, 3> mode_attempt_valid{};
    array<long long, 3> mode_attempt_invalid_bridge{};
    array<long long, 3> mode_attempt_invalid_no_candidate{};

    long long fallback_used = 0;
    long long fallback_free_to_rem_front = 0;
    long long fallback_free_to_rem_bad = 0;
    long long fallback_rem_front_to_rem_bad = 0;
    long long fallback_rem_bad_to_rem_front = 0;

    // relax level used for final selected move: 0 strict,1 half,2 zero
    array<long long, 3> relax_used{};
    array<array<long long, 3>, 3> relax_used_mode{};
};

struct Move2Opt {
    int u_idx = -1;
    int j = -1;
    int candidate_count = 0;
    int eval_count = 0;
    ll delta = 0;
    bool valid = false;
};

struct MoveOr1 {
    int m = -1;
    int t = -1;
    int candidate_count = 0;
    int eval_count = 0;
    ll delta = 0;
    bool valid = false;
    bool bridge_ok = true;
    int source_mode = 0;   // final mode: 0 free, 1 rem_front, 2 rem_bad
    int primary_mode = 0;  // primary chosen at invocation
    int relax_level = 0;   // 0 strict, 1 half, 2 zero
    bool used_fallback = false;
};

int span_bucket8(int span) {
    if (span <= 1) return 0;
    if (span <= 2) return 1;
    if (span <= 4) return 2;
    if (span <= 8) return 3;
    if (span <= 16) return 4;
    if (span <= 32) return 5;
    if (span <= 64) return 6;
    return 7;
}

void solve()
{
    auto start_time = chrono::high_resolution_clock::now();

    const double TIME_LIMIT = 2.92;
    const double START_TEMP = 4.0e7;
    const double END_TEMP = 3.0e3;
    const int TEMP_UPDATE_MASK = 1023;

    const int BAD_PICK_SAMPLE = 16;
    const int REMOVABLE_PICK_SAMPLE = 12;
    const int FREE_HEAVY_SAMPLE = 20;
    const int REMOVABLE_HEAVY_SAMPLE = 16;

    const int MOVE2_EVAL_EARLY = 2;
    const int MOVE2_EVAL_MID = 3;
    const int MOVE2_EVAL_LATE = 4;

    const int OR1_EVAL_EARLY = 5;
    const int OR1_EVAL_MID = 4;
    const int OR1_EVAL_LATE = 3;

    const int OR1_PERCENT_EARLY = 42;
    const int OR1_PERCENT_MID = 34;
    const int OR1_PERCENT_LATE = 18;

    const int OR1_REMOVABLE_PERCENT_EARLY = 60;
    const int OR1_REMOVABLE_PERCENT_MID = 75;
    const int OR1_REMOVABLE_PERCENT_LATE = 90;

    const int FREE_MIN_SPAN_EARLY = 128;
    const int FREE_MIN_SPAN_MID = 64;
    const int FREE_MIN_SPAN_LATE = 32;

    const int REM_MIN_SPAN_EARLY = 32;
    const int REM_MIN_SPAN_MID = 16;
    const int REM_MIN_SPAN_LATE = 0;

    const int REM_FRONT_PERCENT_EARLY = 60;
    const int REM_FRONT_PERCENT_MID = 40;
    const int REM_FRONT_PERCENT_LATE = 20;

    int N;
    if (!(cin >> N)) return;
    const int M = N * N;

    vector<int> A(M);
    rep(i, N) rep(j, N) cin >> A[(int)i * N + (int)j];

    auto id = [&](int x, int y) -> int { return x * N + y; };
    auto adjacent = [&](int a, int b) -> bool {
        int ax = a / N, ay = a % N;
        int bx = b / N, by = b % N;
        return a != b && max(abs(ax - bx), abs(ay - by)) <= 1;
    };
    auto score_of = [&](const vector<int>& p) -> ll {
        ll s = 0;
        rep(k, M) s += (ll)k * (ll)A[p[(int)k]];
        return s;
    };
    auto phase_id = [&](double elapsed) -> int {
        double p = elapsed / TIME_LIMIT;
        if (p < 1.0 / 3.0) return 0;
        if (p < 2.0 / 3.0) return 1;
        return 2;
    };

    vector<int> path(M), best_path(M), pos(M);
    ll current_score = -LINF, best_score = -LINF, initial_score = -LINF;

    auto build_initial = [&]() {
        vector<int> cand(M), best_init(M);
        ll best_init_score = -LINF;
        for (int dir = 0; dir < 2; ++dir) {
            for (int start_r = 0; start_r < 2; ++start_r) {
                for (int start_c = 0; start_c < 2; ++start_c) {
                    int idx = 0;
                    if (dir == 0) {
                        for (int i = 0; i < N; ++i) {
                            int r = (start_r == 0 ? i : N - 1 - i);
                            for (int j = 0; j < N; ++j) {
                                int c = (start_c == 0 ? j : N - 1 - j);
                                if (i & 1) c = N - 1 - c;
                                cand[idx++] = id(r, c);
                            }
                        }
                    } else {
                        for (int j = 0; j < N; ++j) {
                            int c = (start_c == 0 ? j : N - 1 - j);
                            for (int i = 0; i < N; ++i) {
                                int r = (start_r == 0 ? i : N - 1 - i);
                                if (j & 1) r = N - 1 - r;
                                cand[idx++] = id(r, c);
                            }
                        }
                    }
                    ll sc = score_of(cand);
                    if (sc > best_init_score) {
                        best_init_score = sc;
                        best_init = cand;
                    }
                }
            }
        }
        path = best_init;
        rep(i, M) pos[path[(int)i]] = (int)i;
        current_score = best_init_score;
        initial_score = current_score;
        best_score = current_score;
        best_path = path;
    };

    auto badness = [&](int idx) -> ll {
        int v = path[idx];
        return llabs((ll)idx - (ll)(A[v] - 1));
    };
    auto front_heavy_score = [&](int idx) -> ll {
        int v = path[idx];
        return (ll)A[v] - (ll)idx;
    };
    auto removable = [&](int idx) -> bool {
        if (idx == 0 || idx == M - 1) return true;
        return adjacent(path[idx - 1], path[idx + 1]);
    };

    auto pick_bad_index = [&](int l, int r, int sample_cnt) -> int {
        if (l == r) return l;
        int best_idx = l + (int)(xor128() % (uint32_t)(r - l + 1));
        ll best_bad = badness(best_idx);
        for (int s = 1; s < sample_cnt; ++s) {
            int idx = l + (int)(xor128() % (uint32_t)(r - l + 1));
            ll b = badness(idx);
            if (b > best_bad) {
                best_bad = b;
                best_idx = idx;
            }
        }
        return best_idx;
    };

    auto pick_bad_removable_index = [&](int sample_cnt) -> int {
        int best_idx = -1;
        ll best_bad = -LINF;
        for (int s = 0; s < sample_cnt; ++s) {
            int idx = (int)(xor128() % (uint32_t)M);
            if (!removable(idx)) continue;
            ll b = badness(idx);
            if (best_idx == -1 || b > best_bad) {
                best_bad = b;
                best_idx = idx;
            }
        }
        return best_idx;
    };

    auto pick_front_heavy_index = [&](int sample_cnt, int phase) -> int {
        int limit_r = (phase == 0 ? (M * 7) / 10 : (phase == 1 ? (M * 4) / 5 : M - 1));
        int best_idx = (int)(xor128() % (uint32_t)(limit_r + 1));
        ll best_sc = front_heavy_score(best_idx);
        for (int s = 1; s < sample_cnt; ++s) {
            int idx = (int)(xor128() % (uint32_t)(limit_r + 1));
            ll sc = front_heavy_score(idx);
            if (sc > best_sc) {
                best_sc = sc;
                best_idx = idx;
            }
        }
        return best_idx;
    };

    auto pick_front_heavy_removable_index = [&](int sample_cnt, int phase) -> int {
        int limit_r = (phase == 0 ? (M * 7) / 10 : (phase == 1 ? (M * 4) / 5 : M - 1));
        int best_idx = -1;
        ll best_sc = -LINF;
        for (int s = 0; s < sample_cnt; ++s) {
            int idx = (int)(xor128() % (uint32_t)(limit_r + 1));
            if (!removable(idx)) continue;
            ll sc = front_heavy_score(idx);
            if (best_idx == -1 || sc > best_sc) {
                best_sc = sc;
                best_idx = idx;
            }
        }
        return best_idx;
    };

    auto delta_2opt = [&](int l, int r) -> ll {
        ll delta = 0;
        while (l < r) {
            delta += (ll)(r - l) * ((ll)A[path[l]] - (ll)A[path[r]]);
            ++l; --r;
        }
        return delta;
    };

    auto apply_2opt = [&](int u_idx, int j, ll delta) {
        int l = u_idx + 1, r = j;
        while (l < r) {
            swap(path[l], path[r]);
            pos[path[l]] = l;
            pos[path[r]] = r;
            ++l; --r;
        }
        if (l == r) pos[path[l]] = l;
        current_score += delta;
        if (current_score > best_score) {
            best_score = current_score;
            best_path = path;
        }
    };

    auto propose_2opt = [&](int cand_eval_limit) -> Move2Opt {
        Move2Opt best_mv;
        int u_idx = pick_bad_index(0, M - 2, BAD_PICK_SAMPLE);
        int u = path[u_idx];
        int nu = path[u_idx + 1];
        int cands[8], csz = 0;
        int ux = u / N, uy = u % N;
        for (int d = 0; d < 8; ++d) {
            int vx = ux + (int)dxx[d], vy = uy + (int)dyy[d];
            if (vx < 0 || vx >= N || vy < 0 || vy >= N) continue;
            int j = pos[id(vx, vy)];
            if (j <= u_idx + 1) continue;
            if (j == M - 1 || adjacent(nu, path[j + 1])) cands[csz++] = j;
        }
        best_mv.candidate_count = csz;
        if (csz == 0) return best_mv;
        int order[8]; rep(i, csz) order[(int)i] = (int)i;
        for (int i = csz - 1; i >= 1; --i) {
            int j = (int)(xor128() % (uint32_t)(i + 1));
            swap(order[i], order[j]);
        }
        int eval_cnt = min(csz, cand_eval_limit);
        best_mv.eval_count = eval_cnt;
        for (int z = 0; z < eval_cnt; ++z) {
            int j = cands[order[z]];
            ll delta = delta_2opt(u_idx + 1, j);
            if (!best_mv.valid || delta > best_mv.delta) best_mv = {u_idx, j, csz, eval_cnt, delta, true};
        }
        return best_mv;
    };

    auto delta_or1 = [&](int m, int t) -> ll {
        int x = path[m];
        ll ax = A[x], sum = 0;
        if (m < t) {
            for (int k = m + 1; k <= t; ++k) sum += A[path[k]];
            return (ll)(t - m) * ax - sum;
        } else {
            for (int k = t + 1; k <= m - 1; ++k) sum += A[path[k]];
            return sum - (ll)(m - (t + 1)) * ax;
        }
    };

    auto apply_or1 = [&](int m, int t, ll delta) {
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
        current_score += delta;
        if (current_score > best_score) {
            best_score = current_score;
            best_path = path;
        }
    };

    MoveStats stat2, stat_or1;

    auto build_or1_move_from_m = [&](int m, int cand_eval_limit, int source_mode, int primary_mode, int min_span, int relax_level) -> MoveOr1 {
        MoveOr1 best_mv;
        best_mv.source_mode = source_mode;
        best_mv.primary_mode = primary_mode;
        best_mv.relax_level = relax_level;
        if (m < 0 || m >= M) return best_mv;

        stat_or1.mode_attempt_calls[source_mode]++;

        int x = path[m];
        int a = (m > 0 ? path[m - 1] : -1);
        int b = (m + 1 < M ? path[m + 1] : -1);
        if (a != -1 && b != -1 && !adjacent(a, b)) {
            best_mv.bridge_ok = false;
            stat_or1.mode_attempt_invalid_bridge[source_mode]++;
            return best_mv;
        }

        int cands[8], csz = 0;
        int xx = x / N, xy = x % N;
        for (int d = 0; d < 8; ++d) {
            int vx = xx + (int)dxx[d], vy = xy + (int)dyy[d];
            if (vx < 0 || vx >= N || vy < 0 || vy >= N) continue;
            int t = pos[id(vx, vy)];
            if (t == m || t == m - 1) continue;
            int sp = abs(t - m);
            if (sp < min_span) continue;
            if (t < m - 1) {
                if (adjacent(x, path[t + 1])) cands[csz++] = t;
            } else if (t > m) {
                if (t == M - 1 || adjacent(x, path[t + 1])) cands[csz++] = t;
            }
        }
        best_mv.candidate_count = csz;
        if (csz == 0) {
            stat_or1.mode_attempt_invalid_no_candidate[source_mode]++;
            return best_mv;
        }
        stat_or1.mode_attempt_valid[source_mode]++;

        int order[8]; rep(i, csz) order[(int)i] = (int)i;
        for (int i = csz - 1; i >= 1; --i) {
            int j = (int)(xor128() % (uint32_t)(i + 1));
            swap(order[i], order[j]);
        }
        int eval_cnt = min(csz, cand_eval_limit);
        best_mv.eval_count = eval_cnt;
        int best_sp = -1;
        for (int z = 0; z < eval_cnt; ++z) {
            int t = cands[order[z]];
            int sp = abs(t - m);
            ll delta = delta_or1(m, t);
            if (!best_mv.valid || delta > best_mv.delta || (delta == best_mv.delta && sp > best_sp)) {
                best_sp = sp;
                best_mv = {m, t, csz, eval_cnt, delta, true, true, source_mode, primary_mode, relax_level, source_mode != primary_mode || relax_level > 0};
            }
        }
        return best_mv;
    };

    auto try_mode = [&](int mode, int primary_mode, int phase, int cand_eval_limit, int base_min_span, int relax_level) -> MoveOr1 {
        int min_span = base_min_span;
        if (relax_level == 1) min_span /= 2;
        else if (relax_level == 2) min_span = 0;

        int m = -1;
        if (mode == 0) m = pick_front_heavy_index(FREE_HEAVY_SAMPLE, phase);
        else if (mode == 1) m = pick_front_heavy_removable_index(REMOVABLE_HEAVY_SAMPLE, phase);
        else m = pick_bad_removable_index(REMOVABLE_PICK_SAMPLE);
        return build_or1_move_from_m(m, cand_eval_limit, mode, primary_mode, min_span, relax_level);
    };

    auto propose_or1 = [&](int cand_eval_limit, int removable_percent, int phase) -> MoveOr1 {
        int free_min_span = (phase == 0 ? FREE_MIN_SPAN_EARLY : (phase == 1 ? FREE_MIN_SPAN_MID : FREE_MIN_SPAN_LATE));
        int rem_min_span = (phase == 0 ? REM_MIN_SPAN_EARLY : (phase == 1 ? REM_MIN_SPAN_MID : REM_MIN_SPAN_LATE));
        int rem_front_percent = (phase == 0 ? REM_FRONT_PERCENT_EARLY : (phase == 1 ? REM_FRONT_PERCENT_MID : REM_FRONT_PERCENT_LATE));

        bool use_removable = ((int)(xor128() % 100U) < removable_percent);
        int primary_mode = 0;
        if (use_removable) {
            primary_mode = (((int)(xor128() % 100U) < rem_front_percent) ? 1 : 2);
        }
        stat_or1.primary_tried[primary_mode]++;

        auto attempt_chain = [&](int primary) -> MoveOr1 {
            vector<tuple<int,int>> order; // mode, relax
            if (primary == 0) {
                order = {{0,0}, {2,0}, {2,1}, {2,2}, {1,0}, {1,1}, {1,2}};
            } else if (primary == 1) {
                order = {{1,0}, {1,1}, {1,2}, {2,0}, {2,1}, {2,2}};
            } else {
                order = {{2,0}, {2,1}, {2,2}, {1,0}, {1,1}, {1,2}};
            }
            MoveOr1 last;
            last.primary_mode = primary;
            for (auto [mode, relax] : order) {
                int base_span = (mode == 0 ? free_min_span : rem_min_span);
                MoveOr1 mv = try_mode(mode, primary, phase, cand_eval_limit, base_span, relax);
                if (mv.valid) {
                    if (primary == 0 && mode == 1) stat_or1.fallback_free_to_rem_front++;
                    if (primary == 0 && mode == 2) stat_or1.fallback_free_to_rem_bad++;
                    if (primary == 1 && mode == 2) stat_or1.fallback_rem_front_to_rem_bad++;
                    if (primary == 2 && mode == 1) stat_or1.fallback_rem_bad_to_rem_front++;
                    if (mode != primary || relax > 0) stat_or1.fallback_used++;
                    stat_or1.relax_used[relax]++;
                    stat_or1.relax_used_mode[mode][relax]++;
                    return mv;
                }
                last = mv;
            }
            return last;
        };

        return attempt_chain(primary_mode);
    };

#ifdef LOCAL
    auto validate_path = [&](const vector<int>& p) -> bool {
        vector<int> used(M, 0);
        rep(i, M) {
            int v = p[(int)i];
            if (v < 0 || v >= M || used[v]) return false;
            used[v] = 1;
            if (i + 1 < M && !adjacent(v, p[(int)i + 1])) return false;
        }
        return true;
    };
#endif

    build_initial();

#ifdef LOCAL
    if (!validate_path(path) || score_of(path) != current_score) {
        cerr << "initial invalid\n";
        exit(1);
    }
#endif

    double temp = START_TEMP;
    long long iter = 0;
    ll best_score_phase[3] = {best_score, best_score, best_score};

    auto record_accept = [&](MoveStats& st, int phase, int span, ll delta, int mode = -1) {
        ++st.accepted;
        st.accepted_phase[phase]++;
        st.accepted_delta_phase[phase] += delta;
        st.accepted_span_sum += span;
        st.accepted_delta_sum += delta;
        st.accepted_span_bucket[span_bucket8(span)]++;
        if (mode != -1) {
            st.accepted_mode[mode]++;
            st.accepted_delta_mode[mode] += delta;
            st.accepted_span_mode[mode] += span;
            st.accepted_phase_mode[mode][phase]++;
            st.accepted_span_bucket_mode[mode][span_bucket8(span)]++;
        }
        if (delta > 0) {
            ++st.improved;
            st.improved_phase[phase]++;
            st.improved_delta_phase[phase] += delta;
            st.improved_delta_sum += delta;
            st.improved_span_bucket[span_bucket8(span)]++;
            if (mode != -1) {
                st.improved_mode[mode]++;
                st.improved_delta_mode[mode] += delta;
                st.improved_phase_mode[mode][phase]++;
                st.improved_delta_phase_mode[mode][phase] += delta;
                st.improved_span_bucket_mode[mode][span_bucket8(span)]++;
            }
        } else {
            ++st.accepted_non_improve;
            if (delta < 0) {
                ++st.accepted_negative;
                st.negative_phase[phase]++;
            }
        }
    };

    while (true) {
        double elapsed = 0.0;
        int phase = 0;
        if ((iter & TEMP_UPDATE_MASK) == 0) {
            auto current_time = chrono::high_resolution_clock::now();
            elapsed = chrono::duration<double>(current_time - start_time).count();
            if (elapsed > TIME_LIMIT) break;
            double progress = elapsed / TIME_LIMIT;
            temp = START_TEMP * pow(END_TEMP / START_TEMP, progress);
            phase = phase_id(elapsed);
        } else {
            auto current_time = chrono::high_resolution_clock::now();
            elapsed = chrono::duration<double>(current_time - start_time).count();
            phase = phase_id(elapsed);
        }
        ++iter;

        int move2_eval_limit = (phase == 0 ? MOVE2_EVAL_EARLY : (phase == 1 ? MOVE2_EVAL_MID : MOVE2_EVAL_LATE));
        int or1_eval_limit = (phase == 0 ? OR1_EVAL_EARLY : (phase == 1 ? OR1_EVAL_MID : OR1_EVAL_LATE));
        int or1_percent = (phase == 0 ? OR1_PERCENT_EARLY : (phase == 1 ? OR1_PERCENT_MID : OR1_PERCENT_LATE));
        int or1_removable_percent = (phase == 0 ? OR1_REMOVABLE_PERCENT_EARLY : (phase == 1 ? OR1_REMOVABLE_PERCENT_MID : OR1_REMOVABLE_PERCENT_LATE));

        bool use_or1 = ((int)(xor128() % 100U) < or1_percent);
        if (!use_or1) {
            ++stat2.tried;
            ++stat2.tried_phase[phase];
            Move2Opt mv = propose_2opt(move2_eval_limit);
            stat2.candidate_sum += mv.candidate_count;
            stat2.eval_sum += mv.eval_count;
            if (!mv.valid) {
                ++stat2.invalid_no_candidate;
                ++stat2.invalid_no_candidate_phase[phase];
                continue;
            }
            ++stat2.valid;
            ++stat2.valid_phase[phase];
            bool accept = false;
            if (mv.delta >= 0) accept = true;
            else {
                double prob = exp((double)mv.delta / temp);
                double r = (double)(xor128() % 1000000U) * (1.0 / 1000000.0);
                if (r < prob) accept = true;
            }
            if (accept) {
                apply_2opt(mv.u_idx, mv.j, mv.delta);
                record_accept(stat2, phase, mv.j - mv.u_idx, mv.delta);
                chmax(best_score_phase[phase], best_score);
            }
        } else {
            ++stat_or1.tried;
            ++stat_or1.tried_phase[phase];
            MoveOr1 mv = propose_or1(or1_eval_limit, or1_removable_percent, phase);
            stat_or1.candidate_sum += mv.candidate_count;
            stat_or1.eval_sum += mv.eval_count;
            if (!mv.valid) {
                if (!mv.bridge_ok) {
                    ++stat_or1.invalid_bridge;
                    ++stat_or1.invalid_bridge_phase[phase];
                } else {
                    ++stat_or1.invalid_no_candidate;
                    ++stat_or1.invalid_no_candidate_phase[phase];
                }
                continue;
            }
            ++stat_or1.valid;
            ++stat_or1.valid_phase[phase];
            ++stat_or1.valid_mode[mv.source_mode];

            bool accept = false;
            if (mv.delta >= 0) accept = true;
            else {
                double prob = exp((double)mv.delta / temp);
                double r = (double)(xor128() % 1000000U) * (1.0 / 1000000.0);
                if (r < prob) accept = true;
            }
            if (accept) {
                apply_or1(mv.m, mv.t, mv.delta);
                record_accept(stat_or1, phase, abs(mv.t - mv.m), mv.delta, mv.source_mode);
                chmax(best_score_phase[phase], best_score);
            }
        }
    }

#ifdef LOCAL
    if (!validate_path(best_path) || score_of(best_path) != best_score) {
        cerr << "best invalid\n";
        exit(1);
    }

    auto dump_span8 = [&](const array<long long, 8>& a) {
        rep(i, 8) cerr << ' ' << a[(int)i];
    };

    auto dump_stats = [&](const string& name, const MoveStats& st, bool show_mode) {
        cerr << "[" << name << "]\n";
        cerr << "  tried=" << st.tried
             << " valid=" << st.valid
             << " accepted=" << st.accepted
             << " improved=" << st.improved
             << " accepted_non_improve=" << st.accepted_non_improve
             << " accepted_negative=" << st.accepted_negative << '\n';
        cerr << "  invalid_bridge=" << st.invalid_bridge
             << " invalid_no_candidate=" << st.invalid_no_candidate << '\n';
        cerr << fixed << setprecision(6);
        cerr << "  valid_rate=" << (st.tried ? (double)st.valid / st.tried : 0.0)
             << " accept_per_try=" << (st.tried ? (double)st.accepted / st.tried : 0.0)
             << " accept_per_valid=" << (st.valid ? (double)st.accepted / st.valid : 0.0)
             << " improve_per_try=" << (st.tried ? (double)st.improved / st.tried : 0.0)
             << " improve_per_accept=" << (st.accepted ? (double)st.improved / st.accepted : 0.0) << '\n';
        cerr << "  avg_candidate=" << (st.tried ? (double)st.candidate_sum / st.tried : 0.0)
             << " avg_candidate_valid=" << (st.valid ? (double)st.candidate_sum / st.valid : 0.0)
             << " avg_eval=" << (st.tried ? (double)st.eval_sum / st.tried : 0.0)
             << " avg_eval_valid=" << (st.valid ? (double)st.eval_sum / st.valid : 0.0)
             << " avg_span_accepted=" << (st.accepted ? (double)st.accepted_span_sum / st.accepted : 0.0)
             << " avg_delta_accepted=" << (st.accepted ? (double)st.accepted_delta_sum / st.accepted : 0.0)
             << " avg_delta_improved=" << (st.improved ? (double)st.improved_delta_sum / st.improved : 0.0) << '\n';
        cerr << "  phase_tried="; rep(i,3) cerr << ' ' << st.tried_phase[(int)i];
        cerr << "\n  phase_valid="; rep(i,3) cerr << ' ' << st.valid_phase[(int)i];
        cerr << "\n  phase_accepted="; rep(i,3) cerr << ' ' << st.accepted_phase[(int)i];
        cerr << "\n  phase_improved="; rep(i,3) cerr << ' ' << st.improved_phase[(int)i];
        cerr << "\n  phase_negative="; rep(i,3) cerr << ' ' << st.negative_phase[(int)i];
        cerr << "\n  phase_invalid_bridge="; rep(i,3) cerr << ' ' << st.invalid_bridge_phase[(int)i];
        cerr << "\n  phase_invalid_no_candidate="; rep(i,3) cerr << ' ' << st.invalid_no_candidate_phase[(int)i];
        cerr << "\n  phase_accepted_delta="; rep(i,3) cerr << ' ' << st.accepted_delta_phase[(int)i];
        cerr << "\n  phase_improved_delta="; rep(i,3) cerr << ' ' << st.improved_delta_phase[(int)i];
        cerr << "\n  accepted_span_bucket="; dump_span8(st.accepted_span_bucket);
        cerr << "\n  improved_span_bucket="; dump_span8(st.improved_span_bucket);
        cerr << '\n';
        if (show_mode) {
            cerr << "  primary_tried(free,rem_front,rem_bad)=" << st.primary_tried[0] << ' ' << st.primary_tried[1] << ' ' << st.primary_tried[2] << '\n';
            cerr << "  mode_attempt_calls(free,rem_front,rem_bad)=" << st.mode_attempt_calls[0] << ' ' << st.mode_attempt_calls[1] << ' ' << st.mode_attempt_calls[2] << '\n';
            cerr << "  mode_attempt_valid(free,rem_front,rem_bad)=" << st.mode_attempt_valid[0] << ' ' << st.mode_attempt_valid[1] << ' ' << st.mode_attempt_valid[2] << '\n';
            cerr << "  mode_attempt_invalid_bridge(free,rem_front,rem_bad)=" << st.mode_attempt_invalid_bridge[0] << ' ' << st.mode_attempt_invalid_bridge[1] << ' ' << st.mode_attempt_invalid_bridge[2] << '\n';
            cerr << "  mode_attempt_invalid_no_candidate(free,rem_front,rem_bad)=" << st.mode_attempt_invalid_no_candidate[0] << ' ' << st.mode_attempt_invalid_no_candidate[1] << ' ' << st.mode_attempt_invalid_no_candidate[2] << '\n';
            cerr << "  mode_valid(free,rem_front,rem_bad)=" << st.valid_mode[0] << ' ' << st.valid_mode[1] << ' ' << st.valid_mode[2] << '\n';
            cerr << "  mode_accepted(free,rem_front,rem_bad)=" << st.accepted_mode[0] << ' ' << st.accepted_mode[1] << ' ' << st.accepted_mode[2] << '\n';
            cerr << "  mode_improved(free,rem_front,rem_bad)=" << st.improved_mode[0] << ' ' << st.improved_mode[1] << ' ' << st.improved_mode[2] << '\n';
            cerr << "  mode_avg_span_accepted(free,rem_front,rem_bad)="
                 << (st.accepted_mode[0] ? (double)st.accepted_span_mode[0] / st.accepted_mode[0] : 0.0) << ' '
                 << (st.accepted_mode[1] ? (double)st.accepted_span_mode[1] / st.accepted_mode[1] : 0.0) << ' '
                 << (st.accepted_mode[2] ? (double)st.accepted_span_mode[2] / st.accepted_mode[2] : 0.0) << '\n';
            cerr << "  mode_avg_delta_accepted(free,rem_front,rem_bad)="
                 << (st.accepted_mode[0] ? (double)st.accepted_delta_mode[0] / st.accepted_mode[0] : 0.0) << ' '
                 << (st.accepted_mode[1] ? (double)st.accepted_delta_mode[1] / st.accepted_mode[1] : 0.0) << ' '
                 << (st.accepted_mode[2] ? (double)st.accepted_delta_mode[2] / st.accepted_mode[2] : 0.0) << '\n';
            cerr << "  mode_avg_delta_improved(free,rem_front,rem_bad)="
                 << (st.improved_mode[0] ? (double)st.improved_delta_mode[0] / st.improved_mode[0] : 0.0) << ' '
                 << (st.improved_mode[1] ? (double)st.improved_delta_mode[1] / st.improved_mode[1] : 0.0) << ' '
                 << (st.improved_mode[2] ? (double)st.improved_delta_mode[2] / st.improved_mode[2] : 0.0) << '\n';
            cerr << "  mode_improved_per_try(free,rem_front,rem_bad)="
                 << (st.mode_attempt_calls[0] ? (double)st.improved_delta_mode[0] / st.mode_attempt_calls[0] : 0.0) << ' '
                 << (st.mode_attempt_calls[1] ? (double)st.improved_delta_mode[1] / st.mode_attempt_calls[1] : 0.0) << ' '
                 << (st.mode_attempt_calls[2] ? (double)st.improved_delta_mode[2] / st.mode_attempt_calls[2] : 0.0) << '\n';
            cerr << "  fallback_used=" << st.fallback_used
                 << " free_to_rem_front=" << st.fallback_free_to_rem_front
                 << " free_to_rem_bad=" << st.fallback_free_to_rem_bad
                 << " rem_front_to_rem_bad=" << st.fallback_rem_front_to_rem_bad
                 << " rem_bad_to_rem_front=" << st.fallback_rem_bad_to_rem_front << '\n';
            cerr << "  relax_used(total strict,half,zero)=" << st.relax_used[0] << ' ' << st.relax_used[1] << ' ' << st.relax_used[2] << '\n';
            cerr << "  relax_used_mode(free strict,half,zero)=" << st.relax_used_mode[0][0] << ' ' << st.relax_used_mode[0][1] << ' ' << st.relax_used_mode[0][2] << '\n';
            cerr << "  relax_used_mode(rem_front strict,half,zero)=" << st.relax_used_mode[1][0] << ' ' << st.relax_used_mode[1][1] << ' ' << st.relax_used_mode[1][2] << '\n';
            cerr << "  relax_used_mode(rem_bad strict,half,zero)=" << st.relax_used_mode[2][0] << ' ' << st.relax_used_mode[2][1] << ' ' << st.relax_used_mode[2][2] << '\n';
        }
    };

    auto end_time = chrono::high_resolution_clock::now();
    double total_elapsed = chrono::duration<double>(end_time - start_time).count();
    cerr << "initial_score=" << initial_score
         << " final_current_score=" << current_score
         << " best_score=" << best_score
         << " gain_from_initial=" << (best_score - initial_score)
         << " iter=" << iter
         << " elapsed=" << total_elapsed << '\n';
    cerr << "best_score_phase=" << best_score_phase[0] << ' ' << best_score_phase[1] << ' ' << best_score_phase[2] << '\n';
    cerr << "params: TIME_LIMIT=" << TIME_LIMIT
         << " START_TEMP=" << START_TEMP
         << " END_TEMP=" << END_TEMP
         << " BAD_PICK_SAMPLE=" << BAD_PICK_SAMPLE
         << " REMOVABLE_PICK_SAMPLE=" << REMOVABLE_PICK_SAMPLE
         << " FREE_HEAVY_SAMPLE=" << FREE_HEAVY_SAMPLE
         << " REMOVABLE_HEAVY_SAMPLE=" << REMOVABLE_HEAVY_SAMPLE
         << " MOVE2_EVAL={" << MOVE2_EVAL_EARLY << ',' << MOVE2_EVAL_MID << ',' << MOVE2_EVAL_LATE << '}'
         << " OR1_EVAL={" << OR1_EVAL_EARLY << ',' << OR1_EVAL_MID << ',' << OR1_EVAL_LATE << '}'
         << " OR1_PERCENT={" << OR1_PERCENT_EARLY << ',' << OR1_PERCENT_MID << ',' << OR1_PERCENT_LATE << '}'
         << " OR1_REMOVABLE_PERCENT={" << OR1_REMOVABLE_PERCENT_EARLY << ',' << OR1_REMOVABLE_PERCENT_MID << ',' << OR1_REMOVABLE_PERCENT_LATE << '}'
         << " FREE_MIN_SPAN={" << FREE_MIN_SPAN_EARLY << ',' << FREE_MIN_SPAN_MID << ',' << FREE_MIN_SPAN_LATE << '}'
         << " REM_MIN_SPAN={" << REM_MIN_SPAN_EARLY << ',' << REM_MIN_SPAN_MID << ',' << REM_MIN_SPAN_LATE << '}'
         << " REM_FRONT_PERCENT={" << REM_FRONT_PERCENT_EARLY << ',' << REM_FRONT_PERCENT_MID << ',' << REM_FRONT_PERCENT_LATE << "}\n";
    dump_stats("2opt", stat2, false);
    dump_stats("or1", stat_or1, true);
#endif

    rep(k, M) {
        int v = best_path[(int)k];
        cout << v / N << " " << v % N << "\n";
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
gpt-1 im



*/