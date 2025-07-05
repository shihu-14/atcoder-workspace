template<typename T>
struct Bell{
    int n; // 4213597(n=12), 27644437(n=13), 190899322(n=14)
    vector<T> a;
    // add other data
    unordered_set<T> res;
    Bell(vector<T> &a): n(a.size()), a(a) {}
    void build(){
        vector<vector<int>> groups;
        auto op = [&](){
            // Edit below
            ll xor_sum = 0;
            rep(i, groups.size()){
                ll s = 0;
                rep(j, groups[i].size()) s += a[groups[i][j]];
                xor_sum ^= s;
            }
            res.insert(xor_sum);
        };
        auto dfs = [&](auto f, int u=0) -> void{
            if (u == n){
                op();
                return;
            }
            rep(i, groups.size()){
                groups[i].emplace_back(u);
                f(f, u+1);
                groups[i].pop_back();
            }
            groups.emplace_back(vector<int>{u});
            f(f, u+1);
            groups.pop_back();
        };
        dfs(dfs);
    }
};
