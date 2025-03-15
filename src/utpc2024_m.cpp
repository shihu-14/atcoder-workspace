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

// https://beet-aizu.github.io/library/tree/heavylightdecomposition.cpp.html
// Heavy Light Decomposition
struct HLD {
    vector<vector<int>> g;
    vector<int> pos, head, sub, par, inv;
  
    HLD(int n): g(n), pos(n,-1), head(n), sub(n,1), par(n,-1), inv(n) {}
  
    void add_edge(int u,int v) {
      g[u].emplace_back(v);
      g[v].emplace_back(u);
    }
  
    void dfs(int v) {
      auto &es = g[v];
      if(par[v] != -1) es.erase(find(rng(es), par[v]));
      for(int &u : es){
        par[u] = v;
        dfs(u);
        sub[v] += sub[u];
        if(sub[u] > sub[es[0]]) swap(u, es[0]);
      }
    }
  
    void decompose(int v, int &i) {
      pos[v] = i++;
      inv[pos[v]] = v;
      for(int u: g[v]){
        if(u == par[v]) continue;
        head[u] = (u == g[v][0] ? head[v] : u);
        decompose(u, i);
      }
    }
  
    void build(int root=0) {
      int pos = 0;
      dfs(root);
      head[root] = root;
      decompose(root, pos);
    }
  
    int lca(int u,int v) {
      while(1){
        if (pos[u] > pos[v]) swap(u, v);
        if (head[u] == head[v]) return u;
        v = par[head[v]];
      }
    }
  
    template<typename F>
    void for_each(int u, int v, const F& f) {
      while(1){
        if (pos[u] > pos[v]) swap(u, v);
        f(max(pos[head[v]], pos[u]), pos[v]+1);
        if (head[u] != head[v]) v = par[head[v]];
        else break;
      }
    }
  
    template<typename F>
    void for_each_edge(int u, int v, const F& f) {
      while(1){
        if (pos[u] > pos[v]) swap(u, v);
        if (head[u] != head[v]){
          f(pos[head[v]], pos[v]+1);
          v = par[head[v]];
        } else {
          if(u != v) f(pos[u]+1, pos[v]+1);
          break;
        }
      }
    }
};

struct Edge{
    int u, v, w, id; 
    Edge() {}
    Edge(int u, int v, int w, int id): u(u), v(v), w(w), id(id) {}
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<Edge> edges, res;
    vector<bool> used(m);
    rep(i, m){
        int u, v, w; cin >> u >> v >> w; u--, v--;
        edges.emplace_back(u, v, w, i);
    }
    sort(rng(edges), [](Edge e1, Edge e2){
        return e1.w < e2.w;
    });
    dsu uf(n);
    HLD g(n);
    fenwick_tree<ll> fw(n);
    rep(i, m){
        auto [u, v, w, id] = edges[i];
        if (uf.same(u, v)){
            res.emplace_back(u, v, w, id);
        }
        else{
            uf.merge(u, v);
            g.add_edge(u, v);
            used[id] = true;
        }
    }
    g.build();
    for (auto [u, v, w, id] : edges) if (used[id]) {
        if (g.par[u] == v) fw.add(g.pos[u], w);
        else fw.add(g.pos[v], w);
    }

    for(auto [u, v, w, _]: res){
        ll d = 0;
        g.for_each_edge(u, v, [&](int l, int r){
            d += fw.sum(l, r);
        });
        if (d > w){
            cout << "No" << endl;
            return 0;
        }
    }
    cout << "Yes" << endl;
    return 0;
}