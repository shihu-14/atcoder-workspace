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
    int x, y; cin >> x >> y;
    if (x == y) cout << x << endl;
    else{
        cout << 3 - x - y << endl;
    }
    return 0;
}