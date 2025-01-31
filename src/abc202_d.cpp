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
    int a, b; ll k; cin >> a >> b >> k;
    vector dp(60, vector<ll>(60, 0));
    rep(i, 60) dp[i][0] = 1;
    rep(i, 60) dp[i][i] = 1;
    for (int i = 1; i < 60; i++){
        for (int j = 1; j < i; j++){
            dp[i][j] = dp[i-1][j-1] + dp[i-1][j];
        }
    }
    string ans;
    int ra = a, rb = b;
    rep(i, a+b){
        if (ra > 0){
            if (dp[ra-1+rb][rb] < k){
                k -= dp[ra-1+rb][rb];
                ans += 'b';
                rb--;
                continue;
            }
            else{
                ans += 'a';
                ra--;
                continue;
            }
        }
        else{
            k--;
            ans += 'b';
            rb--;
            continue;
        }
    }
    cout << ans << endl;
    return 0;
}