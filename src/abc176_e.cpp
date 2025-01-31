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
    int h, w, m; cin >> h >> w >> m;
    vector<int> r(300010), c(300010);
    map<P, bool> mp;
    rep(i, m){
        int a, b; cin >> a >> b;
        r[a]++; c[b]++;
        mp[{a, b}] = true;
    }
    int max_r = 0, max_c = 0;
    rep(i, 300001){
        max_r = max(max_r, r[i]);
        max_c = max(max_c, c[i]);
    }

    int ri = 0, ci = 0;
    rep(i, 300001){
        if (r[i] == max_r) ri++;
        if (c[i] == max_c) ci++;
    }

    int ans = max_r+max_c-1;
    int res = 0;
    for (auto [k, v]: mp){
        auto [a, b] = k;
        if (r[a] == max_r && c[b] == max_c) res++;
    }
    if (res < (ll)ri*ci) ans++;
    cout << ans << endl;
    return 0;
}