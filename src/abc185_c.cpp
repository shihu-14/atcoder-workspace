#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
#define rep(i, n) for (int i = 0; i < (n); ++i)
using ll = long long;
using P = pair<int, int>;
using T = tuple<int, int, int>;

int main(){
    ll l; cin >> l;
    ll n = 11;
    vector<vector<ll>> dp(l+n+1, vector<ll>(l+n+1, 0));
    rep(i, n+l+1) dp[i][0] = 1;
    rep(i, n+l+1) dp[i][i] = 1;
    for(int i = 2; i < l+n+1; i++){
        for(int j = 1; j < i; j++){
            dp[i][j] = dp[i-1][j-1] + dp[i-1][j];
        }
    }
    cout << dp[l+n-12][n] << endl;
    return 0;
}