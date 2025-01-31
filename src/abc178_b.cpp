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
    cout << max({a*c, b*d, a*d, b*c}) << endl;
    return 0;
}