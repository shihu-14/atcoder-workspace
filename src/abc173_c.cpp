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
    int h, w, k; cin >> h >> w >> k;
    vector<string> g(h);
    rep(i, h) cin >> g[i];

    vector<int> r(h), c(w);
    int sum_b = 0;
    rep(i, h)rep(j, w){
        if (g[i][j] == '#'){
            r[i]++, c[j]++;
            sum_b++;
        }
    }
    int ans = 0;
    rep(i, 1<<h)rep(j, 1<<w){
        int res = 0;
        vector<int> ri, ci;
        rep(k, h) if (i>>k&1){
            res += r[k];
            ri.emplace_back(k);
        }
        rep(k, w) if (j>>k&1){
            res += c[k];
            ci.emplace_back(k);
        }
        for(auto a: ri) for(auto b: ci) if (g[a][b] == '#') res--;
        if (sum_b-res == k){
            ans++;
        }
    }
    cout << ans << endl;
    return 0;
}