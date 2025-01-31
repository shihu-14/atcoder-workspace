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
    ll n; int k; cin >> n >> k;
    rep(i, k){
        if (n % 200 == 0) n /= 200;
        else n = n * 1000 + 200;
    }
    cout << n << endl;
    return 0;
}