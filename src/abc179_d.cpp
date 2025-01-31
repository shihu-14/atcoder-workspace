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
    int n, k; cin >> n >> k;
    vector<P> s;
    rep(i, k){
        int l, r; cin >> l >> r;
        s.push_back({l, r});
    }
    vector<mint> dp(n+1);
    dp[1] = 1;
    for(int i = 2; i <= n; i++){
        rep(j, k){
            auto [l, r] = s[j];
            if (i-l < 1) continue;
            int t = max(i-r, 1);
            dp[i] += dp[i-l] - dp[t-1];
        }
        if (i < n) dp[i] += dp[i-1];
    }
    cout << dp[n].val() << endl;
    return 0;
}