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
    set<ll> ans;
    for (ll i = 2; i*i <= n; i++){
        for (ll j = i*i; j <= n; j*=i){
            ans.insert(j);
        }
    }
    cout << n-ans.size() << endl;
    return 0;
}