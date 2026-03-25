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
    long long tried = 0;
    long long valid = 0;
    long long accepted = 0;
    long long improved = 0;
    long long accepted_non_improve = 0;
    long long accepted_negative = 0;

    long long invalid_bridge = 0;
    long long invalid_no_candidate = 0;

    long long candidate_sum = 0;
    long long eval_sum = 0;
    long long accepted_span_sum = 0;
    long long accepted_delta_sum = 0;
    long long improved_delta_sum = 0;

    array<long long, 3> tried_phase{};
    array<long long, 3> valid_phase{};
    array<long long, 3> accepted_phase{};
    array<long long, 3> improved_phase{};
    array<long long, 3> negative_phase{};

    array<long long, 6> accepted_span_bucket{};
    array<long long, 6> improved_span_bucket{};
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
};

void solve()
{
    auto start_time = chrono::high_resolution_clock::now();

    const double TIME_LIMIT = 2.92;
    const double START_TEMP = 4.0e7;
    const double END_TEMP = 3.0e3;
    const int TEMP_UPDATE_MASK = 1023;

    const int BAD_PICK_SAMPLE = 16;
    const int MOVE2_EVAL_EARLY = 2;
    const int MOVE2_EVAL_MID = 3;
    const int MOVE2_EVAL_LATE = 4;
    const int OR1_EVAL_EARLY = 4;
    const int OR1_EVAL_MID = 3;
    const int OR1_EVAL_LATE = 2;

    const int OR1_PERCENT_EARLY = 40;
    const int OR1_PERCENT_MID = 30;
    const int OR1_PERCENT_LATE = 18;

    int N;
    if (!(cin >> N)) return;
    const int M = N * N;

    vector<int> A(M);
    rep(i, N) rep(j, N) cin >> A[(int)i * N + (int)j];

    auto id = [&](int x, int y) -> int {
        return x * N + y;
    };

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

    auto span_bucket = [&](int span) -> int {
        if (span <= 1) return 0;
        if (span <= 2) return 1;
        if (span <= 4) return 2;
        if (span <= 8) return 3;
        if (span <= 16) return 4;
        return 5;
    };

    vector<int> path(M), best_path(M), pos(M);
    ll current_score = -LINF;
    ll best_score = -LINF;
    ll initial_score = -LINF;

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

    auto removable = [&](int idx) -> bool {
        if (idx == 0 || idx == M - 1) return true;
        return adjacent(path[idx - 1], path[idx + 1]);
    };

    auto pick_bad_index = [&](int l, int r, int sample_cnt) -> int {
        if (l == r) return l;
        if ((xor128() & 7U) == 0U) {
            return l + (int)(xor128() % (uint32_t)(r - l + 1));
        }
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
        int fallback = (int)(xor128() % (uint32_t)M);
        int best_idx = -1;
        ll best_bad = -1;
        for (int s = 0; s < sample_cnt; ++s) {
            int idx = (int)(xor128() % (uint32_t)M);
            if (!removable(idx)) continue;
            ll b = badness(idx);
            if (best_idx == -1 || b > best_bad) {
                best_idx = idx;
                best_bad = b;
            }
        }
        if (best_idx != -1) return best_idx;
        if (removable(fallback)) return fallback;
        for (int i = 0; i < M; ++i) if (removable(i)) return i;
        return fallback;
    };

    auto delta_2opt = [&](int l, int r) -> ll {
        ll delta = 0;
        while (l < r) {
            delta += (ll)(r - l) * ((ll)A[path[l]] - (ll)A[path[r]]);
            ++l;
            --r;
        }
        return delta;
    };

    auto apply_2opt = [&](int u_idx, int j, ll delta) {
        int l = u_idx + 1, r = j;
        while (l < r) {
            swap(path[l], path[r]);
            pos[path[l]] = l;
            pos[path[r]] = r;
            ++l;
            --r;
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

        int cands[8];
        int csz = 0;

        int ux = u / N, uy = u % N;
        for (int d = 0; d < 8; ++d) {
            int vx = ux + (int)dxx[d];
            int vy = uy + (int)dyy[d];
            if (vx < 0 || vx >= N || vy < 0 || vy >= N) continue;
            int j = pos[id(vx, vy)];
            if (j <= u_idx + 1) continue;
            if (j == M - 1 || adjacent(nu, path[j + 1])) {
                cands[csz++] = j;
            }
        }
        best_mv.candidate_count = csz;
        if (csz == 0) return best_mv;

        int order[8];
        rep(i, csz) order[(int)i] = (int)i;
        for (int i = csz - 1; i >= 1; --i) {
            int j = (int)(xor128() % (uint32_t)(i + 1));
            swap(order[i], order[j]);
        }

        int eval_cnt = min(csz, cand_eval_limit);
        best_mv.eval_count = eval_cnt;
        for (int z = 0; z < eval_cnt; ++z) {
            int j = cands[order[z]];
            ll delta = delta_2opt(u_idx + 1, j);
            if (!best_mv.valid || delta > best_mv.delta) {
                best_mv = {u_idx, j, csz, eval_cnt, delta, true};
            }
        }
        return best_mv;
    };

    auto delta_or1 = [&](int m, int t) -> ll {
        int x = path[m];
        ll ax = A[x];
        ll sum = 0;

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

    auto propose_or1 = [&](int cand_eval_limit) -> MoveOr1 {
        MoveOr1 best_mv;

        int m = pick_bad_removable_index(BAD_PICK_SAMPLE + 8);
        int x = path[m];
        int a = (m > 0 ? path[m - 1] : -1);
        int b = (m + 1 < M ? path[m + 1] : -1);

        if (a != -1 && b != -1 && !adjacent(a, b)) {
            best_mv.bridge_ok = false;
            return best_mv;
        }

        int cands[8];
        int csz = 0;

        int xx = x / N, xy = x % N;
        for (int d = 0; d < 8; ++d) {
            int vx = xx + (int)dxx[d];
            int vy = xy + (int)dyy[d];
            if (vx < 0 || vx >= N || vy < 0 || vy >= N) continue;
            int t = pos[id(vx, vy)];
            if (t == m || t == m - 1) continue;

            if (t < m - 1) {
                if (adjacent(x, path[t + 1])) cands[csz++] = t;
            } else if (t > m) {
                if (t == M - 1 || adjacent(x, path[t + 1])) cands[csz++] = t;
            }
        }
        best_mv.candidate_count = csz;
        if (csz == 0) return best_mv;

        int order[8];
        rep(i, csz) order[(int)i] = (int)i;
        for (int i = csz - 1; i >= 1; --i) {
            int j = (int)(xor128() % (uint32_t)(i + 1));
            swap(order[i], order[j]);
        }

        int eval_cnt = min(csz, cand_eval_limit);
        best_mv.eval_count = eval_cnt;
        for (int z = 0; z < eval_cnt; ++z) {
            int t = cands[order[z]];
            ll delta = delta_or1(m, t);
            if (!best_mv.valid || delta > best_mv.delta) {
                best_mv = {m, t, csz, eval_cnt, delta, true, true};
            }
        }
        return best_mv;
    };

#ifdef LOCAL
    auto validate_path = [&](const vector<int>& p) -> bool {
        vector<int> used(M, 0);
        rep(i, M) {
            int v = p[(int)i];
            if (v < 0 || v >= M) return false;
            if (used[v]) return false;
            used[v] = 1;
            if (i + 1 < M && !adjacent(v, p[(int)i + 1])) return false;
        }
        return true;
    };
#endif

    build_initial();

#ifdef LOCAL
    if (!validate_path(path)) {
        cerr << "initial path invalid\n";
        exit(1);
    }
    if (score_of(path) != current_score) {
        cerr << "initial score mismatch\n";
        exit(1);
    }
#endif

    MoveStats stat2, stat_or1;

    double temp = START_TEMP;
    long long iter = 0;
    ll best_score_phase[3] = {best_score, best_score, best_score};

    auto record_accept = [&](MoveStats& st, int phase, int span, ll delta) {
        ++st.accepted;
        st.accepted_phase[phase]++;
        st.accepted_span_sum += span;
        st.accepted_delta_sum += delta;
        st.accepted_span_bucket[span_bucket(span)]++;
        if (delta > 0) {
            ++st.improved;
            st.improved_phase[phase]++;
            st.improved_delta_sum += delta;
            st.improved_span_bucket[span_bucket(span)]++;
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

        bool use_or1 = (xor128() % 100U) < (uint32_t)or1_percent;

        if (!use_or1) {
            ++stat2.tried;
            ++stat2.tried_phase[phase];
            Move2Opt mv = propose_2opt(move2_eval_limit);
            stat2.candidate_sum += mv.candidate_count;
            stat2.eval_sum += mv.eval_count;
            if (!mv.valid) {
                ++stat2.invalid_no_candidate;
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
            MoveOr1 mv = propose_or1(or1_eval_limit);
            stat_or1.candidate_sum += mv.candidate_count;
            stat_or1.eval_sum += mv.eval_count;
            if (!mv.valid) {
                if (!mv.bridge_ok) ++stat_or1.invalid_bridge;
                else ++stat_or1.invalid_no_candidate;
                continue;
            }
            ++stat_or1.valid;
            ++stat_or1.valid_phase[phase];

            bool accept = false;
            if (mv.delta >= 0) accept = true;
            else {
                double prob = exp((double)mv.delta / temp);
                double r = (double)(xor128() % 1000000U) * (1.0 / 1000000.0);
                if (r < prob) accept = true;
            }

            if (accept) {
                apply_or1(mv.m, mv.t, mv.delta);
                record_accept(stat_or1, phase, (int)llabs((ll)mv.t - (ll)mv.m), mv.delta);
                chmax(best_score_phase[phase], best_score);
            }
        }
    }

#ifdef LOCAL
    if (!validate_path(best_path)) {
        cerr << "best path invalid\n";
        exit(1);
    }
    if (score_of(best_path) != best_score) {
        cerr << "best score mismatch\n";
        exit(1);
    }

    auto dump_stats = [&](const string& name, const MoveStats& st) {
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

        cerr << "  phase_tried=";
        rep(i, 3) cerr << ' ' << st.tried_phase[(int)i];
        cerr << "\n  phase_valid=";
        rep(i, 3) cerr << ' ' << st.valid_phase[(int)i];
        cerr << "\n  phase_accepted=";
        rep(i, 3) cerr << ' ' << st.accepted_phase[(int)i];
        cerr << "\n  phase_improved=";
        rep(i, 3) cerr << ' ' << st.improved_phase[(int)i];
        cerr << "\n  phase_negative=";
        rep(i, 3) cerr << ' ' << st.negative_phase[(int)i];
        cerr << "\n  accepted_span_bucket=";
        rep(i, 6) cerr << ' ' << st.accepted_span_bucket[(int)i];
        cerr << "\n  improved_span_bucket=";
        rep(i, 6) cerr << ' ' << st.improved_span_bucket[(int)i];
        cerr << '\n';
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
         << " MOVE2_EVAL={" << MOVE2_EVAL_EARLY << ',' << MOVE2_EVAL_MID << ',' << MOVE2_EVAL_LATE << '}'
         << " OR1_EVAL={" << OR1_EVAL_EARLY << ',' << OR1_EVAL_MID << ',' << OR1_EVAL_LATE << '}'
         << " OR1_PERCENT={" << OR1_PERCENT_EARLY << ',' << OR1_PERCENT_MID << ',' << OR1_PERCENT_LATE << '}'
         << '\n';
    dump_stats("2opt", stat2);
    dump_stats("or1", stat_or1);
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
gpt1

*/