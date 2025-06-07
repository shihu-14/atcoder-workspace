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
// using mint = modint;
using mint = modint998244353;
// using mint = modint1000000007;
using uint = unsigned int;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T3 = tuple<int, int, int>;
using G = vector<vector<int>>;
#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define rep2(i, a, b) for (ll i = (a); i < (b); ++i)
#define rrep2(i, a, b) for (ll i = (a)-1; i >= (b); --i)
#define rep3(i, a, b, c) for (ll i = (a); i < (b); i += (c))
#define rng(a) a.begin(), a.end()
#define rrng(a) a.rbegin(), a.rend()
#define popcount __builtin_popcount
#define popcount_ll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(rng(v))
#define MAX(v) *max_element(rng(v))
#define SUM(v) accumulate(rng(v), 0LL)
#define IN(v, x) (find(rng(v), x) != v.end())
template <class T> bool chmin(T &a, T b) { if (a > b) { a = b; return 1; } return 0; }
template <class T> bool chmax(T &a, T b) { if (a < b) { a = b; return 1; } return 0; }
template <class T> void printv(vector<T> &v) { rep(i, v.size()) cout << v[i] << " \n"[i == (ll)v.size() - 1]; }
template <class T> void printvv(vector<vector<T>> &v) { rep(i, v.size()) rep(j, v[i].size()) cout << v[i][j] << " \n"[j == (ll)v[i].size() - 1]; cout << endl; }
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 3001002003004005006ll;
const int INF = 1001001001;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K, H;
    int T_limit;
    int D;
    cin >> N >> K >> H >> T_limit >> D;

    vector<array<double, 3>> tube(K);
    rep(i, K) {
        cin >> tube[i][0] >> tube[i][1] >> tube[i][2];
    }

    vector<array<double, 3>> target(H);
    rep(i, H) {
        cin >> target[i][0] >> target[i][1] >> target[i][2];
    }

    rep(i, N) {
        rep(j, N - 1) {
            cout << 1 << (j == N - 2 ? '\n' : ' ');
        }
    }
    rep(i, N - 1) {
        rep(j, N) {
            cout << 1 << (j == N - 1 ? '\n' : ' ');
        }
    }

    vector<string> ops;
    ops.reserve(2 * H);

    int cell_idx = 0; 
    rep(idx, H) {
        int i = cell_idx / N;
        int j = cell_idx % N;
        cell_idx = (cell_idx + 1) % (N * N);

       
        int best_k = 0;
        double best_dist = 1e100;
        rep(k, K) {
            double dc = tube[k][0] - target[idx][0];
            double dm = tube[k][1] - target[idx][1];
            double dy = tube[k][2] - target[idx][2];
            double dist = dc * dc + dm * dm + dy * dy;
            if (dist < best_dist) {
                best_dist = dist;
                best_k = k;
            }
        }

       
        {
            ostringstream oss;
            oss << 1 << ' ' << i << ' ' << j << ' ' << best_k;
            ops.emplace_back(oss.str());
        }
       
        {
            ostringstream oss;
            oss << 2 << ' ' << i << ' ' << j;
            ops.emplace_back(oss.str());
        }
    }

    for (auto &s : ops) cout << s << '\n';

    return 0;
}
