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
    vector<int> a(n);
    rep(i, n) cin >> a[i];
    ll ans = 0;
    rep(i, n-1){
        if (a[i+1]-a[i] >= 0){
            a[i+1] = max(a[i+1], a[i]);
            continue;
        }
        ans += a[i]-a[i+1];
        a[i+1] = max(a[i+1], a[i]);
    }
    cout << ans << endl;
    return 0;
}