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
    fenwick_tree<int> BIT(n);
    ll ans = 0;
    rep(i, n){
        ans += BIT.sum(a[i]+1, n);
        BIT.add(a[i], 1);
    }
    cout << ans << endl;
    rep(i, n-1){
        ans -= a[i];
        ans += n-1-a[i];
        cout << ans << endl;
    }
    return 0;
}