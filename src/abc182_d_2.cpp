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
    rep(i, n-1) a[i+1] += a[i];
    a.insert(a.begin(), 0);
    s.insert(s.begin(), 0);
    rep(i, n) a[i+1] = max(a[i], a[i+1]);
    a.push_back(0);
    ll ans = -inf;
    rep(i, n+1){
        ans = max(ans, s[i]+a[i+1]);
    }
    cout << ans << endl;
    return 0;
}