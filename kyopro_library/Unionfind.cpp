// Union-find
struct UnionFind {
    vector<int> par, sz;
    UnionFind(int n=0): par(n, -1), sz(n, 1) {}
    // xの属する代表元を返す。
    int find(int x) {
        if(par[x] == -1) return x;
        return par[x] = find(par[x]);
    }
    // xとyが同じ連結成分かの判定
    bool same(int x, int y) {
        return find(x) == find(y);
    }
    // xとyをマージする。すでに同じ連結成分ならfalseを返す。マージに成功すればtrueを返す。
    // 新しい代表元は、サイズの大きい連結成分の代表元となる。
    bool merge(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) return false;
        if (sz[x] < sz[y]) swap(x, y);
        par[y] = x;
        sz[x] += sz[y]; 
        return true;
    }
    // xの属する連結成分のサイズを返す。
    int size(int x){
        return sz[find(x)];
    }
};
