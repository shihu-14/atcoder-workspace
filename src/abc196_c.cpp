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
    ll n; cin >> n;
    vector<ll> ten(9, 1);
    rep(i, 8) ten[i+1] = ten[i]*10;
    int ans = 0;
    for(int i = 1; i < 1000000; i++){
        string s = to_string(i);
        int m = s.size();
        if ((ll)i*ten[m]+i > n) break;
        ans++;
    }
    cout << ans << endl;
    return 0;
}