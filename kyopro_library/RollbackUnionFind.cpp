struct RollbackUnionFind {
    vector<int> d, snap; // d: parent or -size
    vector<pii> hist;
    RollbackUnionFind(int n=0): d(n,-1) {}
    int find(int x) {
        if(d[x] < 0) return x;
        return find(d[x]);
    }
    bool same(int x, int y) {
        return find(x) == find(y);
    }
    bool merge(int x, int y) { // if merge successed, return true
        x = find(x);
        y = find(y);
        if (x == y) return false;
        if (d[x] > d[y]) swap(x, y);
        hist.emplace_back(x, d[x]);
        hist.emplace_back(y, d[y]);
        d[x] += d[y];
        d[y] = x;
        return true;
    }
    int size(int x){
        return -d[find(x)];
    }
    void snapshot(){ // save the history from this point onward
        snap.emplace_back(hist.size());
    }
    void rollback(){
        while (hist.size() > snap.back()){
            auto [i, sz] = hist.back(); hist.pop_back();
            d[i] = sz;
        }
        snap.pop_back();
    }
};
