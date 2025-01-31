// 無向連結グラフにおける橋と関節点の検出を行う構造体
struct Bridge{
    int n;
    vector<int> ord, low;
    vector<bool> art;
    vector<vector<int>> g;
    Bridge(int n) : n(n), ord(n, -1), low(n), art(n), g(n) {}
    void add_edge(int a, int b){
        g[a].emplace_back(b);
        g[b].emplace_back(a);
    }
    void dfs(int u, int p, int &c){
        low[u] = ord[u] = c++; // dfsで見た順番を記録。
        int t = 0; // 無向辺を正しく処理するための変数。
        bool isart = false; // 関節点かどうか
        for(auto v: g[u]){
            if (ord[v] == -1){
                dfs(v, u, c);
                chmin(low[u], low[v]);
                isart |= ord[v] <= low[u];
            } 
            else if (v != p){ // 後退辺によるlowの更新
                chmin(low[u], ord[v]);
            }
        }
        art[u] = isart;
    }
    void build(){
        int c = 0;
        rep(u, n) if (ord[u] == -1){
            int num_child = 0;
            low[u] = ord[u] = c++;
            for (auto v: g[u]) if (ord[v] == -1){ // uを根として、uの子であるvについてdfsを行う。
                dfs(v, u, c);
                num_child++;
            }
            if (num_child >= 2) art[u] = true; // 根である場合の関節点判定
        }
    }
    bool isbridge(int a, int b){
        if (ord[a] > ord[b]) swap(a, b);
        return ord[a] < low[b];
    }
    bool isart(int v){return art[v];}
};
