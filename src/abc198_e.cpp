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
    vector<int> c(n);
    rep(i, n) cin >> c[i];
    vector<vector<int>> g(n);
    rep(i, n-1){
        int a, b; cin >> a >> b;
        a--, b--;
        g[a].push_back(b);
        g[b].push_back(a);
    }

    vector<int> ans = {0};
    multiset<int> used = {c[0]};
    auto f = [&](auto f, int now, int pre) -> void{
        for (auto to : g[now]){
            if (to == pre) continue;
            if (!used.count(c[to])){
                ans.push_back(to);
            }
            used.insert(c[to]);
            f(f, to, now);
            used.erase(used.find(c[to]));
        }  
    };
    f(f, 0, -1);

    sort(ans.begin(), ans.end());
    for (auto i : ans) cout << i+1 << endl;

    return 0;
}