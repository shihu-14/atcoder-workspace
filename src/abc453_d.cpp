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

const char dir[] = "URDL";
void solve()
{
    int h, w; cin >> h >> w;
    vector<string> s(h);
    rep(i, h) cin >> s[i];
    vector<vector<vector<pii>>> g(h*w, vector<vector<pii>>(4));
    rep(i, h)rep(j, w)
    {
        int v = i*w+j;
        if (s[i][j] == '#') continue;
        if (s[i][j] == 'S' || s[i][j] == 'G' || s[i][j] == '.')
        {
            rep(d, 4)
            {
                g[v][d].emplace_back(v, (d+1)%4);
            }
        }
        rep(d, 4)
        {
            int ni = i + dx[d], nj = j + dy[d];
            if (0 > ni || ni >= h || 0 > nj || nj >= w) continue;
            if (s[ni][nj] == '#') continue;
            int u = ni*w+nj;
            if (s[ni][nj] == '.' || s[ni][nj] == 'S' || s[ni][nj] == 'G')
            {
                g[v][d].emplace_back(u, d);
            }
            else if (s[ni][nj] == 'o')
            {
                g[v][d].emplace_back(u, d);
            }
            else if (s[ni][nj] == 'x')
            {
                rep(nd, 4)
                {
                    if (nd == d) continue;
                    g[v][d].emplace_back(u, nd);
                }
            }
        }
    }
    int si, sj, gi, gj;
    {
        rep(i, h)rep(j, w)
        {
            if (s[i][j] == 'S')
            {
                si = i; sj = j;
            }
            if (s[i][j] == 'G')
            {
                gi = i; gj = j;
            }
        }
    }
    vector<vector<int>> dist(h*w, vector<int>(4, INF));
    vector<vector<pii>> from(h*w, vector<pii>(4, {-1, -1}));
    queue<pii> q;
    rep(d, 4)
    {
        dist[si*w+sj][d] = 0;
        q.emplace(si*w+sj, d);
    }
    while(!q.empty())
    {
        auto [v, d] = q.front(); q.pop();
        for (auto [u, nd] : g[v][d])
        {
            if (dist[u][nd] != INF) continue;
            dist[u][nd] = dist[v][d] + 1;
            from[u][nd] = {v, d};
            q.emplace(u, nd);
        }
    }
    int ans = INF;
    int pv = gi*w+gj, pd = 0;
    rep(d, 4) 
    {
        if (chmin(ans, dist[pv][d]))
        {
            pd = d;
        }
    }
    if (ans == INF)
    {
        cout << "No" << endl;
        return;
    }
    cout << "Yes" << endl;
    vector<pii> path;
    path.emplace_back(pv/w, pv%w);
    while (true)
    {
        auto [v, d] = from[pv][pd];
        if (v == -1) break;
        path.emplace_back(v/w, v%w);
        pd = d;
        pv = v;
    }
    reverse(rng(path));
    string path_dir;
    int pi = path[0].fi, pj = path[0].se;
    rep(i, path.size())
    {
        int ni = path[i].fi, nj = path[i].se;
        int di = ni - pi, dj = nj - pj;
        if (di == 0 && dj == 0) continue;
        rep(d, 4)
        {
            if (di == dx[d] && dj == dy[d])
            {
                path_dir += dir[d];
                break;
            }
        }
        pi = ni; pj = nj;
    }
    cout << path_dir << endl;
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