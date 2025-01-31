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
    vector<string> s(n);
    rep(i, n) cin >> s[i];
    vector<ll> dp(2, 1); 
    rep(i, n){
        vector<ll> pre(2); swap(pre, dp);
        rep(j, 2){
            if (s[i] == "AND"){
                rep(k, 2){
                    int nj = j&k;
                    dp[nj] += pre[j];
                }
            }else{
                rep(k, 2){
                    int nj = j|k;
                    dp[nj] += pre[j];
                }
            }
        }
    }
    cout << dp[1] << endl;
    return 0;
}