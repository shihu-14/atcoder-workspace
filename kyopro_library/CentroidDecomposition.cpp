struct CentroidDecomposition{
    int n;
    vector<vector<int>> g;
    vector<bool> centroid_used;
    vector<int> subtree_size;
    // Edit here (define required data structures)
    ll ret = 0;
    vector<int> a;
    
    CentroidDecomposition(int n): n(n), g(n), centroid_used(n), subtree_size(n), a(n){}
    void add_edge(int u, int v){
        g[u].emplace_back(v);
        g[v].emplace_back(u);
    }
    void add_data(){
        // Edit here (input required data)
        rep(i, n) cin >> a[i];
    }
    void CD(int u){
        auto getCentroid = [&]() -> int{
            auto dfs = [&](auto f, int u, int p=-1) -> int{ // calc subtree size
                subtree_size[u] = 1;
                for (int v: g[u]){
                    if (v == p || centroid_used[v]) continue;
                    subtree_size[u] += f(f, v, u);
                }
                return subtree_size[u];
            };
            int m = dfs(dfs, u), centorid = -1;
            auto dfs2 = [&](auto f, int u, int p=-1) -> void{ // find centroid
                bool flag = true;
                if (m-subtree_size[u] > m/2) flag = false;
                for (int v: g[u]){
                    if (v == p || centroid_used[v]) continue;
                    f(f, v, u);
                    if (subtree_size[v] > m/2) flag = false;
                }
                if (flag){
                    centorid = u;
                    return;
                }
            };
            dfs2(dfs2, u);
            return centorid;
        };
        int centorid = getCentroid();
        centroid_used[centorid] = true;

        // Edit here (describe the process for the centroid-decomposed subtree)
        /* sample abc359-G
        map<int, ll> mc, ms;
        mc[a[centorid]] = 1, ms[a[centorid]] = 0;
        for (int v: g[centorid]){
            if (centroid_used[v]) continue;
            vector<pii> ps;
            auto dfs = [&](auto f, int u, int p=-1, int d=1) -> void{
                ps.emplace_back(a[u], d);
                for (int v: g[u]){
                    if (v == p || centroid_used[v]) continue;
                    f(f, v, u, d+1);
                }
            };
            dfs(dfs, v);
            for (auto [c, d]: ps){
                ret += ms[c]+mc[c]*d;
            }
            for (auto [c, d]: ps){
                mc[c]++;
                ms[c] += d;
            }
        }
        */
        for (int v: g[centorid]){ // decompose about the centroid
            if (centroid_used[v]) continue;
            CD(v);
        }
    }
    ll solve(){
        CD(0);
        return ret;
    }
};
