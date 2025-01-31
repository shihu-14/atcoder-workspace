using S = pair<int, int>;
S op(S a, S b){return min(a, b);}
S e(){return {INF, 0};}

struct Edge{
    int to, cost, id; // costは適宜int->longlongに変更する
    Edge() {}
    Edge(int to, int cost, int id) : to(to), cost(cost), id(id) {}
};

// オイラーツアーとセグ木を用いてLCAを求める。
struct LCA{
    int n;
    vector<pair<int,int>> euler_tour;
    // ein,eoutはi番目の辺のEulertour上でのindexを表す。inが進む側、outが戻る側。
    vector<int> first_appear, dist, ein, eout, ew;
    vector<vector<Edge>> g;
    segtree<S, op, e> seg;
    fenwick_tree<ll> fw;
    LCA(int n) : n(n), first_appear(n), dist(n), g(n), seg(2*n-1), fw(2*n-1), ein(n-1), eout(n-1), ew(n-1){}
    void add_edge(int a, int b, int w, int id){
        g[a].emplace_back(b, w, id);
        g[b].emplace_back(a, w, id);
    }
    // オイラーツアーを求める。
    void dfs(int now, int pre=-1, int d=0){
        first_appear[now] = euler_tour.size();
        dist[now] = d;
        euler_tour.emplace_back(d, now);
        for (auto e: g[now]){
            if (e.to == pre) continue;
            ein[e.id] = euler_tour.size();
            ew[e.id] = e.cost;
            dfs(e.to, now, d+1);
            eout[e.id] = euler_tour.size();
            euler_tour.emplace_back(d, now);
        }
    };
    // 根を指定してオイラーツアー、seg木を初期化する。
    void init(int root){
        dfs(root);
        for(int i=0; i< euler_tour.size(); i++) seg.set(i, euler_tour[i]);
        for(int i=0; i< n-1; i++) fw.add(ein[i], ew[i]);
        for(int i=0; i< n-1; i++) fw.add(eout[i], -ew[i]);
    }
    // LCAを求める。
    int lca(int a, int b){
        int l = first_appear[a], r = first_appear[b];
        if (l > r) swap(l, r);
        return seg.prod(l, r+1).second;
    }
    // i番目の辺の重みをwに変更する。(適宜加算に変更)
    void renew(int i, int w) {
        int diff = w-ew[i]; ew[i] = w;
        fw.add(ein[i], diff); fw.add(eout[i], -diff);
    }
    // 任意の2点間の辺の数を求める。
    int length(int a, int b){return dist[a] + dist[b] - 2*dist[lca(a, b)];}
    // 任意の2点間の辺の重みの和を求める。
    ll length2(int a, int b){
        int c = lca(a, b); c = first_appear[c];
        int l = first_appear[a], r = first_appear[b];
        return fw.sum(0, l+1)+fw.sum(0, r+1)-2*fw.sum(0, c+1);
    }
    // ある点が指定したパス上(a,b)にあるかどうかの判定をする。
    bool is_on_pass(int a, int b, int c){return length(a, b) == length(a, c)+length(c, b);}
};
