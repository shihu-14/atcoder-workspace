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
    int h, w, n; cin >> h >> w >> n;
    int sx, sy, gx, gy; cin >> sx >> sy >> gx >> gy;
    sx--; sy--; gx--; gy--;
    map<int, vector<pii>> rows, cols;
    rep(i, n)
    {
        int r, c; cin >> r >> c;
        r--; c--;
        rows[r].emplace_back(c, i);
        cols[c].emplace_back(r, i);
    }
    // rep(i, h) sort(rng(rows[i]));
    // rep(i, w) sort(rng(cols[i]));
    for (auto &[_, rs]: rows) sort(rng(rs));
    for (auto &[_, cs]: cols) sort(rng(cs));
    int n2 = 4*n;
    vector<vector<pii>> g(n2);
    {
        for (auto [i, rs]: rows)
        {
            rep(j, (int)rs.size()-1)
            {
                auto [c1, id1] = rs[j];
                auto [c2, id2] = rs[j+1];
                c1++, c2--;
                // cout << i << " " << c1 << " " << id1 << " ,-> " << i << " " << c2 << " " << id2 << endl;
                if (c1 <= c2)
                {
                    g[4*id1+1].emplace_back(4*id2+3, c1!=c2);
                    g[4*id2+3].emplace_back(4*id1+1, c1!=c2);
                }
            }
        }
        for (auto [_, cs]: cols)
        {
            rep(i, (int)cs.size()-1)
            {
                auto [r1, id1] = cs[i];
                auto [r2, id2] = cs[i+1];
                r1++, r2--;
                if (r1 <= r2)
                {
                    g[4*id1+2].emplace_back(4*id2+0, r1!=r2);
                    g[4*id2+0].emplace_back(4*id1+2, r1!=r2);
                }
            }
        }
    }
    {
        for (auto [i, rs]: rows)
        {
            rep(j, (int)rs.size())
            {
                auto [c, id] = rs[j];
                int cr = c+1, cl = c-1;
                auto it = upper_bound(rng(cols[cr]), make_pair(i, -1));
                if (it != cols[cr].end())
                {
                    auto [r, id2] = *it;
                    r--;
                    if (i <= r)
                    {
                        // g[4*id+3].emplace_back(4*id2+0, r!=i);
                        // g[4*id2+0].emplace_back(4*id+3, r!=i);
                        g[4*id+1].emplace_back(4*id2+0, r!=i); 
                        g[4*id2+0].emplace_back(4*id+1, r!=i);
                    }
                }
                if (it != cols[cr].begin())
                {
                    it--;
                    auto [r, id2] = *it;
                    r++;
                    if (r <= i)
                    {
                        g[4*id+1].emplace_back(4*id2+2, r!=i);
                        g[4*id2+2].emplace_back(4*id+1, r!=i);
                        // g[4*id+3].emplace_back(4*id2+2, r!=i);
                        // g[4*id2+2].emplace_back(4*id+3, r!=i);
                    }
                }
                it = upper_bound(rng(cols[cl]), make_pair(i, -1));
                if (it != cols[cl].end())
                {
                    auto [r, id2] = *it;
                    r--;
                    if (i <= r)                    
                    {
                        g[4*id+3].emplace_back(4*id2+0, r!=i); 
                        g[4*id2+0].emplace_back(4*id+3, r!=i);
                        // g[4*id+1].emplace_back(4*id2+0, r!=i);
                        // g[4*id2+0].emplace_back(4*id+1, r!=i);
                    }   
                }
                if (it != cols[cl].begin())
                {
                    it--;
                    auto [r, id2] = *it;
                    r++;
                    if (r <= i)
                    {
                        g[4*id+3].emplace_back(4*id2+2, r!=i);
                        g[4*id2+2].emplace_back(4*id+3, r!=i);
                        // g[4*id+1].emplace_back(4*id2+2, r!=i);
                        // g[4*id2+2].emplace_back(4*id+1, r!=i);
                    }
                }
            }
        }
        for (auto [j, cs]: cols)
        {
            rep(i, (int)cs.size())
            {
                auto [r, id] = cs[i];
                int rd = r+1, ru = r-1;
                auto it = upper_bound(rng(rows[rd]), make_pair(j, -1));
                if (it != rows[rd].end())
                {
                    auto [c, id2] = *it;
                    c--;
                    if (j <= c)
                    {
                        g[4*id+2].emplace_back(4*id2+3, c!=j);
                        g[4*id2+3].emplace_back(4*id+2, c!=j);
                    }
                }
                if (it != rows[rd].begin())
                {
                    it--;
                    auto [c, id2] = *it;
                    c++;
                    if (c <= j)
                    {
                        g[4*id2+1].emplace_back(4*id+2, c!=j);
                        g[4*id+2].emplace_back(4*id2+1, c!=j);
                    }
                }
                it = upper_bound(rng(rows[ru]), make_pair(j, -1));
                if (it != rows[ru].end())
                {
                    auto [c, id2] = *it;
                    c--;
                    if (j <= c)          
                    {
                        g[4*id+0].emplace_back(4*id2+3, c!=j);
                        g[4*id2+3].emplace_back(4*id+0, c!=j);
                    }
                }
                if (it != rows[ru].begin())
                {
                    it--;
                    auto [c, id2] = *it;
                    c++;
                    if (c <= j)
                    {
                        g[4*id+0].emplace_back(4*id2+1, c!=j);
                        g[4*id2+1].emplace_back(4*id+0, c!=j);
                    }
                }
            }
        }
    }
    rep(i, n2)
    {
        rep(j, g[i].size())
        {
            auto [u, _] = g[i][j];
            // cout << i/4+1 << " " << i%4 << " -> " << u/4+1 << " " << u%4 << endl;
        }
    }
    vector<int> dist(n2, INF);
    deque<int> q;
    {
        auto it = upper_bound(rng(rows[sx]), make_pair(sy, -1));
        if (it != rows[sx].end())
        {
            auto [c, id] = *it;
            dist[4*id+3] = (c-1 != sy);
            if (c-1 == sy) q.emplace_front(4*id+3);
            else q.emplace_back(4*id+3);
        }
        if (it != rows[sx].begin())
        {
            it--;
            auto [c, id] = *it;
            dist[4*id+1] = (c+1 != sy);
            if (c+1 == sy) q.emplace_front(4*id+1);
            else q.emplace_back(4*id+1);
        }
        it = upper_bound(rng(cols[sy]), make_pair(sx, -1));
        if (it != cols[sy].end())
        {
            auto [r, id] = *it;
            dist[4*id+0] = (r-1 != sx);
            if (r-1 == sx) q.emplace_front(4*id+0);
            else q.emplace_back(4*id+0);
        }
        if (it != cols[sy].begin())
        {
            it--;
            auto [r, id] = *it;
            dist[4*id+2] = (r+1 != sx);
            if (r+1 == sx) q.emplace_front(4*id+2);
            else q.emplace_back(4*id+2);
        }
    }
    // printv(dist);
    while(q.size())
    {
        auto v = q.front(); q.pop_front();
        // cout << "visit " << v/4+1 << " " << v%4 << " dist = " << dist[v] << endl;
        for (auto [u, w] : g[v])
        {
            // cout << "  to " << u/4+1 << " " << u%4 << " dist = " << dist[u] << endl;
            if (dist[u] > dist[v]+w)
            {
                dist[u] = dist[v]+w;
                if (w == 0) q.emplace_front(u);
                else q.emplace_back(u);
            }
        }
    }
    // printv(dist);
    int ans = INF;
    {
        auto it = upper_bound(rng(rows[gx]), make_pair(gy, -1));
        if (it != rows[gx].end())
        {
            auto [c, id] = *it;
            if (c-1 == gy)
            {
                chmin(ans, dist[4*id+3]);
            }
        }
        if (it != rows[gx].begin())
        {
            it--;
            auto [c, id] = *it;
            if (c+1 == gy)
            {
                chmin(ans, dist[4*id+1]);
            }
        }
        it = upper_bound(rng(cols[gy]), make_pair(gx, -1));
        if (it != cols[gy].end())
        {
            auto [r, id] = *it;
            if (r-1 == gx)
            {
                chmin(ans, dist[4*id+0]);   
            }
        }
        if (it != cols[gy].begin())
        {
            it--;
            auto [r, id] = *it;
            if (r+1 == gx)
            {
                chmin(ans, dist[4*id+2]);
            }
        }
    }

    cout << (ans == INF ? -1 : ans) << endl;
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