#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
#define rep(i, n) for (int i = 0; i < (n); ++i)
using ll = long long;
using P = pair<int, int>;
using T = tuple<int, int, int>;
ll dp[410][410][410];
ll inf = 1e18;
int main(){
    int n, m; cin >> n >> m;
    rep(i, n)rep(j, n){
        dp[i][j][0] = inf;
    }
    rep(i, n){
        dp[i][i][0] = 0;
    }
    rep(i, m){
        int a, b, c; cin >> a >> b >> c; a--, b--;
        dp[a][b][0] = c;
    }
    


    for (int k = 1; k <= n; k++){
        rep(i, n)rep(j, n){
            dp[i][j][k] = min(dp[i][k-1][k-1]+dp[k-1][j][k-1], dp[i][j][k-1]);
        }
    }

    ll ans = 0;    
    rep(i, n)rep(j, n)rep(k, n){
        if (dp[i][j][k+1] != inf) ans += dp[i][j][k+1];
    }

    cout << ans << endl;
    
    return 0;
}