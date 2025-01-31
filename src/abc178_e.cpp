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
    vector<ll> x(n), y(n), nx(n), ny(n);
    rep(i, n) cin >> x[i] >> y[i];
    rep(i, n) nx[i] = x[i] - y[i];
    rep(i, n) ny[i] = x[i] + y[i];

    sort(nx.begin(), nx.end());
    sort(ny.begin(), ny.end());    

    cout << max(nx[n-1] - nx[0], ny[n-1] - ny[0]) << endl;
    return 0;
}