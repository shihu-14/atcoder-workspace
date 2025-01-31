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
    vector<int> c(1010), a(n), b(n);
    rep(i, n) cin >> a[i];
    rep(i, n) cin >> b[i];
    rep(i, n){
        for (int j = a[i]; j <= b[i]; j++){
            c[j]++;
        }
    }
    int ans = 0;
    rep(i, 1010){
        if (c[i] == n) ans++;
    }
    cout << ans << endl;
    return 0;
}