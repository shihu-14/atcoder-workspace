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
    vector<int> a(n), b(n);
    rep(i, n){
        cin >> a[i];
    }
    rep(i, n){
        cin >> b[i];
    }
    int ans = inner_product(a.begin(), a.end(), b.begin(), 0);
    cerr << ans << endl;
    cout << (ans == 0 ? "Yes" : "No") << endl;
    return 0;
}