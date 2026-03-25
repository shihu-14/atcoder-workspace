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
template<class T=int>
using G = vector<vector<T>>;
#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define rep2(i, a, b) for (ll i = a; i < (b); ++i)
#define rrep2(i, a, b) for (ll i = a-1; i >= (b); --i)
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
const char dc[] = {'U', 'R', 'D', 'L'};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 3001002003004005006ll;
const int INF = 1001001001;
using T5 = tuple<int, int, int, int, char>;
void solve()
{
    int n, K, T; cin >> n >> K >> T;
    vector<string> v(n), h(n-1);
    rep(i, n) cin >> v[i];
    rep(i, n-1) cin >> h[i];
    vector<pii> gv(K);
    rep(i, K) cin >> gv[i].fi >> gv[i].se;
    vector<vector<vector<int>>> g(n, vector<vector<int>>(n));
    int sx = gv[0].fi, sy = gv[0].se;
    rep2(t, 1, K)
    {
        int gx = gv[t].fi, gy = gv[t].se;
        queue<pii> q;
        vector<vector<int>> dist(n, vector<int>(n, INF));
        vector<vector<pii>> from(n, vector<pii>(n, {-1, -1}));
        q.emplace(sx, sy);
        dist[sx][sy] = 0;
        while(!q.empty())
        {
            auto [x, y] = q.front(); q.pop();
            if (x == gx && y == gy) break;
            rep(d, 4)
            {
                int nx = x + dx[d], ny = y + dy[d];
                if (nx < 0 || nx >= n || ny < 0 || ny >= n) continue;
                if (d == 0 && h[nx][ny] == '1') continue;
                if (d == 1 && v[x][y] == '1') continue;
                if (d == 2 && h[x][y] == '1') continue;
                if (d == 3 && v[nx][ny] == '1') continue;
                if (dist[nx][ny] != INF) continue;
                dist[nx][ny] = dist[x][y] + 1;
                from[nx][ny] = {x, y};
                q.emplace(nx, ny);
            }
        }
        {
            int x = gx, y = gy;
            vector<T3> path;
            while(x != sx || y != sy)
            {
                auto [px, py] = from[x][y];
                auto calc_dir = [&](int x, int y, int nx, int ny)
                {
                    int dx = nx - x, dy = ny - y;
                    if (dx == -1 && dy == 0) return 0; 
                    if (dx == 0  && dy == 1)  return 1; 
                    if (dx == 1  && dy == 0)  return 2; 
                    if (dx == 0  && dy == -1) return 3; 
                };
                int d = calc_dir(px, py, x, y);
                x = px, y = py;
                path.emplace_back(x, y, d);
            }
            for (auto  [x, y, d]: path)
            {
                g[x][y].emplace_back(d);
            }
        }
        sx = gx, sy = gy;
    }
    {
        rep(i, n)rep(j, n)
        {
            reverse(rng(g[i][j]));
        }
    }
    vector<vector<int>> col(n, vector<int>(n, 0));
    { // init col
        rep(i, n)rep(j, n) if (g[i][j].size())
        {
            col[i][j] = g[i][j].back();
            g[i][j].pop_back();
        }
    }
    auto init_col = col;
    vector<T5> qv;
    {
        int x = gv[0].fi, y = gv[0].se;
        rep2(i, 1, K)
        {
            int sx = x, sy = y;
            int gx = gv[i].fi, gy = gv[i].se;
            while(x != gx || y != gy)
            {
                int d = col[x][y];
                int nx = x + dx[d], ny = y + dy[d];
                int ncol = 0;
                if (g[x][y].size())
                {
                    ncol = g[x][y].back();
                    g[x][y].pop_back();
                }
                if (x == sx && y == sy && i-2 >= 0) qv.emplace_back(col[x][y], i-2, ncol, i-1, dc[d]);
                else qv.emplace_back(col[x][y], i-1, ncol, i-1, dc[d]);
                col[x][y] = ncol;
                x = nx, y = ny;
            }
        }
    }
    UNIQUE(qv);
    cout << 4 << " " << K-1 << " " << qv.size() << endl;
    rep(i, n)
    {
        rep(j, n) cout << init_col[i][j] << " ";
        cout << endl;
    }
    for (auto [c, q, a, s, d]: qv)
    {
        cout << c << " " << q << " " << a << " " << s << " " << d << endl;
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