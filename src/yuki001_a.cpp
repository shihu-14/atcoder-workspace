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

struct Edge{
    int to, y, t; 
    Edge(){}
    Edge(int to, int y, int t) : to(to), y(y), t(t) {}
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, c, v; cin >> n >> c >> v;
    vector<int> s(v), t(v), y(v), m(v);
    scc_graph g1(n);
    vector<vector<Edge>> g2(n);
    rep(i, v) cin >> s[i], s[i]--;
    rep(i, v) cin >> t[i], t[i]--;
    rep(i, v) cin >> y[i];
    rep(i, v) cin >> m[i];
    rep(i, v){
        g1.add_edge(s[i], t[i]);
        g2[s[i]].emplace_back(t[i], y[i], m[i]);
    }
    auto scc = g1.scc();
    vector<vector<int>> dp(n, vector<int>(c+1, INF)); dp[0][c] = 0;
    for(auto vec: scc){
        int u = vec[0];
        rep(i, c+1){
            for (auto [to, yen, time] : g2[u]){
                if (i-yen < 0) continue;
                chmin(dp[to][i-yen], dp[u][i]+time);
            }
        }
    }
    int ans = INF;
    rep(i, c+1) chmin(ans, dp[n-1][i]);
    cout << (ans == INF ? -1 : ans) << endl;
    return 0;
}