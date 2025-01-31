#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
#define rep(i, n) for (int i = 0; i < (n); ++i)
using ll = long long;
using P = pair<ll, int>;
using T = tuple<int, int, int>;

int main(){
    int n ; ll k; cin >> n >> k;
    vector<P> p;
    rep(i, n){
        ll a; int b; cin >> a >> b;
        p.push_back({a, b});
    }
    sort(p.begin(), p.end());
    p.insert(p.begin(), {0, 0});
    rep(i, n){
        auto [a1, b1] = p[i];
        auto [a2, b2] = p[i+1];
        if (k < a2-a1){
            cout << a1+k << endl;
            return 0;
        }
        else{
            k -= a2-a1;
            k += b2;
        }
    }
    cout << p[n].first+k << endl;
    return 0;
}