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
    int h, w, a, b; cin >> h >> w >> a >> b;
    vector<vector<bool>> used(h, vector<bool>(w, false));
    int ans = 0;
    auto f = [&](auto f, int x, int y, int ra, int rb) -> void{
        if (x > h-1){
            if (ra == 0 && rb == 0) ans++;
            return;
        }
        if (used[x][y]){
            if (y < w-1) f(f, x, y+1, ra, rb);
            else f(f, x+1, 0, ra, rb);
            return;
        }
        if (ra > 0){
            if (y < w-1 && !used[x][y] && !used[x][y+1]){
                used[x][y] = true;
                used[x][y+1] = true;
                f(f, x, y+1, ra-1, rb);
                used[x][y] = false;
                used[x][y+1] = false;
            }
            if (x < h-1 && !used[x][y] && !used[x+1][y]){
                used[x][y] = true;
                used[x+1][y] = true;
                if (y < w-1) f(f, x, y+1, ra-1, rb);
                else f(f, x+1, 0, ra-1, rb);
                used[x][y] = false;
                used[x+1][y] = false;
            }
        }
        if (rb > 0){
            used[x][y] = true;
            if (y < w-1) f(f, x, y+1, ra, rb-1);
            else f(f, x+1, 0, ra, rb-1);
            used[x][y] = false;
        }
    };
    f(f, 0, 0, a, b);
    cout << ans << endl;
    return 0;
}