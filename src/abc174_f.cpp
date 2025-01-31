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
    int n, q; cin >> n >> q;
    vector<vector<int>> c(n);
    rep(i, n){
        int cv; cin >> cv; cv--;
        c[cv].emplace_back(i);
    }
    vector<T> query;
    rep(i, n){
        if (c[i].size() < 2) continue;
        rep(j, c[i].size()-1){
            query.emplace_back(c[i][j], q, c[i][j+1]);
        }
    }
    rep(i, q){
        int l, r; cin >> l >> r; l--; r--;
        query.emplace_back(l, i, r);
    }

    sort(query.rbegin(),  query.rend());
    fenwick_tree<int> fw(n);
    vector<int> ans(q);
    for(auto [l, id, r]: query){
        if (id == q){
            fw.add(r, 1);
            continue;
        }
        int res = r-l+1;
        res -= fw.sum(0, r+1);
        ans[id] = res;
    }
    rep(i, q) cout << ans[i] << endl;
    return 0;
}