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
    ll n, d; cin >> n >> d;
    int ans = 0;
    rep(i, n){
        ll x, y; cin >> x >> y;
        if (x*x+y*y <= d*d) ans++;
    }
    cout << ans << endl;
    return 0;
}