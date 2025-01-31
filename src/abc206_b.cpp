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
    double n; cin >> n;
    int ans = ceil(sqrt(1.0/4.0+2*n)-0.5);
    cout << ans << endl;
    return 0;
}