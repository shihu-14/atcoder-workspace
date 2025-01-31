#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
#define rep(i, n) for (int i = 0; i < (n); ++i)
using ll = long long;
using P = pair<int, int>;
using T = tuple<int, int, int>;
int inf = 1e9;
int memo[2000][2000];
int main(){
    int h, w; cin >> h >> w;
    vector<string> g(h);
    rep(i, h) cin >> g[i];
    rep(i, 2000)rep(j, 2000) memo[i][j] = -inf;
    
    auto f = [&](auto f, int x, int y) -> int {
        if (x == h-1 && y == w-1){
            return 0;
        }
        if (memo[x][y] != -inf) return memo[x][y];
        int res = -inf;
        if (x < h-1){
            if (g[x+1][y] == '+'){
                res = max(res, -f(f, x+1, y)+1);
            }
            else{
                res = max(res, -f(f, x+1, y)-1);
            }
        }
        if (y < w-1){
            if (g[x][y+1] == '+'){
                res = max(res, -f(f, x, y+1)+1);
            }
            else{
                res = max(res, -f(f, x, y+1)-1);
            }
        }
        return memo[x][y] = res;
    };

    int ans = f(f, 0, 0);
    if (ans > 0) cout << "Takahashi" << endl;
    else if (ans < 0) cout << "Aoki" << endl;
    else cout << "Draw" << endl;
    return 0;
}