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

struct XorShift64 {
    ull x;
    explicit XorShift64(ull seed = 88172645463393265ull) : x(seed) {}
    ull next_u64() {
        x ^= x << 7;
        x ^= x >> 9;
        return x;
    }
    uint next_uint(uint mod) {
        return static_cast<uint>(next_u64() % mod);
    }
    double next_double() {
        return (next_u64() >> 11) * (1.0 / 9007199254740992.0);
    }
};

struct Timer {
    chrono::steady_clock::time_point st;
    Timer() : st(chrono::steady_clock::now()) {}
    double elapsed_ms() const {
        return chrono::duration<double, milli>(chrono::steady_clock::now() - st).count();
    }
};

constexpr int TILE = 2;
int N_global = 0;
int M_global = 0;

inline int cell_id(int r, int c) {
    return r * N_global + c;
}
inline int cell_r(int id) {
    return id / N_global;
}
inline int cell_c(int id) {
    return id % N_global;
}
inline bool cell_adj(int a, int b) {
    int ra = cell_r(a), ca = cell_c(a);
    int rb = cell_r(b), cb = cell_c(b);
    return max(abs(ra - rb), abs(ca - cb)) == 1;
}
inline int tile_id(int tr, int tc) {
    return tr * M_global + tc;
}
inline int tile_r(int tid) {
    return tid / M_global;
}
inline int tile_c(int tid) {
    return tid % M_global;
}
inline bool tile_adj(int a, int b) {
    int ra = tile_r(a), ca = tile_c(a);
    int rb = tile_r(b), cb = tile_c(b);
    return max(abs(ra - rb), abs(ca - cb)) == 1;
}

ll compute_score(const vector<int>& path, const vector<int>& value) {
    ll sc = 0;
    const int n = (int)path.size();
    for (int i = 0; i < n; ++i) sc += 1LL * i * value[path[i]];
    return sc;
}

ll reversal_delta(const vector<int>& path, int l, int r, const vector<int>& value) {
    ll delta = 0;
    const int len = r - l + 1;
    for (int k = 0; k < len; ++k) {
        delta += 1LL * (len - 1 - 2 * k) * value[path[l + k]];
    }
    return delta;
}

int allowed_mask_from_dir(int dr, int dc) {
    if (dr == -1 && dc == -1) return 1 << 0;                 // top-left
    if (dr == -1 && dc ==  0) return (1 << 0) | (1 << 1);    // top side
    if (dr == -1 && dc ==  1) return 1 << 1;                 // top-right
    if (dr ==  0 && dc == -1) return (1 << 0) | (1 << 2);    // left side
    if (dr ==  0 && dc ==  1) return (1 << 1) | (1 << 3);    // right side
    if (dr ==  1 && dc == -1) return 1 << 2;                 // bottom-left
    if (dr ==  1 && dc ==  0) return (1 << 2) | (1 << 3);    // bottom side
    if (dr ==  1 && dc ==  1) return 1 << 3;                 // bottom-right
    return (1 << 4) - 1;
}

array<int, 4> best_tile_order_2x2(const array<int, 4>& vals, int startMask, int endMask) {
    ll best = -(1LL << 60);
    array<int, 4> bestOrder = {0, 1, 2, 3};

    for (int s = 0; s < 4; ++s) if (startMask & (1 << s)) {
        for (int e = 0; e < 4; ++e) if ((endMask & (1 << e)) && s != e) {
            vector<int> rem;
            for (int x = 0; x < 4; ++x) if (x != s && x != e) rem.push_back(x);
            if (rem.size() != 2) continue;
            if (vals[rem[0]] > vals[rem[1]]) swap(rem[0], rem[1]);
            ll cur = 1LL * vals[rem[0]] + 2LL * vals[rem[1]] + 3LL * vals[e];
            if (cur > best) {
                best = cur;
                bestOrder = {s, rem[0], rem[1], e};
            }
        }
    }

    if (best > -(1LL << 59)) return bestOrder;

    int s = 0;
    while (s < 4 && !(startMask & (1 << s))) ++s;
    if (s == 4) s = 0;
    int e = 0;
    while (e < 4 && (!(endMask & (1 << e)) || e == s)) ++e;
    if (e == 4) {
        e = 0;
        while (e == s) ++e;
    }
    vector<int> rem;
    for (int x = 0; x < 4; ++x) if (x != s && x != e) rem.push_back(x);
    if (vals[rem[0]] > vals[rem[1]]) swap(rem[0], rem[1]);
    return {s, rem[0], rem[1], e};
}

int improve_tile_path(vector<int>& path, const vector<int>& tileWeight, XorShift64& prng, const Timer& timer, double limit_ms) {
    int accepted = 0;
    const int K = (int)path.size();
    while (timer.elapsed_ms() < limit_ms) {
        int mode = (int)prng.next_uint(100);
        int len = 0;
        if (mode < 70) len = 2 + (int)prng.next_uint(32);
        else if (mode < 90) len = 2 + (int)prng.next_uint(128);
        else len = 2 + (int)prng.next_uint(512);
        if (len > K) len = K;
        int l = (int)prng.next_uint(K - len + 1);
        int r = l + len - 1;

        if (l > 0 && !tile_adj(path[l - 1], path[r])) continue;
        if (r + 1 < K && !tile_adj(path[l], path[r + 1])) continue;

        ll delta = reversal_delta(path, l, r, tileWeight);
        if (delta > 0) {
            reverse(path.begin() + l, path.begin() + r + 1);
            ++accepted;
        }
    }
    return accepted;
}

vector<int> expand_tiles_to_cells(const vector<int>& tilePath, const vector<int>& cellValueFlat) {
    const int K = (int)tilePath.size();
    vector<int> fullPath;
    fullPath.reserve(K * 4);

    const int ldr[4] = {0, 0, 1, 1};
    const int ldc[4] = {0, 1, 0, 1};

    for (int idx = 0; idx < K; ++idx) {
        int tid = tilePath[idx];
        int tr = tile_r(tid);
        int tc = tile_c(tid);

        int startMask = (1 << 4) - 1;
        int endMask = (1 << 4) - 1;

        if (idx > 0) {
            int pt = tilePath[idx - 1];
            int pdr = tile_r(pt) - tr;
            int pdc = tile_c(pt) - tc;
            startMask = allowed_mask_from_dir(pdr, pdc);
        }
        if (idx + 1 < K) {
            int nt = tilePath[idx + 1];
            int ndr = tile_r(nt) - tr;
            int ndc = tile_c(nt) - tc;
            endMask = allowed_mask_from_dir(ndr, ndc);
        }

        array<int, 4> vals = {
            cellValueFlat[cell_id(tr * 2 + 0, tc * 2 + 0)],
            cellValueFlat[cell_id(tr * 2 + 0, tc * 2 + 1)],
            cellValueFlat[cell_id(tr * 2 + 1, tc * 2 + 0)],
            cellValueFlat[cell_id(tr * 2 + 1, tc * 2 + 1)]
        };

        array<int, 4> ord = best_tile_order_2x2(vals, startMask, endMask);
        for (int k = 0; k < 4; ++k) {
            int loc = ord[k];
            int gr = tr * 2 + ldr[loc];
            int gc = tc * 2 + ldc[loc];
            fullPath.push_back(cell_id(gr, gc));
        }
    }
    return fullPath;
}

int improve_full_path(vector<int>& path, const vector<int>& cellValueFlat, XorShift64& prng, const Timer& timer, double limit_ms) {
    int accepted = 0;
    const int L = (int)path.size();
    while (timer.elapsed_ms() < limit_ms) {
        int mode = (int)prng.next_uint(100);
        int len = 0;
        if (mode < 80) len = 2 + (int)prng.next_uint(12);
        else if (mode < 95) len = 2 + (int)prng.next_uint(32);
        else len = 2 + (int)prng.next_uint(64);
        if (len > L) len = L;
        int l = (int)prng.next_uint(L - len + 1);
        int r = l + len - 1;

        if (l > 0 && !cell_adj(path[l - 1], path[r])) continue;
        if (r + 1 < L && !cell_adj(path[l], path[r + 1])) continue;

        ll delta = reversal_delta(path, l, r, cellValueFlat);
        if (delta > 0) {
            reverse(path.begin() + l, path.begin() + r + 1);
            ++accepted;
        }
    }
    return accepted;
}

bool validate_path(const vector<int>& path) {
    const int L = (int)path.size();
    if (L != N_global * N_global) return false;
    vector<unsigned char> seen(N_global * N_global, 0);
    for (int x : path) {
        if (x < 0 || x >= N_global * N_global) return false;
        if (seen[x]) return false;
        seen[x] = 1;
    }
    for (int i = 0; i + 1 < L; ++i) {
        if (!cell_adj(path[i], path[i + 1])) return false;
    }
    return true;
}

} // namespace

void solve()
{
    int N;
    cin >> N;
    N_global = N;
    M_global = N / TILE;

    vector<int> cellValueFlat(N * N);
    rep(i, N) rep(j, N) cin >> cellValueFlat[cell_id((int)i, (int)j)];

    vector<int> tileWeight(M_global * M_global, 0);
    rep(tr, M_global) rep(tc, M_global) {
        int sum = 0;
        rep(dr, 2) rep(dc, 2) {
            sum += cellValueFlat[cell_id((int)(tr * 2 + dr), (int)(tc * 2 + dc))];
        }
        tileWeight[tile_id((int)tr, (int)tc)] = sum;
    }

    vector<int> tilePath;
    tilePath.reserve(M_global * M_global);
    for (int tr = 0; tr < M_global; ++tr) {
        if ((tr & 1) == 0) {
            for (int tc = 0; tc < M_global; ++tc) tilePath.push_back(tile_id(tr, tc));
        } else {
            for (int tc = M_global - 1; tc >= 0; --tc) tilePath.push_back(tile_id(tr, tc));
        }
    }

    Timer timer;
    XorShift64 prng(chrono::steady_clock::now().time_since_epoch().count());

    const double coarse_limit_ms = 650.0;
    const double full_limit_ms = 2850.0;

#ifndef ONLINE_JUDGE
    vector<int> initTilePath = tilePath;
    vector<int> initPath = expand_tiles_to_cells(initTilePath, cellValueFlat);
    ll initScore = compute_score(initPath, cellValueFlat);
#endif

    int coarseAccepted = improve_tile_path(tilePath, tileWeight, prng, timer, coarse_limit_ms);

    vector<int> path = expand_tiles_to_cells(tilePath, cellValueFlat);

#ifndef ONLINE_JUDGE
    ll afterCoarseScore = compute_score(path, cellValueFlat);
#endif

    int fullAccepted = improve_full_path(path, cellValueFlat, prng, timer, full_limit_ms);

#ifndef ONLINE_JUDGE
    cerr << "elapsed_ms=" << fixed << setprecision(3) << timer.elapsed_ms() << '\n';
    cerr << "initial_score=" << initScore << '\n';
    cerr << "after_coarse_score=" << afterCoarseScore << '\n';
    cerr << "final_score=" << compute_score(path, cellValueFlat) << '\n';
    cerr << "coarse_accepted=" << coarseAccepted << '\n';
    cerr << "full_accepted=" << fullAccepted << '\n';
    cerr << "valid=" << (validate_path(path) ? 1 : 0) << '\n';
#endif

    for (int id : path) {
        cout << cell_r(id) << ' ' << cell_c(id) << '\n';
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