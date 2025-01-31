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
#define rrep2(i, a, b) for (ll i = a; i > (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcountll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(all(v))
#define MAX(v) *max_element(all(v))
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
    vector<int> par, sz, ans;
    UnionFind(int n=0): par(n, -1), sz(n, 1), ans(n){
        rep(i, n) ans[i] = i;
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
    // 新しい代表元は、マージ前のサイズの大きい連結成分の代表元。
    bool merge(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) return false;
        if (sz[x] < sz[y]) swap(x, y);
        ans[x] = min(ans[x], ans[y]);
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
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    vector<int> rev(n);
    UnionFind uf(n);
    rep(i, n-1){
        cin >> rev[i+1];
        rev[i+1]--;
    }
    int q; cin >> q;
    rep(i, q){
        int type; cin >> type;
        if (type == 1){
            int u, v; cin >> u >> v; u--; v--;
            while (!uf.same(u,v)) {
                u = uf.ans[uf.find(u)];
                uf.merge(u, rev[u]);
            }
        }
        else{
            int x; cin >> x; x--;
            cout << uf.ans[uf.find(x)]+1 << endl;
        }
    }
    return 0;
}