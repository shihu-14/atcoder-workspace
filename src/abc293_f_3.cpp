#include <bits/stdc++.h>
using namespace std;
#define rep(i,n) for (int i = 0; i < (n); ++i)
using ll = long long;

void solve() {
  const int m = 6;
  ll n;
  cin >> n;
  int ans = 0;
  for (ll b = 2; b <= 1000; b++) {
    ll x = n;
    bool ok = true;
    int cnt = 0;
    while (x) {
      if (x%b > 1) ok = false;
      cnt++;
      x /= b;
    }
    if (ok && cnt > m){
        ans++;
    }
  }

  rep(s,1<<m) {
    if (s == 0) continue;
    auto f = [&](ll b) {
      ll res = 0, x = 1;
      rep(i,m) {
        if (s>>i&1) res += x;
        if (x <= n/b) x *= b; else x = ll(1e18)+1;
      }
      return res;
    };
    ll ac = 2, wa = n+1;
    while (ac+1 < wa) {
      ll wj = (ac+wa)/2;
      if (f(wj) <= n) ac = wj; else wa = wj;
    }
    if (f(ac) == n) ans++;
  }

  cout << ans << endl;
}

int main() {
  int t;
  cin >> t;
  rep(ti,t) solve();
  return 0;
}