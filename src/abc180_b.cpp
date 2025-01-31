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
    vector<ll> x(n);
    rep(i, n) cin >> x[i];
    ll ans1 = 0, ans3 = 0; double ans2 = 0;
    rep(i, n){
        ans1 += abs(x[i]);
        ans2 += x[i]*x[i];
        ans3 = max(ans3, abs(x[i]));
    }
    ans2 = sqrt(ans2);
    cout << ans1 << endl;
    printf("%.16f\n", ans2);
    cout << ans3 << endl;
    return 0;
}