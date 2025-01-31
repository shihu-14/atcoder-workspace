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
    int n, x, r; cin >> n >> x >> r;
    vector<vector<pii>> g(n);
    rep(i, n){
        int p, u, c;  cin >> p >> u >> c; c--;
        g[c].emplace_back(p, u);
    }

    vector dp(x+1, vector<ll>(2, -LINF)); dp[0][0] = 0;
    rep(i1, n){
        int m = g[i1].size();
        if (m == 0) continue;
        rep(i2, m){
            vector pre(x+1, vector<ll>(2, -LINF)); swap(dp, pre); 
            rep(j, x+1)rep(k, 2){
                auto [p, u] = g[i1][i2];
                int nj = j+p, nk = k;
                if (i2 == 0) nk = 0;
                chmax(dp[j][nk], pre[j][k]);
                if (nj > x) continue;
                chmax(dp[nj][1], pre[j][k]+u+(nk == 0 ? r : 0));
            }
        }
    }
    ll ans = -LINF;
    rep(i, x+1)rep(j, 2)chmax(ans, dp[i][j]);
    cout << ans << endl;
    return 0;
}