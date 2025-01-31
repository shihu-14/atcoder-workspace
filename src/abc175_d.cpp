#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
#define rep(i, n) for (int i = 0; i < (n); ++i)
using ll = long long;
using P = pair<int, int>;
using T = tuple<int, int, int>;
const ll inf = 1e18;
int main(){
    int n, k; cin >> n >> k;
    vector<ll> p(n), c(n);
    rep(i, n) cin >> p[i];
    rep(i, n) cin >> c[i];
    rep(i, n) p[i]--;
    ll ans = -inf;

    rep(ri, n){
        vector<int> root, ra;
        int now = ri, rs;
        ll raa = 0;
        while(1){
            int to = p[now];
            if (to == ri){
                root.push_back(to);
                ra.push_back(c[to]);
                raa += c[to];
                break;
            }
            root.push_back(to);
            ra.push_back(c[to]);
            raa += c[to];
            now = to;
        }
        rs = root.size();
        // cout << rs << " " << raa << endl;
        // rep(i, rs) cout << root[i] << " " << ra[i] << endl;
        if (k <= rs){
            ll res = 0;
            for(int i = 0; i < k; i++){
                res += c[root[i]];
                ans = max(ans, res);
            }
            continue;
        }

        int m = k; ll res = 0;
        for(int i = 0; i < rs; i++){
            res += c[root[i]];
            m--;
            if (raa > 0){
                ans = max(ans, res+raa*(m/rs));
            }
            ans = max(ans, res);
        }
    }
    cout << ans << endl;
    return 0;
}