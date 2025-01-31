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
    vector<ll> a(n+1), sq(n+1);
    rep(i, n) cin >> a[i+1];
    rep(i, n) sq[i+1] = a[i+1]*a[i+1];
    rep(i, n) a[i+1] += a[i];
    rep(i, n) sq[i+1] += sq[i];
    ll ans = 0;
    for (int i = 2; i <= n; i++){
        ans += (sq[i]-sq[i-1])*(i-1);
        ans -= 2*(a[i]-a[i-1])*a[i-1];
        ans += sq[i-1];
    }
    cout << ans << endl;
    return 0;
}