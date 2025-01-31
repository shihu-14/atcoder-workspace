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
    vector<int> h(n), w(m);
    rep(i, n) cin >> h[i];
    rep(i, m) cin >> w[i];
    sort(h.begin(), h.end());
    sort(w.begin(), w.end());
    ll l = 0, r = 0;
    ll ans = 0;
    int it = lower_bound(h.begin(), h.end(), w[0]) - h.begin();
    if (it&1) it--;
    for(int i = 0; i < it; i+=2){
        l += h[i+1]-h[i];
    }
    for(int i = it+1; i < n; i+=2){
        r += h[i+1]-h[i];
    }
    ans += l+r+abs(h[it]-w[0]);
    int pre = it;
    rep(i, m-1){
        it = lower_bound(h.begin(), h.end(), w[i+1]) - h.begin();
        if (it&1) it--;
        ll tmp = 0;
        for(int j = pre; j < it; j+=2){
            tmp += h[j+1]-h[j];
            r -= h[j+2]-h[j+1];
        }
        l += tmp;
        ans = min(ans, l+r+abs(h[it]-w[i+1]));
        pre = it;
    }
    cout << ans << endl;
    return 0;
}