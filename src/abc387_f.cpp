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

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);   
    int n, m; cin >> n >> m;
    vector<int> a(n);
    scc_graph g(n);
    vector<vector<int>> g2(n);
    rep(i, n){
        cin >> a[i]; a[i]--;
        g.add_edge(i, a[i]);
    }
    auto scc = g.scc();
    dsu uf(n);
    for (auto vec: scc){
        if (vec.size() == 1) continue; 
        rep(i, vec.size()-1){
            uf.merge(vec[i], vec[i+1]);
        }
    }
    rep(i, n){
        g2[uf.leader(a[i])].emplace_back(uf.leader(i));
    }
    vector dp(n, vector<mint>(m+1, 1));
    rep(i, n) dp[i][0] = 0;
    auto f = [&](auto f, int u, int x) -> void{
        for (auto v: g2[u]){
            v = uf.leader(v);
            if (v == u) continue;
            // cout << u << " " << v << " " << x << endl;
            f(f, v, x);
            dp[u][x] *= dp[v][x];
        }
        dp[u][x] += dp[u][x-1];
        // cout << u << "-> " << dp[u][x].val() << endl;
    };

    mint ans = 1;
    for (auto vec: uf.groups()){
        if (vec.size() == 1 && a[vec[0]] != vec[0]) continue;
        int v = uf.leader(vec[0]);
        rep2(j, 1, m+1){
            f(f, v, j);
        }
        ans *= dp[v][m];
    }

    cout << ans.val() << endl;
    return 0;
}