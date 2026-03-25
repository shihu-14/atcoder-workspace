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

namespace {

uint32_t xor128() {
    static uint32_t x = 123456789u, y = 362436069u, z = 521288629u, w = 88675123u;
    uint32_t t = x ^ (x << 11);
    x = y; y = z; z = w;
    return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
}

struct Timer {
    chrono::high_resolution_clock::time_point st;
    Timer() : st(chrono::high_resolution_clock::now()) {}
    double elapsed() const {
        return chrono::duration<double>(chrono::high_resolution_clock::now() - st).count();
    }
};

struct Candidate {
    ll score = -(1LL << 60);
    vector<int> path;
    int family = 0; // 0=row strips, 1=col strips
    int mode = -1;
};

struct SearchStats {
    ll init_score = 0;
    ll final_score = 0;
    long long iter = 0;
    double budget = 0.0;
    double phase_switch_local = -1.0;

    long long twoopt_attempts[2] = {0, 0};
    long long twoopt_no_candidate[2] = {0, 0};
    long long twoopt_accepted[2] = {0, 0};
    long long twoopt_choose_best[2] = {0, 0};
    long long twoopt_choose_topk[2] = {0, 0};
    long long twoopt_choose_random[2] = {0, 0};
    long long twoopt_improve_real[2] = {0, 0};
    long long twoopt_worse_real[2] = {0, 0};
    long long twoopt_candidate_sum[2] = {0, 0};
    long double twoopt_acc_delta_real_sum[2] = {0, 0};

    long long lns_attempts[2] = {0, 0};
    long long lns_no_feasible[2] = {0, 0};
    long long lns_accepted[2] = {0, 0};
    long long lns_improve_real[2] = {0, 0};
    long long lns_feasible_perm_sum[2] = {0, 0};
    long double lns_acc_delta_real_sum[2] = {0, 0};

    long long best_updates_phase1 = 0;
    long long best_updates_phase2 = 0;
};

inline int cheb_dist_idx(int a, int b, int N) {
    int ar = a / N, ac = a % N;
    int br = b / N, bc = b % N;
    return max(abs(ar - br), abs(ac - bc));
}

ll compute_score(const vector<int>& path, const vector<int>& w) {
    ll s = 0;
    for (int i = 0; i < (int)path.size(); ++i) s += 1LL * i * w[path[i]];
    return s;
}

pair<ll, ll> calc_reverse_delta_both(const vector<int>& path, int l, int r, const vector<int>& evalW, const vector<int>& realW) {
    ll de = 0, dr = 0;
    int rem = (r - l + 1) / 2;
    for (int k = 0; k < rem; ++k) {
        int i1 = l + k, i2 = r - k;
        int a = path[i1], b = path[i2];
        ll c = (ll)(r - l - 2 * k);
        de += c * (evalW[a] - evalW[b]);
        dr += c * (realW[a] - realW[b]);
    }
    return {de, dr};
}

inline bool accept_move(ll delta, double temp) {
    if (delta >= 0) return true;
    if (temp <= 1e-12) return false;
    double prob = exp((double)delta / temp);
    double rnd = (double)(xor128() % 1000000u) / 1000000.0;
    return prob > rnd;
}

int select_anchor_index(const vector<int>& path, const vector<int>& evalW, int maxEvalW) {
    int N2 = (int)path.size();
    uint32_t mode = xor128() % 100u;
    if (mode < 8u) return (int)(xor128() % (uint32_t)(N2 - 1));

    if (mode < 56u) {
        int upper = max(2, (3 * N2) / 5);
        int bestIdx = (int)(xor128() % (uint32_t)(upper - 1));
        ll bestBad = -(1LL << 60);
        for (int t = 0; t < 24; ++t) {
            int idx = (int)(xor128() % (uint32_t)(upper - 1));
            ll bad = 1LL * (N2 - 1 - idx) * evalW[path[idx]];
            if (bad > bestBad) bestBad = bad, bestIdx = idx;
        }
        return bestIdx;
    }

    int lower = (2 * N2) / 5;
    if (lower >= N2 - 1) lower = max(0, N2 - 2);
    int range = (N2 - 1) - lower;
    int bestIdx = lower + (int)(xor128() % (uint32_t)max(1, range));
    ll bestBad = -(1LL << 60);
    for (int t = 0; t < 24; ++t) {
        int idx = lower + (int)(xor128() % (uint32_t)max(1, range));
        ll bad = 1LL * (idx + 1) * (maxEvalW + 1 - evalW[path[idx]]);
        if (bad > bestBad) bestBad = bad, bestIdx = idx;
    }
    return bestIdx;
}

struct StripResult {
    ll localScore = -(1LL << 60);
    vector<int> order; // local id = row * W + col
};

StripResult solve_strip_lr(const vector<int>& top, const vector<int>& bot, int startRow, int endRow) {
    int W = (int)top.size();
    int S = W * 4 * 4 * 2 * 2;
    const ll NEG = -(1LL << 60);
    vector<ll> dp(S, NEG);
    vector<int> prv(S, -1);
    vector<short> add(S, -1);
    int pc[4] = {0, 1, 1, 2};

    auto enc = [&](int c, int a, int b, int side, int row) {
        return ((((c * 4 + a) * 4 + b) * 2 + side) * 2 + row);
    };
    auto dec = [&](int id, int& c, int& a, int& b, int& side, int& row) {
        row = id & 1; id >>= 1;
        side = id & 1; id >>= 1;
        b = id & 3; id >>= 2;
        a = id & 3; id >>= 2;
        c = id;
    };

    vector<pair<int, int>> states;
    states.reserve(S);
    for (int c = 0; c < W; ++c) {
        for (int a = 0; a < 4; ++a) {
            if (a == 0) continue;
            for (int b = 0; b < 4; ++b) {
                if (c == W - 1 && b != 0) continue;
                for (int side = 0; side < 2; ++side) {
                    if (side == 1 && c == W - 1) continue;
                    for (int row = 0; row < 2; ++row) {
                        if (side == 0 && ((a >> row) & 1) == 0) continue;
                        if (side == 1 && ((b >> row) & 1) == 0) continue;
                        int id = enc(c, a, b, side, row);
                        int cnt = 2 * c + pc[a] + pc[b];
                        states.push_back({cnt, id});
                    }
                }
            }
        }
    }
    sort(states.begin(), states.end());

    int st = enc(0, 1 << startRow, 0, 0, startRow);
    dp[st] = 0;
    int goal = -1;

    for (auto [cnt, id] : states) {
        ll cur = dp[id];
        if (cur == NEG) continue;

        int c, a, b, side, row;
        dec(id, c, a, b, side, row);

        if (c == W - 1 && a == 3 && b == 0 && side == 0 && row == endRow) {
            if (goal == -1 || dp[goal] < cur) goal = id;
            continue;
        }

        int lastCol = c + side;
        int lastRow = row;
        int step = cnt;

        for (int tgtSide = 0; tgtSide <= 1; ++tgtSide) {
            int tgtCol = c + tgtSide;
            if (tgtCol >= W) continue;
            for (int tr = 0; tr < 2; ++tr) {
                if (max(abs(lastCol - tgtCol), abs(lastRow - tr)) > 1) continue;
                if (tgtSide == 0) {
                    if ((a >> tr) & 1) continue;
                } else {
                    if ((b >> tr) & 1) continue;
                }

                int na = a, nb = b;
                if (tgtSide == 0) na |= (1 << tr);
                else nb |= (1 << tr);

                int nc = c, nside = tgtSide, nrow = tr;
                if (na == 3 && nside == 1) {
                    nc = c + 1;
                    na = nb;
                    nb = 0;
                    nside = 0;
                }

                int nid = enc(nc, na, nb, nside, nrow);
                int val = (tr == 0 ? top[tgtCol] : bot[tgtCol]);
                ll cand = cur + 1LL * step * val;
                if (cand > dp[nid]) {
                    dp[nid] = cand;
                    prv[nid] = id;
                    add[nid] = (short)(tr * W + tgtCol);
                }
            }
        }
    }

    StripResult res;
    if (goal == -1) return res;
    res.localScore = dp[goal];
    vector<int> rev;
    int cur = goal;
    while (cur != st) {
        rev.push_back((int)add[cur]);
        cur = prv[cur];
    }
    rev.push_back(startRow * W + 0);
    reverse(rev.begin(), rev.end());
    res.order = std::move(rev);
    return res;
}

StripResult solve_strip_direction(const vector<int>& topActual, const vector<int>& botActual, bool leftToRight, int startRow, int endRow) {
    int W = (int)topActual.size();
    vector<int> top = topActual, bot = botActual;
    if (!leftToRight) {
        reverse(top.begin(), top.end());
        reverse(bot.begin(), bot.end());
    }

    StripResult sub = solve_strip_lr(top, bot, startRow, endRow);
    if (sub.localScore <= -(1LL << 59)) return sub;

    for (int& id : sub.order) {
        int row = id / W, col = id % W;
        int actualCol = leftToRight ? col : (W - 1 - col);
        id = row * W + actualCol;
    }
    return sub;
}

Candidate build_orientation_rows(const vector<int>& A, int N, bool flipV, bool startLeft, int mode) {
    int strips = N / 2;
    Candidate res;
    res.family = 0;
    res.mode = mode;
    vector<int> path;
    path.reserve(N * N);

    for (int s = 0; s < strips; ++s) {
        int r0 = flipV ? (N - 1 - 2 * s) : (2 * s);
        int r1 = flipV ? (N - 2 - 2 * s) : (2 * s + 1);

        vector<int> top(N), bot(N);
        for (int c = 0; c < N; ++c) {
            top[c] = A[r0 * N + c];
            bot[c] = A[r1 * N + c];
        }

        bool leftToRight = ((s & 1) == 0 ? startLeft : !startLeft);
        vector<int> starts = (s == 0 ? vector<int>{0, 1} : vector<int>{0});
        vector<int> ends = (s == strips - 1 ? vector<int>{0, 1} : vector<int>{1});

        StripResult best;
        for (int sr : starts) for (int er : ends) {
            StripResult cur = solve_strip_direction(top, bot, leftToRight, sr, er);
            if (cur.localScore > best.localScore) best = std::move(cur);
        }

        if (best.localScore <= -(1LL << 59)) {
            res.score = -(1LL << 60);
            res.path.clear();
            return res;
        }

        for (int lid : best.order) {
            int lr = lid / N, c = lid % N;
            int gr = (lr == 0 ? r0 : r1);
            path.push_back(gr * N + c);
        }
    }

    res.path = std::move(path);
    res.score = compute_score(res.path, A);
    return res;
}

Candidate build_orientation_cols(const vector<int>& A, int N, bool flipH, bool startTop, int mode) {
    vector<int> B(N * N);
    for (int r = 0; r < N; ++r) for (int c = 0; c < N; ++c) B[c * N + r] = A[r * N + c];

    Candidate t = build_orientation_rows(B, N, flipH, startTop, mode);
    t.family = 1;
    for (int& id : t.path) {
        int rt = id / N, ct = id % N;
        id = ct * N + rt;
    }
    t.score = compute_score(t.path, A);
    return t;
}

bool try_two_opt(vector<int>& path, vector<int>& pos, const vector<int>& evalW, const vector<int>& realW,
                 ll& curEvalScore, ll& curRealScore, double temp, int N, int maxEvalW, bool phase2, SearchStats& st) {
    int ph = phase2 ? 1 : 0;
    st.twoopt_attempts[ph]++;

    int N2 = (int)path.size();
    int u_idx = select_anchor_index(path, evalW, maxEvalW);
    int u = path[u_idx], nxt_u = path[u_idx + 1];

    int candJ[8];
    ll candDe[8], candDr[8];
    int candCnt = 0;
    int ur = u / N, uc = u % N;

    for (int d = 0; d < 8; ++d) {
        int vr = ur + (int)dxx[d], vc = uc + (int)dyy[d];
        if (vr < 0 || vr >= N || vc < 0 || vc >= N) continue;
        int v = vr * N + vc;
        int j = pos[v];
        if (j <= u_idx + 1) continue;
        if (j == N2 - 1 || cheb_dist_idx(nxt_u, path[j + 1], N) <= 1) {
            int l = u_idx + 1, r = j;
            auto [de, dr] = calc_reverse_delta_both(path, l, r, evalW, realW);
            candJ[candCnt] = j;
            candDe[candCnt] = de;
            candDr[candCnt] = dr;
            candCnt++;
        }
    }

    st.twoopt_candidate_sum[ph] += candCnt;
    if (candCnt == 0) {
        st.twoopt_no_candidate[ph]++;
        return false;
    }

    int ord[8];
    for (int i = 0; i < candCnt; ++i) ord[i] = i;
    sort(ord, ord + candCnt, [&](int a, int b) {
        if (candDe[a] != candDe[b]) return candDe[a] > candDe[b];
        return candJ[a] < candJ[b];
    });

    int topK = min(candCnt, 3);
    int sel = ord[0];
    uint32_t rr = xor128() % 100u;
    if (!phase2) {
        if (rr < 55u) sel = ord[0], st.twoopt_choose_best[ph]++;
        else if (rr < 80u) sel = ord[xor128() % (uint32_t)topK], st.twoopt_choose_topk[ph]++;
        else sel = ord[xor128() % (uint32_t)candCnt], st.twoopt_choose_random[ph]++;
    } else {
        if (rr < 70u) sel = ord[0], st.twoopt_choose_best[ph]++;
        else if (rr < 90u) sel = ord[xor128() % (uint32_t)topK], st.twoopt_choose_topk[ph]++;
        else sel = ord[xor128() % (uint32_t)candCnt], st.twoopt_choose_random[ph]++;
    }

    if (!accept_move(candDe[sel], temp)) return false;

    int l = u_idx + 1, r = candJ[sel];
    for (int a = l, b = r; a < b; ++a, --b) {
        swap(path[a], path[b]);
        pos[path[a]] = a;
        pos[path[b]] = b;
    }

    curEvalScore += candDe[sel];
    curRealScore += candDr[sel];
    st.twoopt_accepted[ph]++;
    if (candDr[sel] > 0) st.twoopt_improve_real[ph]++;
    if (candDr[sel] < 0) st.twoopt_worse_real[ph]++;
    st.twoopt_acc_delta_real_sum[ph] += candDr[sel];
    return true;
}

bool try_segment_rebuild(vector<int>& path, vector<int>& pos, const vector<int>& evalW, const vector<int>& realW,
                         ll& curEvalScore, ll& curRealScore, double temp, int N, int maxEvalW, bool phase2, SearchStats& st) {
    int ph = phase2 ? 1 : 0;
    st.lns_attempts[ph]++;

    int N2 = (int)path.size();
    int L = 4 + (int)(xor128() % 2u);
    if (N2 <= L + 2) {
        st.lns_no_feasible[ph]++;
        return false;
    }

    int anchor = select_anchor_index(path, evalW, maxEvalW);
    int lo = max(1, anchor - L + 1);
    int hi = min(N2 - L - 1, anchor);
    if (lo > hi) {
        st.lns_no_feasible[ph]++;
        return false;
    }
    int start = lo + (int)(xor128() % (uint32_t)(hi - lo + 1));

    int prev = path[start - 1], next = path[start + L];
    array<int, 5> seg{};
    for (int i = 0; i < L; ++i) seg[i] = path[start + i];

    ll baseEval = 0, baseReal = 0;
    for (int i = 0; i < L; ++i) {
        baseEval += 1LL * (start + i) * evalW[seg[i]];
        baseReal += 1LL * (start + i) * realW[seg[i]];
    }

    array<int, 5> ord{};
    for (int i = 0; i < L; ++i) ord[i] = i;
    ll bestDe = LLONG_MIN, bestDr = 0;
    array<int, 5> bestOrd{};
    bool found = false;
    long long feasible = 0;

    sort(ord.begin(), ord.begin() + L);
    do {
        if (cheb_dist_idx(prev, seg[ord[0]], N) > 1) continue;
        bool ok = true;
        for (int i = 0; i + 1 < L; ++i) {
            if (cheb_dist_idx(seg[ord[i]], seg[ord[i + 1]], N) > 1) {
                ok = false;
                break;
            }
        }
        if (!ok) continue;
        if (cheb_dist_idx(seg[ord[L - 1]], next, N) > 1) continue;

        feasible++;
        ll ne = 0, nr = 0;
        for (int i = 0; i < L; ++i) {
            ne += 1LL * (start + i) * evalW[seg[ord[i]]];
            nr += 1LL * (start + i) * realW[seg[ord[i]]];
        }
        ll de = ne - baseEval;
        if (!found || de > bestDe) {
            found = true;
            bestDe = de;
            bestDr = nr - baseReal;
            bestOrd = ord;
        }
    } while (next_permutation(ord.begin(), ord.begin() + L));

    st.lns_feasible_perm_sum[ph] += feasible;
    if (!found) {
        st.lns_no_feasible[ph]++;
        return false;
    }
    if (!accept_move(bestDe, temp)) return false;

    array<int, 5> ns{};
    for (int i = 0; i < L; ++i) ns[i] = seg[bestOrd[i]];
    for (int i = 0; i < L; ++i) {
        path[start + i] = ns[i];
        pos[ns[i]] = start + i;
    }

    curEvalScore += bestDe;
    curRealScore += bestDr;
    st.lns_accepted[ph]++;
    if (bestDr > 0) st.lns_improve_real[ph]++;
    st.lns_acc_delta_real_sum[ph] += bestDr;
    return true;
}

bool validate_path(const vector<int>& path, int N) {
    int N2 = N * N;
    if ((int)path.size() != N2) return false;
    vector<unsigned char> used(N2, 0);
    for (int x : path) {
        if (x < 0 || x >= N2 || used[x]) return false;
        used[x] = 1;
    }
    for (int i = 0; i + 1 < N2; ++i) {
        if (cheb_dist_idx(path[i], path[i + 1], N) != 1) return false;
    }
    return true;
}

SearchStats run_local_search(const Candidate& cand, const Timer& timer, double endTime,
                             const vector<int>& A, const vector<int>& As, int N, int maxA, int maxS,
                             vector<int>& bestPathOut, ll& bestScoreOut) {
    SearchStats st;
    st.init_score = cand.score;
    st.final_score = cand.score;
    st.budget = max(0.0, endTime - timer.elapsed());

    vector<int> path = cand.path;
    vector<int> pos(N * N);
    for (int i = 0; i < N * N; ++i) pos[path[i]] = i;

    ll curEvalScore = compute_score(path, As);
    ll curRealScore = compute_score(path, A);
    ll bestRealScore = curRealScore;
    vector<int> bestPath = path;

    const double SWITCH_PROGRESS = 0.45;
    const double PHASE1_START_TEMP = 1.6e5;
    const double PHASE1_END_TEMP = 2.0e3;
    const double PHASE2_START_TEMP = 1.2e4;
    const double PHASE2_END_TEMP = 1.0e2;

    const vector<int>* evalW = &As;
    int maxEvalW = maxS;
    bool phase2 = false;
    double localStart = timer.elapsed();

    while (timer.elapsed() < endTime) {
        if ((st.iter & 511LL) == 0) {
            double localElapsed = timer.elapsed() - localStart;
            double progress = (st.budget > 1e-9 ? localElapsed / st.budget : 1.0);
            if (!phase2 && progress >= SWITCH_PROGRESS) {
                phase2 = true;
                evalW = &A;
                maxEvalW = maxA;
                curEvalScore = curRealScore;
                st.phase_switch_local = localElapsed;
            }
        }
        ++st.iter;

        double localElapsed = timer.elapsed() - localStart;
        double progress = (st.budget > 1e-9 ? min(1.0, localElapsed / st.budget) : 1.0);
        double temp;
        if (!phase2) {
            double p = min(1.0, progress / SWITCH_PROGRESS);
            temp = PHASE1_START_TEMP * pow(PHASE1_END_TEMP / PHASE1_START_TEMP, p);
        } else {
            double denom = max(1e-9, 1.0 - SWITCH_PROGRESS);
            double p = min(1.0, max(0.0, (progress - SWITCH_PROGRESS) / denom));
            temp = PHASE2_START_TEMP * pow(PHASE2_END_TEMP / PHASE2_START_TEMP, p);
        }

        bool accepted;
        uint32_t op = xor128() % 100u;
        if ((!phase2 && op < 94u) || (phase2 && op < 98u)) {
            accepted = try_two_opt(path, pos, *evalW, A, curEvalScore, curRealScore, temp, N, maxEvalW, phase2, st);
        } else {
            accepted = try_segment_rebuild(path, pos, *evalW, A, curEvalScore, curRealScore, temp, N, maxEvalW, phase2, st);
        }

        if (accepted && chmax(bestRealScore, curRealScore)) {
            bestPath = path;
            if (!phase2) st.best_updates_phase1++;
            else st.best_updates_phase2++;
        }
    }

    st.final_score = bestRealScore;
    bestPathOut = std::move(bestPath);
    bestScoreOut = bestRealScore;
    return st;
}

} // namespace

void solve()
{
    Timer timer;
    const double TIME_LIMIT = 2.95;

    int N;
    if (!(cin >> N)) return;
    int N2 = N * N;

    vector<int> A(N2), As(N2);
    int maxA = 0, maxS = 0;
    rep(i, N) rep(j, N) {
        int v; cin >> v;
        A[(int)i * N + (int)j] = v;
        chmax(maxA, v);
        ll sq = 1LL * v * v / max(1, N2);
        int s = (int)max(1LL, sq);
        As[(int)i * N + (int)j] = s;
        chmax(maxS, s);
    }

    array<pair<bool, bool>, 4> modes = {
        make_pair(false, true),
        make_pair(false, false),
        make_pair(true, true),
        make_pair(true, false)
    };

    vector<Candidate> cands;
    cands.reserve(8);
    rep(id, 4) {
        auto [a, b] = modes[(int)id];
        cands.push_back(build_orientation_rows(A, N, a, b, (int)id));
    }
    rep(id, 4) {
        auto [a, b] = modes[(int)id];
        cands.push_back(build_orientation_cols(A, N, a, b, (int)id));
    }

    double buildElapsed = timer.elapsed();

    vector<int> ord(cands.size());
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&](int x, int y) {
        return cands[x].score > cands[y].score;
    });

    vector<int> chosen;
    chosen.push_back(ord[0]);

    int otherFamily = 1 - cands[ord[0]].family;
    int second = -1;
    for (int idx : ord) {
        if (cands[idx].family == otherFamily) {
            second = idx;
            break;
        }
    }
    if (second == -1) second = ord[min(1, (int)ord.size() - 1)];
    if (second != chosen[0]) chosen.push_back(second);

    vector<SearchStats> restartStats;
    vector<int> globalBestPath = cands[chosen[0]].path;
    ll globalBestScore = cands[chosen[0]].score;

    for (int r = 0; r < (int)chosen.size(); ++r) {
        double now = timer.elapsed();
        if (now >= TIME_LIMIT - 0.005) break;

        double endTime;
        if (r + 1 == (int)chosen.size()) {
            endTime = TIME_LIMIT - 0.002;
        } else {
            double left = TIME_LIMIT - 0.005 - now;
            double frac = (r == 0 ? 0.58 : 1.0);
            endTime = now + left * frac;
        }

        vector<int> bestPathLocal;
        ll bestScoreLocal = -(1LL << 60);
        SearchStats st = run_local_search(cands[chosen[r]], timer, endTime, A, As, N, maxA, maxS, bestPathLocal, bestScoreLocal);
        restartStats.push_back(st);

        if (bestScoreLocal > globalBestScore) {
            globalBestScore = bestScoreLocal;
            globalBestPath = std::move(bestPathLocal);
        }
    }

#ifndef ONLINE_JUDGE
    cerr << fixed << setprecision(6);
    cerr << "build_elapsed=" << buildElapsed << '\n';
    for (int i = 0; i < (int)cands.size(); ++i) {
        cerr << "candidate idx=" << i
             << " family=" << cands[i].family
             << " mode=" << cands[i].mode
             << " score=" << cands[i].score << '\n';
    }
    cerr << "chosen_restarts=";
    for (int i = 0; i < (int)chosen.size(); ++i) {
        cerr << chosen[i] << (i + 1 == (int)chosen.size() ? '\n' : ' ');
    }

    for (int i = 0; i < (int)restartStats.size(); ++i) {
        const auto& st = restartStats[i];
        int idx = chosen[i];
        cerr << "restart=" << i
             << " candidate_idx=" << idx
             << " family=" << cands[idx].family
             << " mode=" << cands[idx].mode
             << " init_score=" << st.init_score
             << " final_score=" << st.final_score
             << " budget=" << st.budget
             << " phase_switch_local=" << st.phase_switch_local
             << " iter=" << st.iter
             << " best_updates_p1=" << st.best_updates_phase1
             << " best_updates_p2=" << st.best_updates_phase2 << '\n';

        for (int ph = 0; ph < 2; ++ph) {
            cerr << " restart=" << i
                 << " phase=" << ph + 1
                 << " twoopt_attempts=" << st.twoopt_attempts[ph]
                 << " accepted=" << st.twoopt_accepted[ph];
            if (st.twoopt_attempts[ph]) {
                cerr << " accept_rate=" << (double)st.twoopt_accepted[ph] / st.twoopt_attempts[ph]
                     << " no_candidate=" << st.twoopt_no_candidate[ph]
                     << " no_candidate_rate=" << (double)st.twoopt_no_candidate[ph] / st.twoopt_attempts[ph]
                     << " avg_candidate_count=" << (double)st.twoopt_candidate_sum[ph] / st.twoopt_attempts[ph]
                     << " avg_acc_delta_real=" << (st.twoopt_accepted[ph] ? (double)(st.twoopt_acc_delta_real_sum[ph] / st.twoopt_accepted[ph]) : 0.0);
            }
            cerr << '\n';

            cerr << " restart=" << i
                 << " phase=" << ph + 1
                 << " choose_best=" << st.twoopt_choose_best[ph]
                 << " choose_topk=" << st.twoopt_choose_topk[ph]
                 << " choose_random=" << st.twoopt_choose_random[ph]
                 << " real_improve=" << st.twoopt_improve_real[ph]
                 << " real_worse=" << st.twoopt_worse_real[ph] << '\n';

            cerr << " restart=" << i
                 << " phase=" << ph + 1
                 << " lns_attempts=" << st.lns_attempts[ph]
                 << " accepted=" << st.lns_accepted[ph];
            if (st.lns_attempts[ph]) {
                cerr << " accept_rate=" << (double)st.lns_accepted[ph] / st.lns_attempts[ph]
                     << " no_feasible=" << st.lns_no_feasible[ph]
                     << " avg_feasible_perm=" << (double)st.lns_feasible_perm_sum[ph] / st.lns_attempts[ph]
                     << " avg_acc_delta_real=" << (st.lns_accepted[ph] ? (double)(st.lns_acc_delta_real_sum[ph] / st.lns_accepted[ph]) : 0.0);
            }
            cerr << '\n';
        }
    }

    cerr << "final_best_score=" << globalBestScore << '\n';
    cerr << "valid=" << (validate_path(globalBestPath, N) ? 1 : 0) << '\n';
#endif

    for (int id : globalBestPath) {
        cout << id / N << ' ' << id % N << '\n';
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
gpt2
*/