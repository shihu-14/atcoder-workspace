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
    int h, w; cin >> h >> w;
    vector<string> g(h);
    rep(i, h) cin >> g[i];

    vector dp(h, vector<mint>(w, 0)), dpx(h, vector<mint>(w, 0)), dpy(h, vector<mint>(w, 0)), dpxy(h, vector<mint>(w, 0));
    dp[0][0] = 1, dpx[0][0] = 0, dpy[0][0] = 0, dpxy[0][0] = 0;
    rep(i, h)rep(j, w){
        if (g[i][j] == '#') continue;
        if (i-1 >= 0 && g[i-1][j] != '#') dp[i][j] += dpx[i-1][j];
        if (j-1 >= 0 && g[i][j-1] != '#') dp[i][j] += dpy[i][j-1];
        if (i-1 >= 0 && j-1 >= 0 && g[i-1][j-1] != '#') dp[i][j] += dpxy[i-1][j-1];
        dpx[i][j] += dp[i][j];
        dpy[i][j] += dp[i][j];
        dpxy[i][j] += dp[i][j];
        if (i-1 >= 0 && g[i-1][j] != '#') dpx[i][j] += dpx[i-1][j];
        if (j-1 >= 0 && g[i][j-1] != '#') dpy[i][j] += dpy[i][j-1];
        if (i-1 >= 0 && j-1 >= 0 && g[i-1][j-1] != '#') dpxy[i][j] += dpxy[i-1][j-1];
    }
    cout << dp[h-1][w-1].val() << endl;
    return 0;
}