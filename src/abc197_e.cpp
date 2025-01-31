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
void chmin(ll &a, ll b) {a = min(a, b);}
int main(){
    int n; cin >> n;
    vector<vector<int>> c(n+2);
    c[0].push_back(0);
    c[n+1].push_back(0);
    rep(i, n){
        int x, cv; cin >> x >> cv;
        c[cv].push_back(x);
    }
    vector<int> nc;
    rep(i, n+2){
        if (c[i].size() == 0) continue;
        nc.push_back(i);
        sort(c[i].begin(), c[i].end());
    }
    vector<ll> dp(2, inf); dp[0] = 0;
    for(int i = 1; i < nc.size(); i++){
        int j = nc[i-1], nj = nc[i]; // 今回収したボールの色jと次回収するボールの色nj
        vector<ll> pre(2, inf); swap(pre, dp);
        if (c[nj].size() == 1){
            int xl = c[j].front(), xr = c[j].back(), nx = c[nj][0];
            if (xl < nx) chmin(dp[0], pre[0]+(nx-xl));
            else chmin(dp[0], pre[0]+(xl-nx));
            
            if (xr < nx) chmin(dp[0], pre[1]+(nx-xr));
            else chmin(dp[0], pre[1]+(xr-nx));
        }
        else{
            int xl = c[j].front(), xr = c[j].back(), nxl = c[nj].front(), nxr = c[nj].back();
            if (xl < nxl) chmin(dp[1], pre[0]+(nxr-xl));
            else if (nxl < xl && xl < nxr){
                chmin(dp[0], pre[0]+(nxr-xl)+(nxr-nxl));
                chmin(dp[1], pre[0]+(xl-nxl)+(nxr-nxl));
            }
            else chmin(dp[0], pre[0]+(xl-nxl));

            if (xr < nxl) chmin(dp[1], pre[1]+(nxr-xr));
            else if (nxl < xr && xr < nxr){
                chmin(dp[0], pre[1]+(nxr-xr)+(nxr-nxl));
                chmin(dp[1], pre[1]+(xr-nxl)+(nxr-nxl));
            }
            else chmin(dp[0], pre[1]+(xr-nxl));
        }
    }
    cout << min(dp[0], dp[1]) << endl;
    return 0;
}