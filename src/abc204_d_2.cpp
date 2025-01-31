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
    vector<int> t(n), s(n+1);
    rep(i, n) cin >> t[i];
    rep(i, n) s[i+1] += s[i]+t[i];
    map<int, int> dp;
    dp[0] = 1;
    rep(i, n){
        map<int, int> pre; swap(dp, pre);
        for (auto[a, v] : pre){
            int b = s[i] - a;
            if (a-(b+t[i]) >= -1000 && a-(b+t[i]) <= 1000) dp[a] = 1;
            if (a+t[i]-b >= -1000 && a+t[i]-b <= 1000) dp[a+t[i]] = 1;
        }
    }
    int ans = 1e9;
    for (auto [k, v]: dp){
        ans = min(ans, max(k, s[n]-k));
    }
    cout << ans << endl;
    return 0;
}