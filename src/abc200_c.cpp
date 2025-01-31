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
    vector<int> c(200);
    rep(i, n){
        int a; cin >> a;
        c[a%200]++;
    }
    ll ans = 0;
    rep(i, 200){
        ans += (ll)c[i]*(c[i]-1)/2;
    }
    cout << ans << endl;
    return 0;
}