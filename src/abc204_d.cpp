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
    int n; cin >> n;
    int sum = 0;
    vector<int> t(n);
    rep(i, n) cin >> t[i];
    rep(i, n) sum += t[i];
    vector<int> dp(100010, 0);
    dp[0] = 1;
    rep(i, n){
        vector<int> pre(100010); swap(dp, pre);
        rep(j, 100001){
            if (pre[j] == 0) continue;
            if (j+t[i] <= 100000) dp[j+t[i]] = 1;
            dp[j] = 1;
        }
    }
    int ans = 1e9;
    rep(i, 100001){
        if (dp[i] == 1) ans = min(ans, max(i, sum-i));
    }
    cout << ans << endl;
    return 0;
}