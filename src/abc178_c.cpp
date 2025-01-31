#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
// using mint = modint998244353;
using mint = modint1000000007;
#define rep(i, n) for (int i = 0; i < (n); ++i)
using ll = long long;
using P = pair<int, int>;
using T = tuple<int, int, int>;

int main(){
    ll n; cin >> n;
    vector<vector<mint>> dp(2, vector<mint>(2));
    dp[0][0] = 1;
    rep(i, n){
        vector<vector<mint>> pre(2, vector<mint>(2)); swap(dp, pre);
        dp[0][0] += pre[0][0]*8;
        dp[1][0] += pre[1][0]*9+pre[0][0];
        dp[0][1] += pre[0][1]*9+pre[0][0];
        dp[1][1] += pre[1][1]*10+pre[1][0]+pre[0][1];
    } 
    cout << dp[1][1].val() << endl;
    return 0;
}