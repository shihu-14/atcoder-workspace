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

using namespace std;
using namespace atcoder;

using uint = unsigned int;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;

#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define rng(a) a.begin(),a.end()
#define chmax(a, b) ((a) < (b) ? (a) = (b), 1 : 0)

const int dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const int dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};

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

inline int cheb_dist_idx(int a, int b, int N) {
    int ar = a / N, ac = a % N;
    int br = b / N, bc = b % N;
    return max(abs(ar - br), abs(ac - bc));
}

vector<int> make_row_snake(int N, int dir_r, int dir_c) {
    vector<int> p;
    p.reserve(N * N);
    for (int i = 0; i < N; ++i) {
        int r = (dir_r == 0 ? i : N - 1 - i);
        if ((i & 1) == 0) {
            for (int j = 0; j < N; ++j) {
                int c = (dir_c == 0 ? j : N - 1 - j);
                p.push_back(r * N + c);
            }
        } else {
            for (int j = N - 1; j >= 0; --j) {
                int c = (dir_c == 0 ? j : N - 1 - j);
                p.push_back(r * N + c);
            }
        }
    }
    return p;
}

vector<int> make_col_snake(int N, int dir_r, int dir_c) {
    vector<int> p;
    p.reserve(N * N);
    for (int j = 0; j < N; ++j) {
        int c = (dir_c == 0 ? j : N - 1 - j);
        if ((j & 1) == 0) {
            for (int i = 0; i < N; ++i) {
                int r = (dir_r == 0 ? i : N - 1 - i);
                p.push_back(r * N + c);
            }
        } else {
            for (int i = N - 1; i >= 0; --i) {
                int r = (dir_r == 0 ? i : N - 1 - i);
                p.push_back(r * N + c);
            }
        }
    }
    return p;
}

ll compute_score(const vector<int>& path, const vector<int>& w) {
    ll s = 0;
    int n = (int)path.size();
    for (int i = 0; i < n; ++i) s += 1LL * i * w[path[i]];
    return s;
}

pair<ll, ll> calc_reverse_delta_both(const vector<int>& path, int l, int r, const vector<int>& evalW, const vector<int>& realW) {
    ll de = 0, dr = 0;
    int rem = (r - l + 1) / 2;
    for (int k = 0; k < rem; ++k) {
        int idx1 = l + k, idx2 = r - k;
        int a = path[idx1], b = path[idx2];
        ll coeff = (ll)(r - l - 2 * k);
        de += coeff * (evalW[a] - evalW[b]);
        dr += coeff * (realW[a] - realW[b]);
    }
    return {de, dr};
}

inline bool accept_move(ll delta, double temp) {
    if (delta >= 0) return true;
    if (temp <= 1e-12) return false;
    return exp((double)delta / temp) > (double)(xor128() % 1000000u) / 1000000.0;
}

int select_anchor_index(const vector<int>& path, const vector<int>& evalW) {
    int N2 = (int)path.size();
    uint32_t mode = xor128() % 100u;
    if (mode < 40) return (int)(xor128() % (uint32_t)(N2 - 1));
    if (mode < 75) {
        int bestIdx = (int)(xor128() % (uint32_t)(N2 - 1));
        int bestVal = evalW[path[bestIdx]];
        for (int t = 0; t < 8; ++t) {
            int idx = (int)(xor128() % (uint32_t)(N2 - 1));
            if (evalW[path[idx]] > bestVal) { bestVal = evalW[path[idx]]; bestIdx = idx; }
        }
        return bestIdx;
    }
    int bestIdx = (int)(xor128() % (uint32_t)(N2 - 1));
    int bestVal = evalW[path[bestIdx]];
    for (int t = 0; t < 8; ++t) {
        int idx = (int)(xor128() % (uint32_t)(N2 - 1));
        if (evalW[path[idx]] < bestVal) { bestVal = evalW[path[idx]]; bestIdx = idx; }
    }
    return bestIdx;
}

bool try_two_opt(vector<int>& path, vector<int>& pos, const vector<int>& evalW, const vector<int>& realW,
                 ll& curEvalScore, ll& curRealScore, double temp, int N) {
    int N2 = (int)path.size();
    int u_idx = select_anchor_index(path, evalW);
    int u = path[u_idx], nxt_u = path[u_idx + 1];
    int candJ[8], candCnt = 0;
    int ur = u / N, uc = u % N;
    for (int d = 0; d < 8; ++d) {
        int vr = ur + dxx[d], vc = uc + dyy[d];
        if (vr < 0 || vr >= N || vc < 0 || vc >= N) continue;
        int v = vr * N + vc, j = pos[v];
        if (j <= u_idx + 1) continue;
        if (j == N2 - 1 || cheb_dist_idx(nxt_u, path[j + 1], N) <= 1) candJ[candCnt++] = j;
    }
    if (candCnt == 0) return false;
    int j = candJ[xor128() % (uint32_t)candCnt], l = u_idx + 1, r = j;
    auto [deltaEval, deltaReal] = calc_reverse_delta_both(path, l, r, evalW, realW);
    if (!accept_move(deltaEval, temp)) return false;
    while (l < r) {
        swap(path[l], path[r]);
        pos[path[l]] = l; pos[path[r]] = r;
        ++l; --r;
    }
    curEvalScore += deltaEval; curRealScore += deltaReal;
    return true;
}

bool try_or_opt(vector<int>& path, vector<int>& pos, const vector<int>& evalW, const vector<int>& realW,
                ll& curEvalScore, ll& curRealScore, double temp, int N) {
    int N2 = (int)path.size();
    int L = 1 + (int)(xor128() % 3u);
    int u_idx = 1 + (int)(xor128() % (uint32_t)(N2 - L - 1));
    if (cheb_dist_idx(path[u_idx - 1], path[u_idx + L], N) > 1) return false;

    int cands[32], candCnt = 0;
    int u_first = path[u_idx], ur = u_first / N, uc = u_first % N;
    for (int d = 0; d < 8; ++d) {
        int vr = ur + dxx[d], vc = uc + dyy[d];
        if (vr < 0 || vr >= N || vc < 0 || vc >= N) continue;
        int v_idx = pos[vr * N + vc];
        if (v_idx < u_idx - 1 || v_idx > u_idx + L) {
            if (v_idx + 1 < N2 && cheb_dist_idx(path[u_idx + L - 1], path[v_idx + 1], N) <= 1) cands[candCnt++] = v_idx;
        }
    }
    if (candCnt == 0) return false;
    int v_idx = cands[xor128() % (uint32_t)candCnt];
    ll deltaEval = 0, deltaReal = 0;
    if (v_idx < u_idx) {
        for (int k = v_idx + 1; k < u_idx; ++k) { deltaEval += (ll)L * evalW[path[k]]; deltaReal += (ll)L * realW[path[k]]; }
        ll shift = (ll)(v_idx + 1 - u_idx);
        for (int k = u_idx; k < u_idx + L; ++k) { deltaEval += shift * evalW[path[k]]; deltaReal += shift * realW[path[k]]; }
    } else {
        for (int k = u_idx + L; k <= v_idx; ++k) { deltaEval -= (ll)L * evalW[path[k]]; deltaReal -= (ll)L * realW[path[k]]; }
        ll shift = (ll)(v_idx - L + 1 - u_idx);
        for (int k = u_idx; k < u_idx + L; ++k) { deltaEval += shift * evalW[path[k]]; deltaReal += shift * realW[path[k]]; }
    }
    if (!accept_move(deltaEval, temp)) return false;
    if (v_idx < u_idx) {
        rotate(path.begin() + v_idx + 1, path.begin() + u_idx, path.begin() + u_idx + L);
        for (int k = v_idx + 1; k < u_idx + L; ++k) pos[path[k]] = k;
    } else {
        rotate(path.begin() + u_idx, path.begin() + u_idx + L, path.begin() + v_idx + 1);
        for (int k = u_idx; k <= v_idx; ++k) pos[path[k]] = k;
    }
    curEvalScore += deltaEval; curRealScore += deltaReal;
    return true;
}

bool try_segment_rebuild(vector<int>& path, vector<int>& pos, const vector<int>& evalW, const vector<int>& realW,
                         ll& curEvalScore, ll& curRealScore, double temp, int N) {
    int N2 = (int)path.size();
    // 改善：L=7まで拡張
    int L = 4 + (int)(xor128() % 4u);
    if (N2 <= L + 2) return false;
    int start = 1 + (int)(xor128() % (uint32_t)(N2 - L - 1));
    int prev = path[start - 1], next = path[start + L];
    array<int, 7> seg{};
    for (int i = 0; i < L; ++i) seg[i] = path[start + i];
    ll baseEval = 0, baseReal = 0;
    for (int i = 0; i < L; ++i) { baseEval += 1LL * (start + i) * evalW[seg[i]]; baseReal += 1LL * (start + i) * realW[seg[i]]; }
    array<int, 7> ord{};
    for (int i = 0; i < L; ++i) ord[i] = i;
    ll bestDeltaEval = LLONG_MIN, bestDeltaReal = 0;
    array<int, 7> bestOrd{};
    bool found = false;
    sort(ord.begin(), ord.begin() + L);
    do {
        if (cheb_dist_idx(prev, seg[ord[0]], N) > 1) continue;
        bool ok = true;
        for (int i = 0; i + 1 < L; ++i) if (cheb_dist_idx(seg[ord[i]], seg[ord[i + 1]], N) > 1) { ok = false; break; }
        if (!ok || cheb_dist_idx(seg[ord[L - 1]], next, N) > 1) continue;
        ll newEval = 0, newReal = 0;
        for (int i = 0; i < L; ++i) { newEval += 1LL * (start + i) * evalW[seg[ord[i]]]; newReal += 1LL * (start + i) * realW[seg[ord[i]]]; }
        if (!found || (newEval - baseEval) > bestDeltaEval) { found = true; bestDeltaEval = newEval - baseEval; bestDeltaReal = newReal - baseReal; bestOrd = ord; }
    } while (next_permutation(ord.begin(), ord.begin() + L));
    if (!found) return false;
    bool is_same = true;
    for(int i = 0; i < L; ++i) if (bestOrd[i] != i) { is_same = false; break; }
    if (is_same || !accept_move(bestDeltaEval, temp)) return false;
    for (int i = 0; i < L; ++i) { path[start + i] = seg[bestOrd[i]]; pos[path[start + i]] = start + i; }
    curEvalScore += bestDeltaEval; curRealScore += bestDeltaReal;
    return true;
}

bool validate_path(const vector<int>& path, int N) {
    if ((int)path.size() != N * N) return false;
    vector<unsigned char> used(N * N, 0);
    for (int x : path) { if (x < 0 || x >= N * N || used[x]) return false; used[x] = 1; }
    for (int i = 0; i + 1 < N * N; ++i) if (cheb_dist_idx(path[i], path[i + 1], N) != 1) return false;
    return true;
}

}

void solve() {
    Timer timer;
    const double TIME_LIMIT = 2.95;
    int N; cin >> N;
    int N2 = N * N;
    vector<int> A(N2), A_sur(N2);
    rep(i, N) rep(j, N) {
        int v; cin >> v; A[i * N + j] = v;
        A_sur[i * N + j] = (int)max(1LL, 1LL * v * v / N2);
    }

    vector<vector<int>> base_paths;
    rep(dr, 2) rep(dc, 2) { base_paths.push_back(make_row_snake(N, dr, dc)); base_paths.push_back(make_col_snake(N, dr, dc)); }
    
    vector<int> path = base_paths[0];
    ll best_init_sur = -1;
    for (auto& bp : base_paths) {
        ll s = compute_score(bp, A_sur);
        if (chmax(best_init_sur, s)) path = bp;
    }

    vector<int> pos(N2); rep(i, N2) pos[path[i]] = i;
    vector<int> curEvalW = A_sur;
    ll curEvalScore = compute_score(path, curEvalW), curRealScore = compute_score(path, A);
    ll bestRealScore = curRealScore;
    vector<int> bestPath = path;

    // パラメータ：END_TEMPを少し上げ、Surrogateの維持を長くする
    const double START_TEMP = 1.0e6, END_TEMP = 200.0;
    long long iter = 0;
    int log_pct = 10;

    while (true) {
        if ((iter & 1023LL) == 0) {
            double elapsed = timer.elapsed();
            if (elapsed >= TIME_LIMIT) break;
            double progress = elapsed / TIME_LIMIT;
            // 改善：減衰タイミングを0.9まで遅らせる
            double w_sur = max(0.0, 1.0 - progress * 1.11);
            rep(i, N2) curEvalW[i] = A[i] + (int)(w_sur * (A_sur[i] - A[i]));
            curEvalScore = compute_score(path, curEvalW);

#ifndef ONLINE_JUDGE
            if (progress * 100.0 >= log_pct) {
                cerr << "[Log " << log_pct << "%] temp:" << fixed << setprecision(1) << START_TEMP * pow(END_TEMP / START_TEMP, progress) << " best:" << bestRealScore << endl;
                log_pct += 10;
            }
#endif
        }
        iter++;
        double temp = START_TEMP * pow(END_TEMP / START_TEMP, timer.elapsed() / TIME_LIMIT);
        uint32_t mode = xor128() % 100u;
        bool acc = false;
        if (mode < 45) acc = try_two_opt(path, pos, curEvalW, A, curEvalScore, curRealScore, temp, N);
        else if (mode < 80) acc = try_or_opt(path, pos, curEvalW, A, curEvalScore, curRealScore, temp, N);
        else acc = try_segment_rebuild(path, pos, curEvalW, A, curEvalScore, curRealScore, temp, N);

        if (acc) chmax(bestRealScore, curRealScore), bestPath = (bestRealScore == curRealScore ? path : bestPath);
    }

#ifndef ONLINE_JUDGE
    cerr << "Final Best: " << bestRealScore << " Iters: " << iter << " Valid: " << validate_path(bestPath, N) << endl;
#endif
    rep(i, N2) cout << bestPath[i] / N << " " << bestPath[i] % N << "\n";
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    solve();
    return 0;
}


/*
gimini

*/