#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
#define rep(i, n) for (int i = 0; i < (n); ++i)
using ll = long long;
using P = pair<int, int>;
using T = tuple<int, int, int>;
const int inf = 1001001001;
int main(){
    int n, m; cin >> n >> m;
    vector<int> a(n), b(m);
    rep(i, n) cin >> a[i];
    rep(i, m) cin >> b[i];
    vector<vector<int>> dp(n+1, vector<int>(m+1, inf));
    rep(i, n) dp[i][0] = i;
    rep(i, m) dp[0][i] = i;
    rep(i, n){
        rep(j, m){
            dp[i+1][j+1] = min(dp[i+1][j+1], dp[i][j]+(a[i] != b[j]));
            dp[i+1][j+1] = min(dp[i+1][j+1], dp[i+1][j]+1);
            dp[i+1][j+1] = min(dp[i+1][j+1], dp[i][j+1]+1);
        }
    }
    cout << dp[n][m] << endl;
    return 0;
}