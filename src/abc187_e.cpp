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
    vector<vector<int>> g(n);
    vector<P> edge;
    rep(i, n-1){   
        int a, b; cin >> a >> b;
        a--; b--;
        g[a].push_back(b);
        g[b].push_back(a);
        edge.push_back({a, b});
    }
    map<P, bool> mp;
    auto f = [&](auto f, int now, int pre)->void{
        for(auto to: g[now]){
            if (to == pre) continue;
            mp[{now, to}] = true;
            f(f, to, now);
        }
    };
    f(f, 0, -1);
    int q; cin >> q;
    vector<ll> ans(n);
    rep(i, q){
        int t, e, x; cin >> t >> e >> x; e--;
        auto [a, b] = edge[e];
        if (t == 2) swap(a, b);
        if (mp[{a, b}]){
            ans[0] += x;
            ans[b] -= x;
        }else{
            ans[a] += x;
        }
    }
    auto h = [&](auto h, int now, int pre) -> void{
        for(auto to: g[now]){
            if (to == pre) continue;
            ans[to] += ans[now];
            h(h, to, now);
        }
    };
    h(h, 0, -1);
    rep(i, n) cout << ans[i] << endl;
    return 0;
}