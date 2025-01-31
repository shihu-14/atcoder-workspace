#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
#define rep(i, n) for (int i = 0; i < (n); ++i)
using ll = long long;
using P = pair<int, int>;
using T = tuple<int, int, int>;
const ll inf = 1e18;
ll dp[3010][3010][4];
void chmax(ll &a, ll b){a = max(a, b);}
int main(){
    int r, c, k; cin >> r >> c >> k;
    vector<vector<int>> g(r, vector<int>(c));
    rep(i, k){
        int rv, cv, vv; cin >> rv >> cv >> vv; rv--, cv--;
        g[rv][cv] = vv;
    }

    rep(i, 3010)rep(j, 3010)rep(k, 4) dp[i][j][k] = -inf;
    dp[0][0][0] = 0;
    if (g[0][0]) dp[0][0][1] = g[0][0];

    rep(i, r)rep(j, c){
        rep(k, 4){
            if (dp[i][j][k] == -inf) continue;
            if (i+1 < r){
                rep(l, 4){
                    chmax(dp[i+1][j][l], dp[i][j][k]);
                    if (l > 0 && g[i+1][j]) chmax(dp[i+1][j][l], dp[i][j][k] + g[i+1][j]);
                }
            }
            if (j+1 < c){
                chmax(dp[i][j+1][k], dp[i][j][k]);
                if (k < 3 && g[i][j+1]) {
                    chmax(dp[i][j+1][k+1], dp[i][j][k] + g[i][j+1]);
                }
            }
        }
    }

    ll ans = -inf;
    rep(i, 4) ans = max(ans, dp[r-1][c-1][i]);
    cout << ans << endl;
    return 0;
}