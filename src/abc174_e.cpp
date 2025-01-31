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
    int n, k; cin >> n >> k;
    vector<int> a(n);
    rep(i, n) cin >> a[i];
    int max_l = 0;
    rep(i, n) max_l = max(max_l, a[i]);

    auto f = [&](int w) -> bool{
        ll res = 0;
        rep(i, n){
            if (a[i]%w == 0) res += a[i]/w-1;
            else res += a[i]/w;
        }
        return res <= k;
    };

    int wa = 0, ac = max_l;
    while(ac-wa > 1){
        int wj = (ac+wa)/2;
        if (f(wj)) ac = wj;
        else wa = wj;
    }
    cout << ac << endl;
    return 0;
}