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