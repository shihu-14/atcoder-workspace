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
    vector<int> a(n), b(n);
    rep(i, n){
        cin >> a[i] >> b[i];
    }
    int ans = 1e9;
    rep(i, n){
        rep(j, n){
            if (i == j) continue;
            ans = min(ans, max(a[i],b[j]));
        }
    }
    rep(i, n){
        ans = min(ans, a[i]+b[i]);
    }
    cout << ans << endl;
    return 0;
}