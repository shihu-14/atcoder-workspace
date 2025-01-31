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
    vector<int> par, sz;
    vector<set<int>> ans;
    UnionFind(int n=0): par(n, -1), sz(n, 1), ans(n){
        rep(i, n) ans[i].insert(i);
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
        ans[x].insert(rng(ans[y]));
        ans[y].clear();
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
    int n, q; cin >> n >> q;
    UnionFind uf(n);
    rep(i, q){
        int type; cin >> type;
        if (type == 1){
            int u, v; cin >> u >> v; u--; v--;
            uf.merge(u, v);
        }
        else{
            int v, k; cin >> v >> k; v--;
            if (uf.size(v) < k){
                cout << -1 << '\n';
                continue;
            }
            v = uf.find(v);
            vector<int> tmp;
            rep(j, k){
                int t = *uf.ans[v].rbegin();
                tmp.push_back(t);
                uf.ans[v].erase(t);
            }
            cout << tmp.back()+1 << '\n';
            rep(j, k){
                uf.ans[v].insert(tmp.back());
                tmp.pop_back();
            }
        }
    }
    cout.flush();
    return 0;
}