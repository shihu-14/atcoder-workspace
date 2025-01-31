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
    int n; ll k; cin >> n >> k;
    vector<int> x(n), a(n);
    rep(i, n) cin >> x[i];
    rep(i, n) x[i]--;
    rep(i, n) cin >> a[i];

    vector dp(64, vector<int>(n, 0));
    dp[0] = x;
    rep(i, 63){
        rep(j, n) dp[i+1][j] = dp[i][dp[i][j]];
    }

    int j = 0;
    while(k > 0){
        vector<int> pre = a;
        if (k&1){
            rep(i, n) a[i] = pre[dp[j][i]];
        } 
        k >>= 1;
        j++;
    }
    rep(i, n) cout << a[i] << " ";
    cout << endl;
    return 0;
}