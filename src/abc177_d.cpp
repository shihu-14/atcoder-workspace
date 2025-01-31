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
    dsu uf(n);
    rep(i, m){
        int a, b; cin >> a >> b;
        uf.merge(a-1, b-1);
    }
    int ans = 0;
    rep(i, n){
        ans = max(ans, uf.size(i));
    }
    cout << ans << endl;
    return 0;
}