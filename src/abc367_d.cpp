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
    int n, m; cin >> n >> m;
    vector<ll> a(2*n+1);
    rep(i, n) cin >> a[i+1];
    rep(i, n) a[i+1+n] = a[i+1];
    rep(i, 2*n) a[i+1] += a[i];
    rep(i, 2*n+1) a[i] %= m;

    vector<vector<int>> c(m); 
    rep(i, 2*n+1){
        c[a[i]].push_back(i);
    }

    ll ans = 0;
    rep(i, n){
        int it1 = lower_bound(c[a[i]].begin(), c[a[i]].end(), i+1) - c[a[i]].begin();
        int it2 = lower_bound(c[a[i]].begin(), c[a[i]].end(), i+n) - c[a[i]].begin();
        ans += it2 - it1;
    }
    cout << ans << endl;
    return 0;
}