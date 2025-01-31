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
    vector<ll> a(n), c(n);
    rep(i, n) cin >> a[i];
    sort(a.begin(), a.end());
    c[0] = a[0];
    rep(i, n-1) c[i+1] = c[i]+a[i+1];
    ll ans = 0;
    rep(i, n-1){
        ans += (c[n-1] - c[i])-a[i]*(n-i-1);
    }
    cout << ans << endl;
    return 0;
}