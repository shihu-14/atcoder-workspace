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

// LCA by Euler Tour and Segtree
struct LCA
{
    using S = pair<int, int>;
    static S op(S a, S b){return min(a, b);}
    static S e(){return {INF, 0};}
    int n, log_n;
    vector<S> euler_tour;
    vector<int> first_appear, dist, sz;
    vector<vector<int>> g, dp_parent;
    segtree<S, op, e> seg;
    LCA(int _n) : n(_n), first_appear(_n), dist(_n), g(_n), seg(2*_n-1), sz(_n)
    {
        log_n = 0;
        while ((1<<log_n) < n) log_n++;
        dp_parent.assign(log_n, vector<int>(n, -1));
    }
    void add_edge(int a, int b)
    {
        g[a].emplace_back(b);
        g[b].emplace_back(a);
    }
    void build(int root=0)
    {
        auto dfs = [&](auto f, int u, int p=-1, int d=0) -> void
        { // solve Euler Tour
            first_appear[u] = euler_tour.size();
            dist[u] = d;
            dp_parent[0][u] = p;
            euler_tour.emplace_back(d, u);
            sz[u] = 1;
            for (auto v: g[u])
            {
                if (v == p) continue;
                f(f, v, u, d+1);
                sz[u] += sz[v];
                euler_tour.emplace_back(d, u);
            }
        };
        dfs(dfs, root);
        rep(i, euler_tour.size()) seg.set(i, euler_tour[i]);
        rep(k, log_n-1)
        {
            rep(v, n)
            {
                if (dp_parent[k][v] == -1) dp_parent[k+1][v] = -1;
                else dp_parent[k+1][v] = dp_parent[k][dp_parent[k][v]];
            }
        }
    }
    int get_lca(int a, int b)
    { 
        int l = first_appear[a], r = first_appear[b];
        if (l > r) swap(l, r);
        return seg.prod(l, r+1).second;
    }
    int get_length(int a, int b){return dist[a]+dist[b]-2*dist[get_lca(a, b)];}
    bool is_on_pass(int a, int b, int c){return get_length(a, b) == get_length(a, c)+get_length(c, b);} // c is on the path a-b
    int get_kth_ancestor(int u, int k) // 頂点uのk個上の祖先
    {
        for (int i = 0; i < log_n; i++)
        {
            if ((k >> i) & 1)
            {
                u = dp_parent[i][u];
                if (u == -1) return -1;
            }
        }
        return u;
    }
};

/* testcase
abc438_f

*/


void solve()
{
    int n, q; cin >> n >> q;
    LCA lca(n);
    rep2(i, 1, n)
    {
        int p; cin >> p; p--;
        lca.add_edge(i, p);
    }
    lca.build();
    rep(i, q)
    {
        int x, y; cin >> x >> y; x--; y--;
        cout << lca.get_lca(x, y)+1 << endl;
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