#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
#define rep(i, n) for (int i = 0; i < (n); ++i)
using ll = long long;
using P = pair<int, int>;
using T = tuple<int, int, int>;

int dx[4] = {-1, -1, 1, 1};
int dy[4] = {-1, 1, 1, -1};

int main(){
    int h, w; cin >> h >> w;
    vector<string> g(h);
    rep(i, h) cin >> g[i];
    int ans = 0;
    rep(i, h)rep(j, w){
        if (g[i][j] == '.') continue;
        rep(t, 4){
            int ni = i+dx[t], nj = j+dy[t];
            if (g[i][nj] == '.' && g[ni][j] == '.' && g[ni][nj] == '.') ans++;
            if (g[i][nj] == '#' && g[ni][j] == '#' && g[ni][nj] == '.') ans++;
        }
    }
    cout << ans << endl;
    
    return 0;
}