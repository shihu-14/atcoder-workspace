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
    int r; double x, y; cin >> r >> x >> y;
    double d = sqrt(x*x + y*y);
    if (x == 0 && y == 0){
        cout << 0 << endl;
        return 0;
    }
    if (d < r){
        cout << 2 << endl;
        return 0;
    }
    cout << (int)ceil(d/r) << endl;
    return 0;
}