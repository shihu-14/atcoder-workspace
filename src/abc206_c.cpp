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
    vector<int> a(n);
    rep(i, n) cin >> a[i];

    ll ans = (ll)n*(n-1)/2;
    map<int, int> mp;
    rep(i, n){
        mp[a[i]]++;
    }
    for (auto[k, v] : mp){
        ans -= (ll)v*(v-1)/2;
    }
    cout << ans << endl;
    return 0;
}