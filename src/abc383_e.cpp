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

struct Edge{
    int u, v, w;
    Edge(int u, int v, int w) : u(u), v(v), w(w) {}
};
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, k; cin >> n >> m >> k;
    dsu uf(n);
    vector<Edge> e;
    rep(i, m){
        int u, v, w; cin >> u >> v >> w; u--, v--;
        e.emplace_back(u, v, w);
    } 
    vector<int> a(n), b(n);
    rep(i, k){
        int x; cin >> x; x--; a[x]++;
    }
    rep(i, k){
        int x; cin >> x; x--; b[x]++;
    }
    sort(rng(e), [](Edge a, Edge b){
        return a.w < b.w;
    });
    ll ans = 0;
    rep(i, m){
        auto [u, v, w] = e[i];
        if (uf.same(u, v)) continue;
        u = uf.leader(u);
        v = uf.leader(v);
        uf.merge(u, v);
        if (b[u]) swap(u, v);
        int c = min(a[u], b[v]);
        ans += (ll)w*c;
        a[u] -= c;
        b[v] -= c;
        int nu = uf.leader(u);
        a[nu] = a[u]+a[v]; 
        b[nu] = b[u]+b[v];
    }
    cout << ans << endl;
    return 0;
}