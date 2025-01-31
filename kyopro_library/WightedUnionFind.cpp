// 重み付きUnion-Find
struct WightedUnionFind {
	vector<int> par, sz;
	vector<ll> diff_weight; // 根からの重み
	WightedUnionFind(int n) : par(n, -1), sz(n, 1), diff_weight(n, 0) {}
    // xの属する代表元を返す。
	int find(int x) {
		if (par[x] == -1) return x;
		int root = find(par[x]); // 再帰的に呼び出すことで、
        diff_weight[x] += diff_weight[par[x]]; // 根からの重みを累積和を取って求めている。
        return par[x] = root;
	}
    // 頂点xの重みを返す。
	ll weight(int x) {
		find(x);
		return diff_weight[x];
	}
    // xとyが同じ連結成分かの判定
	bool same(int x, int y) {
		return find(x) == find(y);
	}
    // xとyをマージする。すでに同じ連結成分ならfalseを返す。マージに成功すればtrueを返す。
    // 新しい代表元は、サイズの大きい連結成分の代表元となる。
	bool merge(int x, int y, ll w) {
		w += weight(x); w -= weight(y); // xとyの重みの差分wを、xの根とyの根の重みの差分に変換する。(mergeするのはこの2点だから)
		x = find(x); y = find(y);
		if (x == y) return false;
		if (sz[x] < sz[y]) swap(x, y), w = -w; 
		par[y] = x;
        sz[x] += sz[y];
		diff_weight[y] = w;
		return true;
	}
    // 頂点x, yの重みの差分を返す。
	ll diff(int x, int y) {
		return weight(y) - weight(x);
	}
    // xの属する連結成分のサイズを返す。
    int size(int x){
        return sz[find(x)];
    }
};
