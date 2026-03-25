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
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 3001002003004005006ll;
const int INF = 1001001001;

// operations on Trees (Incomplete)
struct Tree{
    struct Edge{
        int to, w;
        Edge() {}
        Edge(int _to, int _w) : to(_to), w(_w) {}
    };
    int n;
    vector<int> deg, par, euler_tour, subtree_nodes;
    vector<ll> dist; 
    vector<pii> lr;
    vector<vector<Edge>> g;
    Tree(int n): n(n), g(n), deg(n), par(n, -1), dist(n), lr(n), subtree_nodes(n){}
    void add_edge(int u, int v, int c){
        g[u].emplace_back(v, c);
        g[v].emplace_back(u, c);
        deg[u]++, deg[v]++;
    }
    // solve EulerTour, depth, parent, and subtree size rooted at start
    void build(int root=0){
        ll s = 0;
        auto dfs = [&](auto f, int u, int p=-1) -> void {
            dist[u] = s;
            lr[u].fi = euler_tour.size();
            euler_tour.emplace_back(u);
            for (auto [v, w]: g[u]){
                if (v == p) par[u] = v;
                else{
                    s += w; f(f, v, u); s -= w;
                    euler_tour.emplace_back(u);
                    subtree_nodes[u] += subtree_nodes[v];
                }
            }
            subtree_nodes[u]++;
            lr[u].se = euler_tour.size();
        }; dfs(dfs, root);
    }
    // calc diameter on tree
    tuple<int, int, int> diameter(){
        int from = 0, to = 0; ll max_dist = 0;
        auto dfs = [&](auto f, int u, int p=-1, int s=0) -> void{
            if (g[u].size() == 1 && p != -1){
                if (max_dist < s) max_dist = s, to = u;
                return;
            }
            for (auto [v, w]: g[u])if (v != p) f(f, v, u, s+w);
        };
        dfs(dfs, from);
        from = to; max_dist = 0;
        dfs(dfs, from);
        return {max_dist, from, to};
    }
    // find tree centers (at most 2, -1 if only one)
    pii find_center(){
        int res_n = n;
        vector<int> tmp_deg = deg;
        queue<int> q;
        rep(i, n) if (deg[i] == 1) q.emplace(i); // initialize queue with deg1 vertices
        while(res_n > 2){
            res_n -= q.size();
            queue<int> tmp_q;
            while(!q.empty()){
                int u = q.front(); q.pop();
                for (auto [v, c]: g[u]){ // enqueue vertices that become deg1 after leaf removal
                    tmp_deg[v]--;
                    if (tmp_deg[v] == 1) tmp_q.emplace(v);
                }
            }
            q = move(tmp_q);
        }
        if (q.size() == 1) return {q.front(), -1};
        else return {q.front(), q.back()};
    }
    // find a centroid of the tree
    int find_centroid(){
        int centroid = -1;
        vector<int> sv(n);
        auto dfs = [&](auto f, int u, int p=-1) -> int{
            int mx = 0;
            for (auto [v, c]: g[u]) if (v != p){
                sv[u] += f(f, v, u);
                chmax(mx, sv[v]);
            }
            sv[u]++;
            chmax(mx, n-sv[u]);
            if (mx*2 <= n) centroid = u;
            return sv[u];
        }; dfs(dfs, 0);
        return centroid;
    }
    // calc inversion count when rooted at a given vertex
    vector<ll> inv_count(){
        ll inv = 0;
        { // calc inversion count when rooted at vertex '0'
            fenwick_tree<int> fw(n);
            auto dfs = [&](auto f, int u, int p=-1) -> void{
                inv += fw.sum(u+1, n);
                fw.add(u, 1);
                for (auto [v, _]: g[u])if (v != p) f(f, v, u);
                fw.add(u, -1);
            }; dfs(dfs, 0);
        }
        vector<pii> sn(n);
        { // u in subtree of v,  calc ([u < v], [u < par[v]])
            fenwick_tree<int> fw(n);
            auto dfs = [&](auto f, int u, int p=-1) -> void{
                if (p != -1) sn[u].fi -= fw.sum(0, u), sn[u].se -= fw.sum(0, p);
                fw.add(u, 1);
                for (auto [v, _]: g[u]) if (v != p) f(f, v, u);
                if (p != -1) sn[u].fi += fw.sum(0, u), sn[u].se += fw.sum(0, p);
            }; dfs(dfs, 0);
            rep(i, n) sn[i].fi = i-sn[i].fi;
        }
        vector<ll> res(n);
        { // calc inversion count by updating inv along DFS order
            auto dfs = [&](auto f, int u, int p=-1) -> void{
                inv += sn[u].fi-sn[u].se;
                res[u] = inv;
                for (auto [v, _]: g[u]) if (v != p){
                    f(f, v, u);
                    inv -= sn[v].fi-sn[v].se;
                }  
            }; dfs(dfs, 0);
        }
        return res;
    }
};


void solve()
{
    int n; cin >> n;
    Tree tree(n);
    rep(i, n-1)
    {
        int u, v; cin >> u >> v; u--, v--;
        tree.add_edge(u, v, 1);
    }
    tree.build();
    auto g = tree.find_centroid();

    vector<pii> ps;
    auto dfs = [&](auto f, int u, int p, int id) -> void
    {
        ps.emplace_back(id, u);
        for (auto [v, w]: tree.g[u])
        {
            if (v == p) continue;
            f(f, v, u, id);
        }
    };
    for (auto [v, w]: tree.g[g])
    {
        dfs(dfs, v, g, v);
    }
    sort(rng(ps));
    ps.emplace_back(g, g);

    vector<pii> ans;
    rep(i, n/2)
    {
        ans.emplace_back(ps[i].se, ps[i+n/2].se);
    }

    rep(i, ans.size())
    {
        auto [u, v] = ans[i];
        cout << u+1 << " " << v+1 << "\n";
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