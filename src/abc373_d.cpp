// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
// using mint = modint1000000007;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T = tuple<int, int, int>;
using G = vector<vector<int>>;
#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define rep2(i, a, b) for (ll i = a; i < (b); ++i)
#define rrep2(i, a, b) for (ll i = a-1; i >= (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcountll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(rng(v))
#define MAX(v) *max_element(rng(v))
#define SUM(v) accumulate(rng(v),0)
#define IN(v, x) (find(rng(v),x) != v.end())
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {0, 1, 0, -1};
const ll dy[] = {1, 0, -1, 0};
const ll dxx[] = {0, 1, 0, -1, 1, -1, 1, -1};
const ll dyy[] = {1, 0, -1, 0, 1, 1, -1, -1};
const ll LINF = 1001002003004005006ll;
const int INF = 1001001001;

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


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m; cin >> n >> m;
    WightedUnionFind uf(n);
    rep(i, m){
        int u, v, w; cin >> u >> v >> w;
        u--; v--;
        uf.merge(u, v, w);
    }
    rep(i, n){
        cout << uf.weight(i) << " ";
    }
    cout << endl;
    return 0;
}