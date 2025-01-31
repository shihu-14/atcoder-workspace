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
    int v, t ,s, d; cin >> v >> t >> s >> d;
    if (v*t <= d && d <= v*s) cout << "No" << endl;
    else cout << "Yes" << endl;
    return 0;
}