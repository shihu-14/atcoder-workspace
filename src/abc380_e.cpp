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

// Union-find
struct UnionFind {
    vector<int> par, sz, min_e, max_e;
    UnionFind(int n=0): par(n, -1), sz(n, 1), min_e(n), max_e(n){
        rep(i, n) min_e[i] = i, max_e[i] = i;
    }
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
        chmin(min_e[x], min_e[y]);
        chmax(max_e[x], max_e[y]);
        sz[x] += sz[y]; 
        return true;
    }
    // xの属する連結成分のサイズを返す。
    int size(int x){
        return sz[find(x)];
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q; cin >> n >> q;
    vector<int> color(n), cnt(n, 1);
    rep(i, n) color[i] = i;
    UnionFind uf(n);
    rep(qi, q){
        int type; cin >> type;
        if (type == 1){
            int x, c; cin >> x >> c; x--, c--;
            int pre_size = uf.size(x);
            cnt[color[uf.find(x)]] -= pre_size;
            if (uf.min_e[uf.find(x)] > 0 && color[uf.min_e[uf.find(x)]-1] == c){
                uf.merge(uf.min_e[uf.find(x)]-1, x);
            }
            if (uf.max_e[uf.find(x)] < n-1 && color[uf.max_e[uf.find(x)]+1] == c){
                uf.merge(uf.max_e[uf.find(x)]+1, x);
            }
            color[uf.find(x)] = c;
            cnt[c] += pre_size;
        }
        else{
            int c; cin >> c; c--;
            cout << cnt[c] << endl;
        }
    }
    return 0;
}