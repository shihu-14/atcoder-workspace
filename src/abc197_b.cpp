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
    int h, w, x, y; cin >> h >> w >> x >> y; x--; y--;
    vector<string> g(h);
    rep(i, h) cin >> g[i];
    int ans = 1;
    for (int i = y+1; i < w; i++){
        if (g[x][i] == '#') break;
        ans++;
    }
    for (int i = y-1; i >= 0; i--){
        if (g[x][i] == '#') break;
        ans++;
    }
    for (int i = x+1; i < h; i++){
        if (g[i][y] == '#') break;
        ans++;
    }
    for (int i = x-1; i >= 0; i--){
        if (g[i][y] == '#') break;
        ans++;
    }
    cout << ans << endl;
    
    return 0;
}