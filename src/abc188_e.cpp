#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
#define rep(i, n) for (int i = 0; i < (n); ++i)
using ll = long long;
using P = pair<int, int>;
using T = tuple<int, int, int>;
const ll inf = 1e18;
int main(){
    int n, m; cin >> n >> m;
    vector<ll> a(n);
    rep(i, n) cin >> a[i];
    vector<vector<int>> g(n);
    rep(i, m){
        int a, b; cin >> a >> b;
        a--; b--;
        g[a].push_back(b);
    }
    ll ans = -inf;
    vector<ll> dp(n);
    rep(i, n) dp[i] = a[i];
    rep(i, n){
        for(int j: g[i]){
            ans = max(ans, a[j]-dp[i]);
            dp[j] = min(dp[j], dp[i]);
        }
    }

    cout << ans << endl;
    return 0;
}