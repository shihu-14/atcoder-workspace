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
    ll n, x, m; cin >> n >> x >> m;
    vector<ll> a = {x};
    vector<bool> used(100010); used[x] = true;
    ll now = x, rd = 0, ri = 0, rc = 0;
    while(1){
        ll to = (now*now)%m;
        if (used[to]){
            rd = to;
            rep(i, a.size()) if (a[i] == to) ri = i;
            rc = a.size()-ri;
            break;
        }
        used[to] = true;
        a.push_back(to);
        now = to;
    }
    ll ans = 0;
    if (n < ri){
        rep(i, n) ans += a[i];
        cout << ans << endl;
        return 0;
    }

    ll ra = 0;
    for(int i = ri; i < a.size(); i++){
        ra += a[i];
    }
    rep(i, ri) ans += a[i];
    ans += ((n-ri)/rc)*ra;
    rep(i, (n-ri)%rc) ans += a[ri+i];
    cout << ans << endl;
    return 0;
}