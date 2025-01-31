#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
#define rep(i, n) for (int i = 0; i < (n); ++i)
using ll = long long;
using P = pair<int, int>;
using T = tuple<int, int, int>;


// Union-find
struct UnionFind {
    vector<int> par, sz;
    vector<map<int, int>> mp;
    UnionFind(int n=0): par(n, -1), sz(n, 1), mp(n) {}
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
        for(auto [k, v]: mp[y]){
            mp[x][k] += v;
        }
        par[y] = x;
        sz[x] += sz[y]; 
        return true;
    }
    // xの属する連結成分のサイズを返す。
    int size(int x){
        return sz[find(x)];
    }
};

int main(){
    int n, q; cin >> n >> q;
    UnionFind uf(n);
    rep(i, n){
        int cv; cin >> cv;
        uf.mp[i][cv] += 1;
    }
    rep(i, q){
        int t, x, y; cin >> t >> x >> y;
        if (t == 1){
            x--, y--;
            if (uf.same(x, y)) continue;
            uf.merge(x, y);
        }
        else{
            x--;
            int a = uf.find(x);
            cout << uf.mp[a][y] << endl;
        }
    }
    return 0;
}