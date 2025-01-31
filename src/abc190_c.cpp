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
    vector<P> p;
    rep(i, m){
        int a, b; cin >> a >> b; a--, b--;
        p.emplace_back(a, b);
    }
    int k; cin >> k;
    vector<P> q;
    rep(i, k){
        int c, d; cin >> c >> d; c--, d--;
        q.emplace_back(c, d);
    }
    int ans = -1;
    rep(i, 1<<k){
        vector<int> cnt(n);
        rep(j, k){
            auto [c, d] = q[j];
            if (i&(1<<j)){
                cnt[c]++;
            }
            else cnt[d]++;
        }
        int res = 0;
        rep(j, m){
            auto [a, b] = p[j];
            if (cnt[a] && cnt[b]) res++;
        }
        ans = max(ans, res);
    }
    cout << ans << endl;
    return 0;
}