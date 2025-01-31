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
    for(int i = 1; i < n; i++){
        for(int j = 1; j*j <= i; j++){
            if (i%j) continue;
            if (j*j == i) ans++;
            else ans += 2;
        }
    }
    cout << ans << endl;
    return 0;
}