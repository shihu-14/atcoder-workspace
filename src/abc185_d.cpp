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
    vector<int> a(m+2);
    rep(i, m) cin >> a[i+1];
    a[m+1] = n+1;
    sort(a.begin(), a.end());
    int min_w = 1001001001;
    rep(i, m+1){
        int w = a[i+1]-a[i]-1;
        if (w == 0) continue;
        min_w = min(min_w, w);
    }
    ll ans = 0;
    rep(i, m+1){
        ans += (a[i+1]-a[i]-1+min_w-1)/min_w;
    }
    cout << ans << endl;
    return 0;
}