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
    ll a, b, c, d; cin >> a >> b >> c >> d;
    if (c*d-b <= 0){
        cout << -1 << endl;
        return 0;
    }
    ll e = c*d-b;
    cout << (a+e-1)/e << endl;
    return 0;
}