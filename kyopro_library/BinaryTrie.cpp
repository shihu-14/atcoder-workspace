// mapping: xor, op: min
template<class T=int, int k=30>
struct BinaryTrie{
    struct Node{
        array<int, 2> to;
        int cnt, dp; // Edit here
        Node(): to({-1, -1}), cnt(0), dp(0) {}
    };
    vector<Node> d; 
    BinaryTrie(): d(1){}
    int go(int v, int x){
        if (d[v].to[x] == -1){
            d[v].to[x] = d.size();
            d.emplace_back(Node());
        }
        return d[v].to[x];
    }
    void add(T x, int a){
        int v = 0;
        vector<int> vs;
        rrep2(i, k, 0){
            d[v].cnt += a;
            vs.emplace_back(v);
            v = go(v, x>>i&1);
        }
        // Edit below 
        d[v].cnt += a;
        if (d[v].cnt == 1) d[v].dp = x;
        else d[v].dp = 0;
        reverse(rng(vs));
        for (int v: vs){
            if (d[v].cnt == 0) continue;
            else if (d[v].cnt == 1){
                rep(i, 2){
                    int u = d[v].to[i];
                    if (u != -1 && d[u].cnt == 1) d[v].dp = d[u].dp;
                }
            }
            else{
                d[v].dp = 1<<k; // Edit here(1U, 1LL, 1ULL..)
                rep(i, 2){
                    int u = d[v].to[i];
                    if (u != -1 && d[u].cnt >= 2){
                        chmin(d[v].dp, d[u].dp); // Edit here (op for dp)
                    }
                }
                if (d[v].dp == 1<<k){
                    d[v].dp = 0;
                    rep(i, 2) d[v].dp ^= d[d[v].to[i]].dp; // Edit here (op for node)
                }
            }
        }
    }
};
