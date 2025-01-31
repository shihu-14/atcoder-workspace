#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
#define rep(i, n) for (int i = 0; i < (n); ++i)
using ll = long long;
using P = pair<int, int>;
using T = tuple<int, int, int>;
const ll inf = 1e18;
int main(){
    int n; cin >> n;
    vector<ll> a(n), s(n);
    rep(i, n) cin >> a[i];
    s[0] = a[0];
    rep(i, n-1) s[i+1] = s[i] + a[i+1];
    rep(i, n-1) s[i+1] += s[i];
    ll max_s = 0, id = n;
    rep(i, n){
        if (max_s <= s[i]){
            max_s = s[i];
            id = i;
        }
    }
    if (id == n){
        cout << 0 << endl;
        return 0;
    }

    ll ans = (id == 0 ? 0 : s[id-1]), res = ans;
    rep(i, id+1){
        res += a[i];
        ans = max(ans, res);
    }
    if (id < n-1){
        rep(i, id+2){
            res += a[i];
            ans = max(ans, res);
        }   
    }
    cout << ans << endl;
    return 0;
}