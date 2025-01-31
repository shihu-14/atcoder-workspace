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
    ll ans = 0;
    rep(i, n){
        ll a, b; cin >> a >> b;
        ans += (a+b)*(b-a+1)/2;
    }
    cout << ans << endl;
    return 0;
}