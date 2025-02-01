struct AuxiliaryTree{
    int n;
    LCA lca;
    vector<int> in, out;
    AuxiliaryTree(int _n): n(_n), lca(n), in(n), out(n){}
    void add_edge(int a, int b){lca.add_edge(a, b);} 
    void build(int root=0){
        lca.build(root);
        int si = 0;
        auto dfs = [&](auto f, int u, int p=-1) -> void{
            in[u] = si++;
            for (int v: lca.g[u]) if (v != p) f(f, v, u);   
            out[u] = si;
        };
        dfs(dfs, root);
    }
    vector<pii> solve(vector<int> vs, int &root){ // return edges(parent, child)
        sort(rng(vs), [&](int i, int j){return in[i] < in[j];});
        int m = vs.size();
        rep(i, m-1) vs.emplace_back(lca.lca(vs[i], vs[i+1]));
        sort(rng(vs), [&](int i, int j){return in[i] < in[j];});
        vs.erase(unique(rng(vs)), vs.end());
        root = vs[0];
        vector<pii> res; vector<int> stk;
        for (int v: vs){
            while(stk.size() && (out[stk.back()] <= in[v] || in[v] <= in[stk.back()])) stk.pop_back();
            if (stk.size()) res.emplace_back(stk.back(), v);
            stk.emplace_back(v);
        }
        return res;
    }
};
