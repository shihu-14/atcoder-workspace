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