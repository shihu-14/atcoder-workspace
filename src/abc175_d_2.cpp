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

    rep(i, n){
        vector<int> root, used(n, -1);
        int now = i, rs = 0, rd = -1, rc = 0, ri = -1;
        root.push_back(now);
        used[now] = rs; rs++;
        while(1){
            int to = p[now];
            if (used[to] != -1){
                rd = to;
                rc = rs-used[to];
                ri = used[to];
                break;
            }
            root.push_back(to);
            used[to] = rs++;
            now = to;
        }

        ll ra = 0;
        for(int j = ri; j < rs; j++){
            ra += c[root[j]];
        }

        if (k < rs){
            ll res = 0;
            for(int j = 1; j <= k; j++){
                res += c[root[j]];
                ans = max(ans, res);
            }
            continue;
        }

        int m = k; ll res = 0;
        for(int j = 1; j < rs; j++){
            res += c[root[j]];
            ans = max(ans, res);
            m--;
        }

        if (m > 0){
            res += c[root[ri]];
            ans = max(ans, res);
            m--;
        }
        if (m != 0 && m%rc == 0){
            res += (m/rc-1)*ra;
            ans = max(ans, res);
            rep(j, rc){
                res += c[root[ri+j]];
                ans = max(ans, res);
            }
            ans = max(ans, res);
            continue;
        }

        res += (m/rc)*ra;
        m %= rc;
        ans = max(ans, res);
        for(int j = 0; j < m; j++){
            res += c[root[ri+j]];
            ans = max(ans, res);
        }
    }
    cout << ans << endl;
    return 0;
}