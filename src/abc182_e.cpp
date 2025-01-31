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
    int h, w, n, m; cin >> h >> w >> n >> m;
    vector<vector<P>> x(h), y(w);
    rep(i, n){
        int a, b; cin >> a >> b;
        a--; b--;
        x[a].emplace_back(b, 0);
        y[b].emplace_back(a, 0);
    }
    rep(i, m){
        int c, d; cin >> c >> d;
        c--; d--;
        x[c].emplace_back(d, 1);
        y[d].emplace_back(c, 1);
    }
    rep(i, h) sort(x[i].begin(), x[i].end());
    rep(i, w) sort(y[i].begin(), y[i].end());

    int ans = 0;
    rep(i, h)rep(j, w){
        int it1 = lower_bound(x[i].begin(), x[i].end(), make_pair(j, -1)) - x[i].begin();
        if (it1 < x[i].size() && x[i][it1].first == j && x[i][it1].second == 1){
            continue;
        }
        if (it1 < x[i].size() && x[i][it1].second == 0){
            ans++;
            continue;
        }
        if (it1-1 >= 0 && x[i][it1-1].second == 0){
            ans++;
            continue;
        }

        int it2 = lower_bound(y[j].begin(), y[j].end(), make_pair(i, -1)) - y[j].begin();
        if (it2 < y[j].size() && y[j][it2].second == 0){
            ans++;
            continue;
        }
        if (it2-1 >= 0 && y[j][it2-1].second == 0){
            ans++;
            continue;
        }
    }
    cout << ans << endl;
    return 0;
}