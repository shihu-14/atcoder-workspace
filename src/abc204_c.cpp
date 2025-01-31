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
    vector<vector<int>> g(n);
    rep(i, m){
        int a, b; cin >> a >> b;
        a--; b--;
        g[a].push_back(b);
    }
    vector<bool> used(n, false);
    auto f = [&](auto f, int now) -> void{
        for (int to : g[now]){
            if (used[to]) continue;
            used[to] = true;
            f(f, to);
        }
    };
    int ans = 0;
    rep(i, n){
        used[i] = true;
        f(f, i);
        rep(j, n) if (used[j]) ans++;
        fill(used.begin(), used.end(), false);
    }
    cout << ans << endl;
    return 0;
}