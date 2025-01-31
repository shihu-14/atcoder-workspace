// 木構造に関する操作 (未完成)
struct Tree{
    struct Edge{
        int to, cost;
        Edge() {}
        Edge(int to, int cost) : to(to), cost(cost) {}
    };
    int n;
    vector<int> deg;
    vector<int> par, tour;
    vector<ll> dist;
    vector<ll> subNode;
    vector<pii> lr;
    vector<vector<Edge>> g;
    Tree(int n): n(n), g(n), deg(n), par(n), dist(n), lr(n), subNode(n){}
    void add_edge(int u, int v, int c){
        g[u].emplace_back(v, c);
        g[v].emplace_back(u, c);
        deg[u]++, deg[v]++;
    }
    // 頂点startを根としたオイラーツアー、各頂点の根からの距離、親頂点を求める。
    void setting(int start=0){
        ll s = 0;
        auto f = [&](auto f, int u, int p=-1) -> void {
            dist[u] = s;
            lr[u].fi = tour.size();
            tour.emplace_back(u);
            for (auto [v, w]: g[u]){
                if (v == p){
                    par[u] = v;
                }
                else{
                    s += w;
                    f(f, v, u);
                    subNode[u] += subNode[v];
                    tour.emplace_back(u);
                    s -= w;
                }
            }
            subNode[u]++;
            lr[u].se = tour.size();
            return;
        };
        f(f, start);
    }
    // 木の直径を返す
    int find_diameter(){
        int from = 0, to = 0; ll max_dist = -LINF;
        auto f = [&](auto f, int u, int p=-1, int s=0) -> void{
            if (g[u].size() == 1 && p != -1){
                if (max_dist < s){
                    max_dist = s;
                    to = u;
                }
                return;
            }
            for (auto [v, c]: g[u]) if (v != p){
                f(f, v, u, s+c);
            }   
        };
        f(f, from); from = to;
        f(f, from);
        return max_dist;
    }
    // 木の中心を返す(高々２つなので、１つの場合は片方は-1となる)
    pii find_center(){
        int res_n = n;
        vector<int> tmp_deg = deg;
        vector<bool> used(n);
        queue<int> q;
        rep(i, n) if (tmp_deg[i] == 1) q.emplace(i); // 次数が1の頂点でキューを初期化
        while(res_n > 2){
            res_n -= q.size();
            queue<int> tmp_q;
            while(!q.empty()){
                int u = q.front(); q.pop();
                used[u] = true;
                for (auto [v, c]: g[u]){ // 葉を取り除いたことで次数が1になった頂点をキューに入れる
                    tmp_deg[v]--;
                    if (tmp_deg[v] == 1) tmp_q.emplace(v);
                }
            }
            q = move(tmp_q);
        }
        vector<int> res;
        rep(i, n) if (!used[i]) res.emplace_back(i);
        return (res.size() == 1 ? make_pair(res[0], -1) : make_pair(res[0], res[1]));
    }
    // 木の重心を１つ求める(本来は高々２つある)
    int find_centroid(){
        int centroid = -1;
        vector<int> sv(n);
        auto f = [&](auto f, int u, int p=-1) -> int{
            int mx = 0;
            for (auto [v, c]: g[u]) if (v != p){
                sv[u] += f(f, v, u);
                chmax(mx, sv[v]);
            }
            sv[u]++;
            chmax(mx, n-sv[u]);
            if (mx*2 <= n) centroid = u;
            return sv[u];
        };
        f(f, 0);
        return centroid;
    }
};
