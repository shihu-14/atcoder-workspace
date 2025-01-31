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
    ll ans = 0;
    vector<int> c(n+1);
    for(int i = 1; i <= n; i++){
        for(int j = i; j <= n; j+=i){
            c[j]++;
        }
    }
    for(int i = 1; i <= n; i++){
        ans += (ll)c[i]*i;
    }
    cout << ans << endl;
    return 0;
}