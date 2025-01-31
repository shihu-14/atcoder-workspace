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
#define rep(i, n) for (ll i = 0; i < n; ++i)
#define rep2(i, a, b) for (ll i = a; i < b; ++i)
#define rrep2(i, a, b) for (ll i = a; i > b; --i)
#define rep3(i, a, b, c) for (ll i = a; i < b; i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcountll __builtin_popcountll
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
    int n; cin >> n;
    vector<vector<int>> a(n, vector<int>(n-1));
    rep(i, n)rep(j, n-1) cin >> a[i][j];
    rep(i, n)rep(j, n-1) a[i][j]--;
    map<pii, int> mp;
    int c = 0;
    rep(i, n)rep2(j, i+1, n){
        mp[{i, j}] = mp[{j, i}] = c++;
    }
    scc_graph g(n*(n-1)/2);
    G to(n*(n-1)/2), rev(n*(n-1)/2);
    rep(i, n)rep(j, n-2){
        int u = mp[{i, a[i][j]}];
        int v = mp[{i, a[i][j+1]}];
        g.add_edge(u, v);
        to[u].push_back(v);
        rev[v].push_back(u);
    }

    auto scc = g.scc();
    vector<int> dp(n*(n-1)/2, -INF);
    rep(i, n*(n-1)/2) if (rev[i].size() == 0) dp[i] = 1;
    for(auto vec: scc){
        if (vec.size() >= 2){
            cout << -1 << endl;
            return 0;
        }
        int u = vec[0];
        for(auto v: to[u]){
            chmax(dp[v], dp[u]+1);
        }
    }

    int ans = 0;
    rep(i, n*(n-1)/2){
        chmax(ans, dp[i]);
    }
    cout << ans << endl;
    return 0;
}