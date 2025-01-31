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
    for(ll i = 1; i*i <= n; i++){
        if (n%i) continue;
        if (i*i == n){
            divi.push_back(i);
        }
        else{
            divi.push_back(i);
            divi.push_back(n/i);
        }
    }
    sort(divi.begin(), divi.end());
    rep(i, divi.size()) cout << divi[i] << endl;
    return 0;
}