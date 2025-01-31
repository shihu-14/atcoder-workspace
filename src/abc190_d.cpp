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
    vector<ll> divi;
    for(ll i = 1; i*i <= 2*n; i++){
        if (i*i != 2*n && (2*n)%i == 0){
            divi.push_back(i);
            divi.push_back((2*n)/i);
        }
        else if (i*i == 2*n){
            divi.push_back(i);
        }
    }
    int ans = 0;
    int m = divi.size();
    rep(i, m){
        ll d = divi[i];
        ll r = 2*n/d - d + 1;
        if (~r&1){
            // cerr << d << " " << r/2 << endl;s
            ans++;
        }
    }
    cout << ans << endl;
    return 0;
}