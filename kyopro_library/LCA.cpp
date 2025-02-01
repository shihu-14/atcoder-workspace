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
    void build(int root){
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
