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
    int h, w; cin >> h >> w;
    vector<vector<int>> g(h, vector<int>(w));
    rep(i, h)rep(j, w) cin >> g[i][j];
    int min_c = 1001001001;
    int ans = 0;
    rep(i, h)rep(j, w){
        ans += g[i][j];
        min_c = min(min_c, g[i][j]);
    }

    cout << ans-min_c*h*w << endl;

    return 0;
}
