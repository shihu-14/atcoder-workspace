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
    int S; cin >> S;
    vector<mint> dp(S+1);
    dp[0] = 1;
    for(int i = 3; i <= S; i++){
        dp[i] += dp[0];
        for(int j = 3; j <= i-3; j++){
            dp[i] += dp[j];
        }
    }
    cout << dp[S].val() << endl;
    return 0;
}