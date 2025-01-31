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
    ll x, k, d; cin >> x >> k >> d;
    if (x < 0) x = -x;
    if (x/d > k){
        cout << x-k*d << endl;
        return 0;
    }

    k -= x/d;
    x -= (x/d)*d;
    if (k&1){
        cout << abs(x-d) << endl;
    }
    else{
        cout << x << endl;
    }
    return 0;
}