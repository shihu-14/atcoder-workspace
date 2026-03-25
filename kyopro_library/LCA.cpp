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
