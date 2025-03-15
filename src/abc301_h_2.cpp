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

// https://ei1333.github.io/library/structure/segment-tree/dual-segment-tree.hpp.html
template <typename Act>
struct DualSegmentTree {
  using F = typename Act::F;

 private:
  int sz, height;
  vector<F> lazy;
  Act m;

  inline void propagate(int k) {
    if (lazy[k] != m.id()) {
      lazy[2 * k + 0] = m.composition(lazy[2 * k + 0], lazy[k]);
      lazy[2 * k + 1] = m.composition(lazy[2 * k + 1], lazy[k]);
      lazy[k] = m.id();
    }
  }

  inline void thrust(int k) {
    for (int i = height; i > 0; i--) propagate(k >> i);
  }

 public:
  DualSegmentTree(Act m, int n) : m(m) {
    sz = 1;
    height = 0;
    while (sz < n) sz <<= 1, height++;
    lazy.assign(2 * sz, m.id());
  }

  void set(int k, const F &f) {
    thrust(k += sz);  
    lazy[k] = f;    
  }

  F get(int k) {
    thrust(k += sz);
    return lazy[k];
  }

  F operator[](int k) { return get(k); }

  void apply(int a, int b, const F &f) {
    thrust(a += sz);
    thrust(b += sz - 1);
    for (int l = a, r = b + 1; l < r; l >>= 1, r >>= 1) {
      if (l & 1) lazy[l] = m.composition(lazy[l], f), ++l;
      if (r & 1) --r, lazy[r] = m.composition(lazy[r], f);
    }
  }
};

struct Act{
  // Edit here
  using F = int;
  static constexpr F composition(const F &f, const F &g) {return min(f, g);}
  static constexpr F id() {return INF;}
};

// LCA by Euler Tour and Segtree
struct LCA{
    using S = pair<int, int>;
    static S op(S a, S b){return min(a, b);}
    static S e(){return {INF, 0};}

    vector<S> euler_tour;
    vector<int> first_appear, dist;
    vector<vector<int>> g;
    segtree<S, op, e> seg;
    LCA(int n) : first_appear(n), dist(n), g(n), seg(2*n-1){}
    void add_edge(int a, int b){
        g[a].push_back(b);
        g[b].push_back(a);
    }
    void build(int root=0){
        auto dfs = [&](auto f, int u, int p=-1, int d=0) -> void{ // solve Euler Tour
            first_appear[u] = euler_tour.size();
            dist[u] = d;
            euler_tour.emplace_back(d, u);
            for (auto v: g[u]){
                if (v == p) continue;
                f(f, v, u, d+1);
                euler_tour.emplace_back(d, u);
            }
        };
        dfs(dfs, root);
        rep(i, euler_tour.size()) seg.set(i, euler_tour[i]);
    }
    int lca(int a, int b){ 
        int l = first_appear[a], r = first_appear[b];
        if (l > r) swap(l, r);
        return seg.prod(l, r+1).second;
    }
    int length(int a, int b){return dist[a]+dist[b]-2*dist[lca(a, b)];}
    bool is_on_pass(int a, int b, int c){return length(a, b) == length(a, c)+length(c, b);} // c is on the path a-b
};


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

int op(int a, int b){return max(a, b);}
int e(){return 0;}

struct Edge{
    int u, v, w, id; 
    Edge() {}
    Edge(int _u, int _v, int _w, int _id) : u(_u), v(_v), w(_w), id(_id) {}
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, q; cin >> n >> m;
    vector<bool> mst_used(m);
    vector<Edge> edges;
    dsu uf(n);
    HLD g(n);
    LCA g2(n);
    rep(i, m){
        int a, b, c; cin >> a >> b >> c; a--, b--;
        edges.emplace_back(a, b, c, i);
    }
    sort(rng(edges), [](Edge e1, Edge e2){
        return e1.w < e2.w;
    });
    vector<int> id(m);
    {
        int i = 0;
        for(auto [a, b, c, j]: edges){
            // printf("%d-%d-%d\n", a+1, b+1, c);
            id[j] = i++;
            // printf("%d->%d\n", j, i);
            if (uf.same(a, b)) continue;
            mst_used[j] = true;
            // printf("%d, %d, %d\n", a+1, b+1, c);
            uf.merge(a, b);
            g.add_edge(a, b);
            g2.add_edge(a, b);
        }
    }
    g.build();
    g2.build();
    segtree<int, op, e>  seg(n);
    fenwick_tree<int> fw(n);
    DualSegmentTree seg2(Act(), n);
    vector<int> id2(m, -1);
    {    
        rep(i, m){
            auto [u, v, w, j] = edges[i];
            if (!mst_used[j]) continue;
            if (g.par[u] == v){
                seg.set(g.pos[u], w);
                fw.add(g.pos[u], 1);
                id2[j] = g.pos[u];
            }
            else{
                seg.set(g.pos[v], w);
                fw.add(g.pos[v], 1);
                id2[j] = g.pos[v];
            }
        }
        rep(i, m){
            auto [a, b, c, j] = edges[i];
            if (mst_used[j]) continue;
            // printf("%d, %d, %d\n", a+1, b+1, c);
            int res = 0;
            g.for_each_edge(a, b, [&](int l, int r){
                chmax(res, seg.prod(l, r));
            });
            // printf("res: %d\n", res);
            g.for_each_edge(a, b, [&](int l, int r){
                seg2.apply(l, r, res);
            });
        }

    }
    // rep(i, n) printf("%d ", g.pos[i]); printf("\n");
    // rep(i, n) printf("%d ", seg.get(i)); printf("\n");
    // rep(i, n) printf("%d ", seg2.get(i)); printf("\n");
    cin >> q;
    rep(qi, q){
        int a, s, t; cin >> a >> s >> t; a--, s--, t--;
        // printf("a=%d, id[a]=%d, id2[a]=%d\n", a, id[a], id2[a]);
        auto [u, v, w, j] = edges[id[a]];
        // printf("%d, %d, %d, %d / pos = %d, %d\n", u+1, v+1, w, j, g.pos[s], g.pos[t]);
        
        // auto is_on_pass = [&](int x, int y, int z) -> bool{
        //     int dxy = 0, dxz = 0, dyz = 0;
        //     g.for_each_edge(x, y, [&](int l, int r){
        //         dxy += fw.sum(l, r);    
        //     });
        //     g.for_each_edge(x, z, [&](int l, int r){
        //         dxz += fw.sum(l, r);    
        //     });
        //     g.for_each_edge(y, z, [&](int l, int r){
        //         dyz += fw.sum(l, r);    
        //     });
        //     return dxy == dxz+dyz;
        // };

        // printf("on-pass: (%d, %d)\n", is_on_pass(s, t, u), is_on_pass(s, t, v));
        
        if (!mst_used[a] || !g2.is_on_pass(s, t, u) || !g2.is_on_pass(s, t, v)){
            cout << 0 << endl;
            continue;
        }
        int res = 0;
        g.for_each_edge(s, t, [&](int l, int r){
            chmax(res, seg.prod(l, r));
        });
        // printf("seg2[a]= %d, s-t: %d\n", res, seg2.get(id2[a]));
        if (res == w && res < seg2.get(id2[a])){
            cout << 1 << endl;
        }
        else{
            cout << 0 << endl;
        }
    }
    return 0;
}