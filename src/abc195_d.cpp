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
    int n, m, q; cin >> n >> m >> q;
    vector<P> a;
    rep(i, n){
        int w, v; cin >> w >> v;
        a.push_back({w, v});
    }
    sort(a.begin(), a.end());
    vector<P> b;
    rep(i, m){
        int x; cin >> x;
        b.push_back({x, i});
    }
    sort(b.begin(), b.end());

    rep(t, q){
        int l, r; cin >> l >> r;
        l--; r--;
        vector<int> nb;
        rep(i, m){
            if (b[i].second >= l && b[i].second <= r) continue;
            nb.push_back(b[i].first);
        }
        int k = nb.size();
        vector<vector<int>> dp(n+1, vector<int>(k+1, -inf));
        rep(i, n+1) dp[i][0] = 0;
        rep(i, k+1) dp[0][i] = 0;
        rep(i, n){
            rep(j, k){
                auto [w, v] = a[i];
                auto x = nb[j];
                if (w <= x){
                    dp[i+1][j+1] = max(dp[i][j+1], dp[i][j]+v);
                }
                else{
                    dp[i+1][j+1] = dp[i][j+1];
                }
            }
        }
        cout << (dp[n][k] == -inf ? 0 : dp[n][k]) << endl;
    }
    return 0;
}