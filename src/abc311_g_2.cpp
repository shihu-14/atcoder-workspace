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

// https://nyaannyaan.github.io/library/tree/cartesian-tree.hpp.html
template <typename T> // (graph, root)
pair<vector<vector<int>>, int> CartesianTree(vector<T> &a){
    int n = a.size();
    vector<vector<int>> g(n);
    vector<int> stk, par(n, -1);
    stk.reserve(n);
    rep(i, n){
        int last = -1;
        while (!stk.empty() && a[i] < a[stk.back()]) {
            last = stk.back();
            stk.pop_back();
        }
        if (last != -1) par[last] = i;
        if (!stk.empty()) par[i] = stk.back();
        stk.emplace_back(i);
    }
    int root = -1;
    rep(i, n){
        if (par[i] != -1) g[par[i]].emplace_back(i);
        else root = i;
    }
    return make_pair(g, root);
}


/* testcase

* abc311_g

*/

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector a(n, vector<int>(m));
    rep(i, n)rep(j, m) cin >> a[i][j];
    ll ans = 0;
    rep(i, n){
        vector<pii> query(m, {INF, 0});
        rep2(j, i, n){
            rep(k, m){
                auto [mn, sum] = query[k];
                chmin(mn, a[j][k]);
                sum += a[j][k];
                query[k] = {mn, sum};
            }
            vector<int> b(m);
            rep(k, m) b[k] = query[k].fi;
            auto [g, root] = CartesianTree(b);
            ll res = 0;
            auto dfs = [&](auto f, int v) -> ll{
                ll dp = query[v].se;
                for (int u: g[v]){
                    dp += f(f, u);
                }
                chmax(res, dp*query[v].fi);
                return dp;
            };
            dfs(dfs, root);
            chmax(ans, res);
        }
    }
    cout << ans << endl;
    return 0;
}