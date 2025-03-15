// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
// #include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
// using mint = modint1000000007;
// using namespace boost::multiprecision;
using uint = unsigned int;
using ll = long long;
// using l3 = __int128;
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
int rand(){static random_device rd; static mt19937 mt(rd()); static uniform_int_distribution<int> dist(0, INF); return dist(mt);}
pll op(pll a, pll b){return {a.fi+b.fi, a.se+b.se};}
pll e(){return {0, 0};}
pll mapping(ll f, pll x){return {f*x.se+x.fi, x.se};}
ll composition(ll f, ll g){return f+g;}
ll id(){return 0;}

//  Heavy Light Decomposition
struct HLD {
    int n;
    vector<vector<int>> g; 
    vector<int> par, depth, subsize, heavy, head, pos;
    int cur;
    // add here (segtree, fenwicktree..)
    lazy_segtree<pll, op, e, ll, mapping, composition, id> seg;
    HLD(int _n) : n(_n), g(_n), par(_n, -1), depth(_n, 0),
        subsize(_n, 0), heavy(_n, -1), head(_n), pos(_n), cur(0), seg(_n){
            rep(i, n) seg.set(i, {0,1});
        }

    void add_edge(int u, int v) {
        g[u].push_back(v); 
        g[v].push_back(u);
    }

    int dfs(int v, int p=-1) {
        par[v] = p;
        subsize[v] = 1;
        int max_subtree = 0;
        for (auto u: g[v]) if (u != p){
            depth[u] = depth[v] + 1;
            int sz = dfs(u, v);
            if (sz > max_subtree){
                max_subtree = sz;
                heavy[v] = u;
            }
            subsize[v] += sz;
        }
        return subsize[v];
    }

    void decompose(int v, int h) {
        head[v] = h;
        pos[v] = cur++;
        if (heavy[v] != -1) {
            decompose(heavy[v], h);
        }
        for (auto u : g[v]) {
            if (u == par[v] || u == heavy[v]) continue;
            decompose(u, u);
        }
    }
    void build(int root=0) {
        dfs(root);
        decompose(root, root);
    }

    ll solve(int u, int v) { //** Edit return type
        ll res = 0;
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) swap(u, v);
            int L = pos[head[u]], R = pos[u]+1; // [L, R)
            seg.apply(L, R, 1);
            res += seg.prod(L, R).fi;
            u = par[head[u]];
        }
        if (depth[u] < depth[v]) swap(u, v);
        int L = pos[v], R = pos[u]+1;
        seg.apply(L, R, 1);
        res += seg.prod(L, R).fi;
        return res;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    HLD g(n);
    rep(i, n-1){
        int u, v; cin >> u >> v; u--, v--;
        g.add_edge(u, v);
    }
    g.build();
    int q; cin >> q;
    ll ans = 0;
    rep(qi, q){
        int u, v; cin >> u >> v; u--, v--;
        ans += g.solve(u, v);
    }
    cout << ans << endl;
    return 0;
}