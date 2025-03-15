// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
// #include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
// using mint = modint1000000007;
// using namespace boost::multiprecision;
using uint = unsigned int;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T3 = tuple<int, int, int>;
using G = vector<vector<int>>;
#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define rep2(i, a, b) for (ll i = a; i < (b); ++i)
#define rrep2(i, a, b) for (ll i = a-1; i >= (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcount_ll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(rng(v))
#define MAX(v) *max_element(rng(v))
#define SUM(v) accumulate(rng(v),0LL)
#define IN(v, x) (find(rng(v),x) != v.end())
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 3001002003004005006ll;
const int INF = 1001001001;
int rand(){static random_device rd; static mt19937 mt(rd()); static uniform_int_distribution<int> dist(0, INF); return dist(mt);}

struct UnionFind {
    int ans = 0;
    vector<int> d, cnt;
    vector<bool> isE;
    UnionFind(int n=0, int m=0): d(n+m,-1), cnt(n+m, 0), isE(n+m, false){
        rep(i, n) cnt[i] = 1;
        rep2(i, n, n+m) isE[i] = true;
    }
    int find(int x) {
        if(d[x] < 0) return x;
        return d[x] = find(d[x]);
    }
    bool same(int x, int y) {
        return find(x) == find(y);
    }
    bool merge(int x, int y) { // if merge successed, return true
        x = find(x);
        y = find(y);
        if (x == y) return false;
        if (d[x] > d[y]) swap(x, y);
        if (isE[x]) ans -= cnt[x];
        if (isE[y]) ans -= cnt[y];
        if (isE[y]) isE[x] = true;
        cnt[x] += cnt[y];
        if (isE[x]) ans += cnt[x];
        d[x] += d[y];
        d[y] = x;
        return true;
    }
    int size(int x){
        return -d[find(x)];
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, E; cin >> n >> m >> E;
    vector<T3> edges;
    rep(i, E){
        int u, v; cin >> u >> v; u--, v--;
        edges.emplace_back(u, v, i);
    }
    int q; cin >> q;
    vector<int> query(q);
    vector<bool> used(E, false);
    rep(qi, q) cin >> query[qi], query[qi]--, used[query[qi]] = true;
    reverse(rng(query));
    UnionFind uf(n, m);
    for (auto [u, v, id]: edges){
        if (used[id]) continue;
        uf.merge(u, v);
    }
    vector<int> ans;
    rep(i, q){
        auto [u, v, _] = edges[query[i]];
        ans.emplace_back(uf.ans);
        uf.merge(u, v);
    }
    reverse(rng(ans));
    rep(i, q) cout << ans[i] << endl; 
    return 0;
}