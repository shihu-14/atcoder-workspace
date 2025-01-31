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
    int a, b; cin >> a >> b;
    a += b;
    if (a >= 15 && b >= 8) cout << 1 << endl;
    else if (a >= 10 && b >= 3) cout << 2 << endl;
    else if (a >= 3) cout << 3 << endl;
    else cout << 4 << endl;
    return 0;
}