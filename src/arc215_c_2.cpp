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

void solve()
{
    int n; cin >> n;
    vector<vector<vector<pii>>> g(3, vector<vector<pii>>(n));
    vector<vector<vector<int>>> inv(3, vector<vector<int>>(n));
    int sx = 0, sy = 0, sz = 0;
    rep(i, n)
    {
        int x, y, z; cin >> x >> y >> z; x--, y--, z--;
        g[0][x].emplace_back(1, y);
        g[1][y].emplace_back(2, z);
        g[2][z].emplace_back(0, x);
        inv[0][x].emplace_back(i);
        inv[1][y].emplace_back(i);
        inv[2][z].emplace_back(i);
        chmax(sx, x);
        chmax(sy, y);
        chmax(sz, z);
    }
    rep(i, n-1)
    {
        g[0][i].emplace_back(0, i+1);
        g[1][i].emplace_back(1, i+1);
        g[2][i].emplace_back(2, i+1);
    }
    queue<pii> q;
    q.emplace(0, sx);
    q.emplace(1, sy);
    q.emplace(2, sz);
    vector<vector<bool>> dist(3, vector<bool>(n));
    dist[0][sx] = dist[1][sy] = dist[2][sz] = true;
    vector<bool> oks(n);
    while(!q.empty())
    {
        auto [d, u] = q.front(); q.pop();
        for (int i : inv[d][u])
        {
            oks[i] = true;
        }
        for (auto [nd, v] : g[d][u])
        {
            if (dist[nd][v]) continue;
            dist[nd][v] = true;
            q.emplace(nd, v);
        }
    }
    int ans = 0;
    rep(i, n) if (oks[i]) ans++;
    cout << ans << "\n";
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    cin >> t;
    while(t--)
    {
        solve();
    }
    return 0;
}