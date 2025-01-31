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
    int n; cin >> n;
    vector<P> p;
    vector<int> a(n), b(n);
    rep(i, n){
        cin >> a[i] >> b[i];
        p.emplace_back((ll)2*a[i]+b[i], i);      
    }
    sort(p.rbegin(), p.rend());
    ll sum_a = 0, sum_b = 0;
    rep(i, n) sum_a += a[i];
    rep(i, n){
        auto [_, id] = p[i];
        sum_b += a[id]+b[id];
        sum_a -= a[id];
        if (sum_a < sum_b){
            cout << i+1 << endl;
            return 0;
        }
    }
    return 0;
}