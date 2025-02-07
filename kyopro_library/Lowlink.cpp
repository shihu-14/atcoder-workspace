// bridge and articulation detection
struct Lowlink{
    struct Edge{
        int to, id;
        Edge() {}
        Edge(int to, int id=-1): to(to), id(id) {}
    };
    int n, m=0;
    vector<int> par, ord, low, bc_id; // bc_id; edge set name after partition
    vector<bool> art;
    vector<vector<Edge>> g;
    Lowlink(int n): n(n), par(n, -1), ord(n,-1), low(n), art(n), g(n){}
    void add_edge(int a, int b){
        if (a > b) swap(a, b);
        g[a].emplace_back(b, m);
        g[b].emplace_back(a, m++);
    }
    void build(){
        int c = 0;
        auto dfs = [&](auto f, int v, int p=-1) -> void{
            low[v] = ord[v] = c++;
            bool isart = false;
            for(auto e: g[v]){
                if (ord[e.to] == -1){
                    par[e.to] = v;
                    f(f, e.to, v);
                    chmin(low[v], low[e.to]);
                    isart |= ord[v] <= low[e.to];
                } 
                else if (e.to != p){
                    chmin(low[v], ord[e.to]);
                }
            }
            art[v] = isart;
        };
        rep(v, n) if (ord[v] == -1){
            int num_child = 0;
            low[v] = ord[v] = c++;
            for (auto e: g[v]) if (ord[e.to] == -1){
                par[e.to] = v;
                dfs(dfs, e.to, v);
                num_child++;
            }
            if (num_child >= 2) art[v] = true;
        }
    }
    bool isbridge(int a, int b){
        if (ord[a] > ord[b]) swap(a, b);
        return ord[a] < low[b];
    }
    bool isart(int v){return art[v];}
    // edge set partition by articulation
    vector<vector<int>> bcc(){
        vector<vector<int>> bc;
        bc_id = vector<int>(m,-1);
        auto add = [&](int ei, int k) {
            bc[k].push_back(ei);
            bc_id[ei] = k;
        };
        auto dfs = [&](auto f, int v, int k=-1) -> void {
            for (auto e : g[v]) if (e.to != par[v]){
                if (par[e.to] == v) {
                    int nk = k;
                    if (low[e.to] >= ord[v]) nk = bc.size(), bc.emplace_back();
                    add(e.id, nk);
                    f(f, e.to, nk);
                }
                else if (ord[e.to] < ord[v]) add(e.id, k);
            }
        };
        rep(v, n) if (par[v] == -1) dfs(dfs, v);
        return bc;
    }
};
