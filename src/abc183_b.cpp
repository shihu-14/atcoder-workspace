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
    double sx, sy, gx, gy; cin >> sx >> sy >> gx >> gy;
    double ans = (sy*gx+sx*gy)/(sy+gy);
    printf("%.16f\n", ans);
    return 0;
}