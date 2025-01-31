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
    vector<ll> a(n+1);
    rep(i, n) cin >> a[i+1];
    vector<ll> b(n+1);
    rep(i, n){
        b[i+1] = a[i+1]-a[i]-1;
    }
    rep(i, n) b[i+1] += b[i];
    rep(i, q){
        ll k; cin >> k;
        int it = lower_bound(b.begin(), b.end(), k) - b.begin();
        cout << k+it-1 << endl;
    }
    return 0;
}