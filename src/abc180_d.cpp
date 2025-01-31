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
    ll x, y, a, b; cin >> x >> y >> a >> b;
    ll ans = 0;
    ll m = log(y/x)/log(a);
    if (y%x == 0 && (y/x)%a == 0) m--;
    if (m > 0){
        if (b/x/(a-1) > 0){
            ll n = log(b/x/(a-1))/log(a) + 1;
            ans += n;
            x *= pow(a, n);
        }
    }
    ll m2 = (y-x)/b;
    if ((y-x)%b == 0) m2--;
    if (m2 > 0){
        ans += m2;
    }
    cout << ans << endl;
    return 0;
}