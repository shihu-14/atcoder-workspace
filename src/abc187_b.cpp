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
    int n; cin >> n;
    vector<int> x(n), y(n);
    rep(i, n) cin >> x[i] >> y[i];
    int ans = 0;
    rep(i, n)rep(j, n){
        if (i >= j) continue;
        int d1 = x[i] - x[j];
        int d2 = y[i] - y[j];
        if (d1 < 0){
            d1 = -d1;
            d2 = -d2;
        }
        if (-d1 <= d2 && d2 <= d1) ans++;
    }
    cout << ans << endl;
    return 0;
}